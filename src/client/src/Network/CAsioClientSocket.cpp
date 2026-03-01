/*
 * CAsioClientSocket.cpp
 *
 * Implementation of the ASIO-backed client socket that replaces the legacy
 * WinSock CRawSOCKET / CClientSOCKET / CshoSOCKET stack.
 *
 * Two-phase receive (mirrors CRoseSocket::onReceived in osIROSE-new):
 *   Phase 1 – ASIO reads 6 bytes (header).  onReceived() decodes the header
 *             with PacketCodec::decodeServerHeader (or reads the raw size if
 *             DISABLE_CRYPT) and sets pktSize to the full packet length.
 *             The network layer then reads (pktSize – 6) more bytes.
 *   Phase 2 – ASIO reads the body.  onReceived() decodes it, copies the full
 *             packet into the receiver queue, then calls reset_internal_buffer()
 *             to restart the cycle.
 *
 * Thread-safety: the receiver queue (m_recvQ / m_recvMutex) is the only shared
 * state between the ASIO thread pool and the game-loop thread that calls
 * Peek_Packet().  All other members are written only during construction or
 * from within the single-threaded game loop.
 */

#include "StdAfx.h"

// ---- Own header ----
#include "CAsioClientSocket.h"

// ---- ASIO networking (pulled in here, not in the .h) ----
#include <cnetwork_asio.h>

// ---- Game-level packet types ----
#include "Net_Prototype.h"      // t_PACKET, t_PACKETHEADER definitions
#include "epackettype.h"        // RoseCommon::ePacketType (PAKSS_ACCEPT_REPLY, …)
#include "packetfactory.h"      // RoseCommon::fetchPacket<>

#include <cstring>              // std::memcpy

using namespace RoseCommon;

//-------------------------------------------------------------------------------------------------
// Construction / destruction
//-------------------------------------------------------------------------------------------------

CAsioClientSocket::CAsioClientSocket()
    : m_iSendSEQ(0)
    , m_connected(false)
{
    auto net = std::make_unique<Core::CNetwork_Asio>();

    // Wire up the three callbacks we care about
    net->registerOnConnected([this]() {
        onConnected();
    });

    net->registerOnDisconnected([this]() {
        onDisconnected();
    });

    net->registerOnReceived([this](uint16_t id, uint16_t& pktSize, uint8_t* buf) -> bool {
        return onReceived(id, pktSize, buf);
    });

    m_network = std::move(net);
}

CAsioClientSocket::~CAsioClientSocket() {
    Close();
}

//-------------------------------------------------------------------------------------------------
// Connection management
//-------------------------------------------------------------------------------------------------

bool CAsioClientSocket::Connect(const char* szServerIP, uint16_t port) {
    if (!szServerIP || port == 0)
        return false;

    if (!m_network->init(szServerIP, port))
        return false;

    return m_network->connect();
    // onConnected() will be called asynchronously once TCP (and optional SSL)
    // handshake completes; it starts the receive loop and posts
    // NETWORK_STATUS_CONNECT into the queue.
}

void CAsioClientSocket::Close() {
    if (m_connected) {
        m_connected = false;
        m_network->shutdown(/*_final=*/true);
    }
}

//-------------------------------------------------------------------------------------------------
// Session hooks  (mirror of CshoClientSOCK's mF_Init / OnAccepted / Set_NetSTATUS)
//-------------------------------------------------------------------------------------------------

void CAsioClientSocket::mF_Init(uint32_t dwInitCode) {
    // In CshoClientSOCK:
    //   dwInitCode != 0 → CPacketCODEC::Init(code)     → set new XOR seed
    //   dwInitCode == 0 → CPacketCODEC::Default()       → use the built-in seed
    // RoseCommon::PacketCodec uses 0x0042D266 by default and exposes changeSeed().
#ifndef DISABLE_CRYPT
    if (dwInitCode)
        m_crypt.changeSeed(dwInitCode);
    // else: keep the default seed that was set in the constructor
#endif
    (void)dwInitCode;
}

void CAsioClientSocket::OnAccepted(int* pSendSEQ) {
    // Store the first server-assigned socket ID as the send-sequence counter.
    // The legacy CPacketCODEC::Encode_SendClientPACKET used this, but
    // RoseCommon::PacketCodec::encodeClientPacket does not need it directly.
    m_iSendSEQ = pSendSEQ[0];
}

void CAsioClientSocket::Set_NetSTATUS(uint8_t btStatus) {
    enqueueStatus(btStatus);
}

//-------------------------------------------------------------------------------------------------
// Packet send
//-------------------------------------------------------------------------------------------------

void CAsioClientSocket::Packet_Register2SendQ(const t_PACKET* pRegPacket) {
    if (!m_connected)
        return;

    const uint16_t size = static_cast<uint16_t>(pRegPacket->m_HEADER.m_nSize);
    auto data = std::make_unique<uint8_t[]>(size);
    std::memcpy(data.get(), pRegPacket, size);

#ifndef DISABLE_CRYPT
    m_crypt.encodeClientPacket(data.get());
#endif

    m_network->send_data(std::move(data));
}

