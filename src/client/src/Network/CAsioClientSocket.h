/*
 * CAsioClientSocket.h
 *
 * Replaces the legacy CRawSOCKET -> CClientSOCKET -> CshoSOCKET WinSock stack
 * with a modern ASIO-backed socket.  When built with USE_SSL (set by ENABLE_SSL
 * in CMake) the underlying CNetwork_Asio uses asio::ssl::stream; without it
 * a plain TCP socket is used.  XOR packet encryption is still performed via
 * RoseCommon::PacketCodec unless DISABLE_CRYPT is defined.
 *
 * Public interface mirrors the methods that CNetwork (and the rest of the
 * client code) calls on CshoClientSOCK:
 *   Connect / Close
 *   mF_Init / OnAccepted / Set_NetSTATUS
 *   Packet_Register2SendQ (two overloads)
 *   Peek_Packet             (two overloads)
 */
#pragma once
#ifndef __CASIO_CLIENT_SOCKET_H
#define __CASIO_CLIENT_SOCKET_H

#include "../Util/PacketHEADER.h"
#include "crosepacket.h"    // RoseCommon::CRosePacket
#ifndef DISABLE_CRYPT
#include "crosecrypt.h"     // RoseCommon::PacketCodec
#endif

#include <list>
#include <tuple>
#include <mutex>
#include <optional>
#include <memory>
#include <cstdint>

struct t_PACKET;

// Forward-declare INetwork so that ASIO headers are not pulled into every
// translation unit that includes this header.
namespace Core { class INetwork; }

//-------------------------------------------------------------------------------------------------

class CAsioClientSocket {
public:
     CAsioClientSocket();
    ~CAsioClientSocket();

    // Non-copyable / non-movable (owns the network backend)
    CAsioClientSocket(const CAsioClientSocket&)            = delete;
    CAsioClientSocket& operator=(const CAsioClientSocket&) = delete;

    // ----- Connection management -----

    /// Resolve and connect to the given server (non-blocking: callbacks fire on ASIO threads).
    bool Connect(const char* szServerIP, uint16_t port);

    /// Shut down the socket immediately.
    void Close();

    // ----- Session hooks (mirror of CshoClientSOCK) -----

    /// Called when the server sends the init seed for the XOR codec.
    /// dwInitCode == 0 → use the default seed; otherwise call changeSeed().
    void mF_Init(uint32_t dwInitCode);

    /// Called when the server replies with socket IDs; stores the first ID as
    /// the send-sequence counter (kept for compatibility; not used by
    /// RoseCommon::PacketCodec directly).
    void OnAccepted(int* pSendSEQ);

    /// Synthesise a synthetic PAKSS_ACCEPT_REPLY status packet and push it
    /// into the receive queue so CNetwork::Proc() can dispatch it normally.
    void Set_NetSTATUS(uint8_t btStatus);

    // ----- Packet I/O -----

    void Packet_Register2SendQ(const t_PACKET* pRegPacket);
    void Packet_Register2SendQ(RoseCommon::CRosePacket&& pRegPacket);

    /// Copy the front packet into outPacket.  Returns false if the queue is empty.
    bool Peek_Packet(t_PACKET* outPacket, bool bRemoveFromQ = true);

    /// Return the front packet as a CRosePacket (server-side).  Empty optional if queue empty.
    std::optional<std::unique_ptr<RoseCommon::CRosePacket>> Peek_Packet(bool bRemoveFromQ = true);

private:
    // ---- ASIO network backend ----
    std::unique_ptr<Core::INetwork> m_network;

    // ---- XOR packet codec ----
#ifndef DISABLE_CRYPT
    RoseCommon::PacketCodec m_crypt;
#endif
    int m_iSendSEQ;     ///< send-sequence counter (from OnAccepted)

    // ---- Receive queue ----
    using Node = std::tuple<uint16_t, std::unique_ptr<uint8_t[]>>;
    std::list<Node> m_recvQ;
    std::mutex      m_recvMutex;

    // ---- State ----
    bool m_connected;

    // ---- Internal ASIO callbacks ----
    bool onReceived(uint16_t socketId, uint16_t& pktSize, uint8_t* buffer);
    void onConnected();
    void onDisconnected();

    // ---- Helpers ----
    void enqueueStatus(uint8_t btStatus);
};

//-------------------------------------------------------------------------------------------------
#endif // __CASIO_CLIENT_SOCKET_H
