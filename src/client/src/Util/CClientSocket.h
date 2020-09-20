/*
	$Header: /7HeartsOnline/Client/Util/CClientSOCKET.h 3     04-11-05 12:14a Icarus $
*/
#ifndef	__CClientSOCKET_H
#define	__CClientSOCKET_H
#include "CRawSOCKET.h"
#include "PacketHEADER.h"
#include "crosepacket.h"
#include <list>
#include <tuple>
#include <mutex>
#include <optional>

//-------------------------------------------------------------------------------------------------

struct t_PACKET;

class CClientSOCKET : public CRawSOCKET {
private:
  bool _Init(void);
  void _Free(void);

  uint8_t   m_pRecvPacket[MAX_PACKET_SIZE];
  short     m_nRecvBytes;
  short     m_nPacketSize;

  short m_nSendBytes;
  bool  m_bWritable;

  std::mutex   m_recvMutex;
  std::mutex   m_waitMutex;
  std::mutex   m_sendMutex;
  HANDLE       m_hThreadEvent;
  HANDLE       m_hThread;
  // DWORD	   m_dwThreadID;
  unsigned int m_dwThreadID;

  char m_cStatus;

  // <size, data>
  using Node = std::tuple<uint16_t, std::unique_ptr<uint8_t[]>>;

  std::list<Node>       m_RecvPacketQ;
  std::list<Node>			  m_SendPacketQ;
  std::list<Node>			  m_WaitPacketQ;

  //classDLLIST<struct tagUDPPACKET *> m_RecvUDPPacketQ;

protected:
  //friend	DWORD WINAPI ClientSOCKET_SendTHREAD (LPVOID lpParameter);
  friend unsigned __stdcall ClientSOCKET_SendTHREAD(void* lpParameter);

  void Packet_Recv(int iToRecvBytes);
  bool Packet_Send(void);

  virtual WORD  mF_ESP(t_PACKETHEADER* pPacket);
  virtual WORD  mF_DRH(t_PACKETHEADER* pPacket);
  virtual short mF_DRB(t_PACKETHEADER* pPacket);

public:
  classDLLNODE<CClientSOCKET *>* m_pNode;

          CClientSOCKET() { _Init(); }
  virtual ~CClientSOCKET() { _Free(); }

  void Packet_Register2RecvUDPQ(u_long ulFromIP, WORD wFromPort, int iPacketSize);
  void Packet_RecvFrom(void            );
  void SetSendEvent() { SetEvent( m_hThreadEvent ); }

  virtual void mF_Init(DWORD   dwInit);
  void         OnConnect(int   nErrorCode) override;
  void         OnClose(int     nErrorCode) override;
  void         OnReceive(int   nErrorCode) override;
  void         OnSend(int      nErrorCode) override;
  virtual void OnAccepted(int* pCode);
  virtual bool WndPROC(WPARAM  wParam, LPARAM lParam) =0;

public:
  // user interface
  bool Connect(HWND hWND, char* szServerIP, int iTCPPort, UINT uiWindowMsg);
  void Close() override;

  virtual void Set_NetSTATUS(BYTE btStatus);

  void Packet_Register2RecvQ(const t_PACKET* const pRegPacket);
  void Packet_Register2RecvQ(RoseCommon::CRosePacket&& pRegPacket);
  void Packet_Register2SendQ(const t_PACKET* const pRegPacket);
  void Packet_Register2SendQ(RoseCommon::CRosePacket&& pRegPacket);
  bool Peek_Packet(t_PACKET* outPacket, bool bRemoveFromQ = true);
  std::optional<std::unique_ptr<RoseCommon::CRosePacket>> Peek_Packet(bool bRemoveFromQ = true);
};

//-------------------------------------------------------------------------------------------------
#endif