void CAsioClientSocket::Packet_Register2SendQ(RoseCommon::CRosePacket&& pRegPacket) {
    if (!m_connected)
        return;

    auto data = pRegPacket.getPacked();

#ifndef DISABLE_CRYPT
    m_crypt.encodeClientPacket(data.get());
#endif

    m_network->send_data(std::move(data));
}

//-------------------------------------------------------------------------------------------------
// Packet receive (game-loop side)
//-------------------------------------------------------------------------------------------------

bool CAsioClientSocket::Peek_Packet(t_PACKET* outPacket, bool bRemoveFromQ) {
    std::lock_guard<std::mutex> guard(m_recvMutex);
    if (m_recvQ.empty())
        return false;

    const auto& [size, data] = m_recvQ.front();
    std::memcpy(outPacket, data.get(), size);

    if (bRemoveFromQ)
        m_recvQ.pop_front();

    return true;
}

std::optional<std::unique_ptr<RoseCommon::CRosePacket>>
CAsioClientSocket::Peek_Packet(bool bRemoveFromQ) {
    std::lock_guard<std::mutex> guard(m_recvMutex);
    if (m_recvQ.empty())
        return {};

    const auto& [size, data] = m_recvQ.front();

    // fetchPacket<true> = server-sent packet
    auto res = RoseCommon::fetchPacket<true>(data.get());

    if (bRemoveFromQ)
        m_recvQ.pop_front();

    return res;
}

//-------------------------------------------------------------------------------------------------
// Private: ASIO receive callback  (called on an ASIO thread-pool thread)
//
// The INetwork two-phase receive contract:
//   • First call:  pktSize == 6           → header was just read into buffer[0..5]
//                  Decode header, set pktSize = full packet size.
//                  Return true → ASIO reads (pktSize-6) more bytes.
//   • Second call: pktSize == full size   → body was just appended to buffer[6..]
//                  Decode body, queue complete packet, call reset_internal_buffer().
//                  Return true → ASIO resets to 6-byte header read (via reset_internal_buffer).
//-------------------------------------------------------------------------------------------------

bool CAsioClientSocket::onReceived(uint16_t /*socketId*/,
                                   uint16_t& pktSize,
                                   uint8_t*  buffer)
{
    if (pktSize == 6) {
        // ---- Phase 1: header received ----
        uint16_t realSize;
#ifndef DISABLE_CRYPT
        realSize = m_crypt.decodeServerHeader(buffer);
#else
        // Raw: first two bytes of the header are the packet length
        realSize = *reinterpret_cast<uint16_t*>(buffer);
#endif

        if (realSize == 0 || realSize > MAX_PACKET_SIZE) {
            // Malformed packet — tell the network layer to disconnect
            return false;
        }

        pktSize = realSize;  // the network layer will now read (realSize - 6) more bytes
        return true;
    }

    // ---- Phase 2: full packet received ----
#ifndef DISABLE_CRYPT
    if (!m_crypt.decodeServerBody(buffer)) {
        return false;   // decryption failure → disconnect
    }
#endif

    // Copy the decoded packet into the receive queue
    const uint16_t size = pktSize;
    auto data = std::make_unique<uint8_t[]>(size);
    std::memcpy(data.get(), buffer, size);

    {
        std::lock_guard<std::mutex> guard(m_recvMutex);
        m_recvQ.emplace_back(size, std::move(data));
    }

    // Reset the network layer's internal offset/size counters so the next
    // async_read starts fresh at 6-byte header mode.
    m_network->reset_internal_buffer();

    return true;
}

//-------------------------------------------------------------------------------------------------
// Private: ASIO connection callbacks  (called on an ASIO thread-pool thread)
//-------------------------------------------------------------------------------------------------

void CAsioClientSocket::onConnected() {
    m_connected = true;

    // Notify the game loop that a connection was established.
    enqueueStatus(NETWORK_STATUS_CONNECT);

    // Kick off the receive loop.  For non-SSL builds active_ is set to true by
    // CNetwork_Asio::connect() after this callback returns, but the first
    // async_read will already be queued; subsequent reads check is_active() and
    // will continue normally.  For SSL builds active_ is set before this
    // callback fires.
    m_network->recv_data();
}

void CAsioClientSocket::onDisconnected() {
    m_connected = false;
    enqueueStatus(NETWORK_STATUS_DISCONNECT);
}

//-------------------------------------------------------------------------------------------------
// Private: helpers
//-------------------------------------------------------------------------------------------------

void CAsioClientSocket::enqueueStatus(uint8_t btStatus) {
    // Build a synthetic PAKSS_ACCEPT_REPLY packet that CNetwork::Proc()
    // switches on, exactly as CClientSOCKET::Set_NetSTATUS() did.
    t_NETWORK_STATUS pkt{};
    pkt.m_wType     = to_underlying(ePacketType::PAKSS_ACCEPT_REPLY);
    pkt.m_nSize     = static_cast<short>(sizeof(t_NETWORK_STATUS));
    pkt.m_btStatus  = btStatus;

    const uint16_t size = static_cast<uint16_t>(pkt.m_nSize);
    auto data = std::make_unique<uint8_t[]>(size);
    std::memcpy(data.get(), &pkt, size);

    std::lock_guard<std::mutex> guard(m_recvMutex);
    m_recvQ.emplace_back(size, std::move(data));
}
