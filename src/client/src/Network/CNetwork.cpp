/*
  $Header: /Client/Network/CNetwork.cpp 168   05-10-18 3:10p Young $
*/
#include "StdAfx.h"
#include "CNetwork.h"
#include "../Interface/ExternalUI/ExternalUILobby.h"
#include "../Interface/it_mgr.h"
#include "../System/CGame.h"
#include "../GameProc/LiveCheck.h"
#include "../Util/CSocketWND.h"
#include "Debug.h"
#include "Game.h"
#include "IO_Terrain.h"
#include "cli_accept_req.h"
#include "epackettype.h"

////005. 5. 23 박 지호
//#include "../nProtect/NProtect.h"

CNetwork* g_pNet;

CNetwork* CNetwork::m_pInstance = nullptr;

#define WM_WORLD_SOCKET_NOTIFY (WM_SOCKETWND_MSG + 0)
#define WM_ZONE_SOCKET_NOTIFY (WM_SOCKETWND_MSG + 1)

using namespace RoseCommon;

/*
void	(*fpCMDProc )	(t_unit *pUnit);
typedef	unsigned int UINT;
*/

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
CNetwork::CNetwork(HINSTANCE hInstance) {
  m_btZoneSocketSTATUS = 0;
  m_nProcLEVEL         = NS_NULL;

  CSocketWND* pSockWND = CSocketWND::InitInstance( hInstance, 2 );
  if ( pSockWND ) {
    pSockWND->AddSocket( &this->m_WorldSOCKET, WM_WORLD_SOCKET_NOTIFY );
    pSockWND->AddSocket( &this->m_ZoneSOCKET, WM_ZONE_SOCKET_NOTIFY );
  }
}

CNetwork::~CNetwork() {
  CSOCKET::Free();

  if ( CSocketWND::GetInstance() )
    CSocketWND::GetInstance()->Destroy();
}

CNetwork* CNetwork::Instance(HINSTANCE hInstance) {
  if ( m_pInstance == nullptr ) {
    if ( CSOCKET::Init() ) {
      m_pInstance = new CNetwork( hInstance );
    }
  }

  return m_pInstance;
}

void CNetwork::Destroy() {
  m_WorldSOCKET.Close();
  m_ZoneSOCKET.Close();
  SAFE_DELETE(m_pInstance);
}

void CNetwork::Send_PACKET(t_PACKET* pSendPacket, bool bSendToWorld) {
    if ( bSendToWorld || bAllInONE )
      m_WorldSOCKET.Packet_Register2SendQ( pSendPacket );
    else
      m_ZoneSOCKET.Packet_Register2SendQ( pSendPacket );
  }

void CNetwork::Send_PACKET(RoseCommon::CRosePacket&& packet, bool sendToWorld)
{
  if (sendToWorld || bAllInONE) {
    m_WorldSOCKET.Packet_Register2SendQ(std::move(packet));
  } else {
    m_ZoneSOCKET.Packet_Register2SendQ(std::move(packet));
  }
}

//-------------------------------------------------------------------------------------------------
bool CNetwork::ConnectToServer(char* szServerIP, WORD wTcpPORT,
                               short nProcLEVEL) {
  // World 소켓...
  if ( m_nProcLEVEL == nProcLEVEL )
    return true;

  m_nProcLEVEL = nProcLEVEL;
  return m_WorldSOCKET.Connect( CSocketWND::GetInstance()->GetWindowHandle(),
                                szServerIP, wTcpPORT, WM_WORLD_SOCKET_NOTIFY );
}

//-------------------------------------------------------------------------------------------------

void CNetwork::DisconnectFromServer(short nProcLEVEL) {
  m_nProcLEVEL = nProcLEVEL;

  // Check to see if we shouldn't actually disconnect
  if ( NS_TRN_TO_WSV == nProcLEVEL )
    return;

  m_WorldSOCKET.Close();
  m_WorldSOCKET.Set_NetSTATUS( NETWORK_STATUS_DISCONNECT );
}

//-------------------------------------------------------------------------------------------------
void CNetwork::MoveZoneServer(const bool reconnect) {
  // Check to see if we shouldn't actually disconnect
  if ( NS_TRN_TO_WSV == m_nProcLEVEL )
    return;

  if ( NETWORK_STATUS_CONNECT == m_btZoneSocketSTATUS ) {
    // 존 서버 소켓을 끊고 새로 접속한다...
    m_bMoveingZONE = true;
    m_ZoneSOCKET.Close();

    if ( !reconnect ) {
      return;
    }
    LogString(LOG_NORMAL, "MoveZoneServer reconnection logic hit\n");
  }
  // 바로 접속...
  m_bMoveingZONE = false;
  m_ZoneSOCKET.Connect( CSocketWND::GetInstance()->GetWindowHandle(),
                        m_GSV_IP.Get(), m_wGSV_PORT, WM_ZONE_SOCKET_NOTIFY );
}

//-------------------------------------------------------------------------------------------------
void              CNetwork::Proc_WorldPacket() {
  CshoClientSOCK* pSocket = &this->m_WorldSOCKET;

  std::unique_ptr<t_PACKET> packet(new t_PACKET);
  while ( m_WorldSOCKET.Peek_Packet( packet.get(), true ) ) {
    switch ( packet->m_HEADER.m_wType ) {
      case to_underlying(ePacketType::PAKSS_ACCEPT_REPLY): {
        switch ( packet->m_NetSTATUS.m_btStatus ) {
          case NETWORK_STATUS_ACCEPTED: {
            pSocket->OnAccepted( (int *)packet->m_NetSTATUS.m_dwSocketIDs );
            CGame::GetInstance().AcceptedConnectLoginSvr();
            continue;
          }
          case NETWORK_STATUS_CONNECT: {
            // 서버와 연결됐다...
            switch ( m_nProcLEVEL ) {
              case NS_CON_TO_WSV: // 월드 서버에 접속했다.
                Send_cli_JOIN_SERVER_REQ( m_dwWSV_ID, true );
                m_bWarping = false;
                bAllInONE  = true;
                break;
              case NS_CON_TO_LSV: // 로긴 서버에 접속했다.
                pSocket->mF_Init( 0 );
                Send_PACKET(RoseCommon::Packet::CliAcceptReq::create(), true);
                break;
            }

            g_pCApp->SetCaption( "ROSE online" );
#ifdef __VIRTUAL_SERVER
        g_pCApp->ErrorBOX("가상 서버가 설정되어 있음..", "ERROR !!!", MB_OK);
#endif
            continue;
          }
          case NETWORK_STATUS_DISCONNECT: {
            // g_pCApp->SetCaption ( "Disconnected" );

            if ( NS_DIS_FORM_LSV == m_nProcLEVEL ) {
              // 게임 서버에 재접속 한다...
              this->ConnectToServer( m_WSV_IP.Get(), m_wWSV_PORT, NS_CON_TO_WSV );
              continue;
            }
            CGame::GetInstance().ProcWndMsg( WM_USER_WORLDSERVER_DISCONNECTED, 0, 0 );
            // SendServerDisconnectMsg( "WorldServer" );
            break;
          }
          case NETWORK_STATUS_DERVERDEAD: {
            g_pCApp->SetCaption( "Server DEAD" );
#ifndef __VIRTUAL_SERVER
            g_pCApp->ErrorBOX( STR_SERVER_EXAMINE, STR_SERVER_INFO, MB_OK );
#ifndef _DEBUG
            g_pCApp->SetExitGame();
#endif
#endif
            break;
          }
        }

        // CGame::GetInstance().ProcWndMsg( WM_USER_SERVER_DISCONNECTED,0,0 );
        // 서버와 접속 실패 했다.
        // LogString(LOG_NORMAL, "서버와의 접속에 실패했습니다.\n");
        break;
      }
      case to_underlying(ePacketType::PAKSS_ERROR): Recv_srv_ERROR(packet.get());
        break;

      case to_underlying(ePacketType::PAKSC_JOIN_SERVER_REPLY): // 월드 서버에 접속했다.
      {
        DWORD dwRet = Recv_srv_JOIN_SERVER_REPLY(packet.get());
        if ( dwRet ) {
          pSocket->OnAccepted( (int *)&dwRet );
          this->Send_cli_CHAR_LIST();
        } else {
          // TODO:: error
          this->DisconnectFromServer();
          return;
        }
        break;
      }

      case to_underlying(ePacketType::PAKLC_LOGIN_REPLY):
        using RoseCommon::Packet::SrvLoginReply;
        if ( !Recv_lsv_LOGIN_REPLY(SrvLoginReply::create(reinterpret_cast<const uint8_t*>(packet.get()))) ) {
          this->DisconnectFromServer();
          return;
        }
        break;
      case to_underlying(ePacketType::PAKLC_SRV_SELECT_REPLY): {
        DWORD dwRet = Recv_lsv_SELECT_SERVER(packet.get());
        if ( dwRet )
          pSocket->mF_Init( dwRet );

        if (NS_TRN_TO_WSV == m_nProcLEVEL) {
          Send_cli_JOIN_SERVER_REQ( m_dwWSV_ID, true );
          m_bWarping = false;
          bAllInONE  = true;
        }
        break;
      }
      case to_underlying(ePacketType::PAKLC_CHANNEL_LIST_REPLY): Recv_lsv_CHANNEL_LIST_REPLY(RoseCommon::Packet::SrvChannelListReply::create(reinterpret_cast<const uint8_t*>(packet.get())));
        break;
        // 캐릭터 리스트 받았음
      case to_underlying(ePacketType::PAKCC_CHAR_LIST_REPLY): Recv_wsv_CHAR_LIST(RoseCommon::Packet::SrvCharListReply::create(reinterpret_cast<const uint8_t*>(packet.get())));
        break;

      case to_underlying(ePacketType::PAKCC_DELETE_CHAR_REPLY): Recv_wsv_DELETE_CHAR(packet.get());
        break;
        // 캐릭터 생성요청 결과 통보받음
      case to_underlying(ePacketType::PAKCC_CREATE_CHAR_REPLY): Recv_wsv_CREATE_CHAR(packet.get());

        break;
      case to_underlying(ePacketType::PAKCC_MESSENGER): Recv_tag_MCMD_HEADER(packet.get());
        break;
        case to_underlying(ePacketType::PAKCC_MESSENGER_CHAT): Recv_wsv_MESSENGER_CHAT(packet.get());
        break;
      case to_underlying(ePacketType::PAKCC_MEMO): Recv_wsv_MEMO(packet.get());
        break;
      case to_underlying(ePacketType::PAKCC_CHATROOM): Recv_wsv_CHATROOM(packet.get());
        break;
      case to_underlying(ePacketType::PAKCC_CHATROOM_MESSAGE): Recv_wsv_CHATROOM_MSG(packet.get());
        break;
      case to_underlying(ePacketType::PAKCC_CHAN_CHAR_REPLY): Recv_wsv_CHAR_CHANGE(RoseCommon::Packet::SrvChanCharReply::create(reinterpret_cast<const uint8_t*>(packet.get())));
        break;
        // 존 서버를 이동해라...
      case to_underlying(ePacketType::PAKCC_SWITCH_SERVER): {
        bAllInONE = false;
        Recv_wsv_MOVE_SERVER(packet.get());
        MoveZoneServer();
        break;
      }

      case to_underlying(ePacketType::PAKWC_FAIRY): {
        Recv_gsv_GODDNESS_MODE(packet.get());
        break;
      }

      default: Proc_ZonePacket(std::move(packet));
    }
  }
}

void CNetwork::Proc_ZonePacket(std::unique_ptr<t_PACKET> packet) {
  switch ( packet->m_HEADER.m_wType ) {
  case to_underlying(ePacketType::PAKCC_SWITCH_SERVER): {
      this->m_bWarping = true;
      bAllInONE        = false;
      Recv_wsv_MOVE_SERVER(packet.get());
      MoveZoneServer( true );
      this->m_bWarping = false;
      break;
    }

  case to_underlying(ePacketType::PAKWC_RIDE_REQUEST): Recv_gsv_CART_RIDE(packet.get());
      break;

  case to_underlying(ePacketType::PAKWC_RIDE_STATE_CHANGE): Recv_gsv_PATSTATE_CHANGE(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_NPC_SHOW): Recv_gsv_SET_NPC_SHOW(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_BANK_MOVE_MONEY): Recv_gsv_MOVE_ZULY(packet.get());
      break;
    case to_underlying(ePacketType::PAKCC_CLAN_ICON_TIMESTAMP): Recv_wsv_CLANMARK_REG_TIME(packet.get());
      break;
      case to_underlying(ePacketType::PAKWC_BILLING_MESSAGE): Recv_gsv_BILLING_MESSAGE(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_CRAFT_STATUS): Recv_gsv_ITEM_RESULT_REPORT(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_LOGOUT_REPLY): Recv_gsv_LOGOUT_REPLY(packet.get());
      break;

    case to_underlying(ePacketType::PAKCC_CLAN_COMMAND): Recv_wsv_CLAN_COMMAND(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_CLAN_CHAT): Recv_wsv_CLAN_CHAT(packet.get());
      break;
    case to_underlying(ePacketType::PAKCC_CLAN_ICON_REPLY): Recv_wsv_CLANMARK_REPLY(packet.get());
      break;

    case to_underlying(ePacketType::PAKSS_ERROR): Recv_srv_ERROR(packet.get());
      break;

    //case GSV_RELAY_REQ: this->Send_PACKET( packet );
    //  break;

    case to_underlying(ePacketType::PAKWC_GLOBAL_VARS): Recv_gsv_SET_GLOBAL_VAR(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_GLOBAL_FLAGS): Recv_gsv_SET_GLOVAL_FLAG(packet.get());
      break;

    case to_underlying(ePacketType::PAKSS_ANNOUNCE_TEXT): Recv_srv_ANNOUNCE_TEXT(packet.get());
      break;
    case to_underlying(ePacketType::PAKSW_ANNOUNCE_CHAT): Recv_gsv_ANNOUNCE_CHAT(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_GM_COMMAND): Recv_gsv_GM_COMMAND(packet.get());
      break;

      // 캐릭터 선택 결과 통보받음
    case to_underlying(ePacketType::PAKWC_SELECT_CHAR_REPLY): Recv_gsv_SELECT_CHAR(packet.get());

      // Send_cli_JOIN_ZONE ();		// to_underlying(ePacketType::PAKWC_SELECT_CHAR_REPLY)
      break;

    case to_underlying(ePacketType::PAKWC_CHANGE_MAP_REPLY): Recv_gsv_JOIN_ZONE(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_REVIVE_REPLY): Recv_gsv_REVIVE_REPLY(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_SET_SERVER_VAR_REPLY): Recv_gsv_SET_VAR_REPLY(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_TELEPORT_REPLY): this->m_bWarping = false;
      Recv_gsv_TELEPORT_REPLY(packet.get());
      //				Send_cli_JOIN_ZONE ();		//
      // to_underlying(ePacketType::PAKWC_TELEPORT_REPLY)
      break;

    case to_underlying(ePacketType::PAKWC_INVENTORY_DATA): Recv_gsv_INVENTORY_DATA(packet.get());
      //				g_pCApp->SetStatus( AS_MAIN_GAME );
      break;
    case to_underlying(ePacketType::PAKWC_QUEST_DATA): Recv_gsv_QUEST_DATA(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_INIT_DATA): Recv_gsv_INIT_DATA(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_GM_COMMAND_CODE): Recv_gsv_CHEAT_CODE(packet.get());
      break;

    case to_underlying(ePacketType::PACWC_SET_ANIMATION):
    {
      using RoseCommon::Packet::SrvSetAnimation;
      Recv_gsv_SET_MOTION(SrvSetAnimation::create(reinterpret_cast<const uint8_t*>(packet.get())));
      break;
    }

    case to_underlying(ePacketType::PAKWC_TOGGLE_MOVE): Recv_gsv_TOGGLE(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_PARTY_CHAT): Recv_gsv_PARTY_CHAT(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_NORMAL_CHAT): Recv_gsv_CHAT(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_WHISPER_CHAT): Recv_gsv_WHISPER(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_SHOUT_CHAT): Recv_gsv_SHOUT(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_NPC_CHAR): Recv_gsv_NPC_CHAR(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_MOB_CHAR): Recv_gsv_MOB_CHAR(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_PLAYER_CHAR): Recv_gsv_AVT_CHAR(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_REMOVE_OBJECT): Recv_gsv_SUB_OBJECT(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_STOP): Recv_gsv_STOP(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_MOVE): Recv_gsv_MOVE(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_ATTACK): Recv_gsv_ATTACK(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_UPDATE_NPC): Recv_gsv_CHANGE_NPC(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_DAMAGE): Recv_gsv_DAMAGE(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_SET_EXP): Recv_gsv_SETEXP(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_LEVELUP): Recv_gsv_LEVELUP(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_HP_REPLY): Recv_gsv_HP_REPLY(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_CHANGE_SKIN): Recv_gsv_CHANGE_SKIN(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_EQUIP_ITEM): Recv_gsv_EQUIP_ITEM(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_DROP_ITEM): Recv_gsv_ADD_FIELDITEM(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_PICKUP_ITEM_REPLY): Recv_gsv_GET_FIELDITEM_REPLY(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_MOUSE_CMD): Recv_gsv_MOUSECMD(packet.get());
      break;

    //case GSV_SET_WEIGHT_RATE: Recv_gsv_SET_WEIGHT_RATE(packet.get());
    //  break;

    case to_underlying(ePacketType::PAKWC_SET_POSITION): Recv_gsv_ADJUST_POS(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_STOP_MOVING): Recv_gsv_STOP(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_SKILL_LEARN): Recv_gsv_SKILL_LEARN_REPLY(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_SKILL_LEVEL_REPLY): Recv_gsv_SKILL_LEVELUP_REPLY(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_SKILL_CAST_SELF): Recv_gsv_SELF_SKILL(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_SKILL_CAST_TARGET): Recv_gsv_TARGET_SKILL(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_SKILL_CAST_POSITION): Recv_gsv_POSITION_SKILL(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_SKILL_EFFECT): Recv_gsv_EFFECT_OF_SKILL(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_SKILL_DAMAGE): Recv_gsv_DAMAGE_OF_SKILL(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_SKILL_FINISH): Recv_gsv_RESULT_OF_SKILL(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_SKILL_START): Recv_gsv_SKILL_START(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_SKILL_CANCEL): Recv_gsv_SKILL_CANCEL(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_CLEAR_STATUS): Recv_gsv_CLEAR_STATUS(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_SPEED_CHANGED): Recv_gsv_SPEED_CHANGED(packet.get());
      break;

      /// 아이템을 사용했다.
    case to_underlying(ePacketType::PAKWC_USE_ITEM): Recv_gsv_USE_ITEM(packet.get());
      break;

    //case GSV_P_STORE_MONEYnINV: Recv_gsv_P_STORE_MONEYnINV(packet.get());
    //  break;
    case to_underlying(ePacketType::PAKWC_SET_MONEY_AND_ITEM): Recv_gsv_SET_MONEYnINV(RoseCommon::Packet::SrvSetMoneyAndItem::create(reinterpret_cast<const uint8_t*>(packet.get())));
      break;

    case to_underlying(ePacketType::PAKWC_QUEST_REWARD_ITEM): {
      Recv_gsv_REWARD_ITEM(packet.get());
      // Missing break is intentional.
    }

    [[fallthrough]];
    case to_underlying(ePacketType::PAKWC_SET_ITEM): Recv_gsv_SET_INV_ONLY(RoseCommon::Packet::SrvSetItem::create(reinterpret_cast<const uint8_t*>(packet.get())));
      break;

    case to_underlying(ePacketType::PAKWC_HOTBAR_SET_ICON_REPLY): Recv_gsv_SET_HOTICON(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_STAT_ADD_REPLY): Recv_gsv_USE_BPOINT_REPLY(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_QUEST_DATA_REPLY): Recv_gsv_QUEST_REPLY(packet.get());
      break;
      /*
      case to_underlying(ePacketType::PAKWC_QUEST_DATA)_REPLY :
        Recv_gsv_QUEST_DATA_REPLY ();
        break;
      */
    case to_underlying(ePacketType::PAKWC_TRADE): Recv_gsv_TRADE_P2P(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_TRADE_ITEM): Recv_gsv_TRADE_P2P_ITEM(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_PARTY_REQ): Recv_gsv_PARTY_REQ(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_PARTY_REPLY): Recv_gsv_PARTY_REPLY(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_PARTY_MEMBER): Recv_gsv_PARTY_MEMBER(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_PARTY_LEVELEXP): Recv_gsv_PARTY_LEVnEXP(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_PARTY_ITEM): Recv_gsv_PARTY_ITEM(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_PARTY_RULE): Recv_gsv_PARTY_RULE(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_PARTY_MEMBER_UPDATE): Recv_gsv_CHANGE_OBJIDX(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_STORE_TRADE_REPLY): Recv_gsv_STORE_TRADE_REPLY(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_CRAFT_REPLY): Recv_gsv_CREATE_ITEM_REPLY(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_BANK_LIST_REPLY): Recv_gsv_BANK_LIST_REPLY(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_BANK_MOVE_ITEM): Recv_gsv_MOVE_ITEM(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_EQUIP_PROJECTILE): Recv_gsv_SET_BULLET(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_SERVER_DATA): Recv_gsv_SERVER_DATA(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_EQUIP_ITEM_RIDE): Recv_gsv_ASSEMBLE_RIDE_ITEM(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_EVENT_STATUS): Recv_GSV_SET_EVENT_STATUS(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_SET_ITEM_LIFE): Recv_GSV_SET_ITEM_LIFE(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_SHOP_OPEN): Recv_gsv_P_STORE_OPENED(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_SHOP_CLOSE): Recv_gsv_P_STORE_CLOSED(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_SHOP_LIST_REPLY): Recv_gsv_P_STORE_LIST_REPLY(packet.get());
      break;
    case to_underlying(ePacketType::PAKCS_SHOP_BUYSELL_REPLY): Recv_gsv_P_STORE_RESULT(packet.get());
      break;

    case to_underlying(ePacketType::PAKCS_REPAIR_USE_ITEM): Recv_gsv_USED_ITEM_TO_REPAIR(packet.get());
      break;
    case to_underlying(ePacketType::PAKCS_REPAIR_NPC): Recv_gsv_REPAIRED_FROM_NPC(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_SET_MONEY): Recv_gsv_SET_MONEY_ONLY(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_QUEST_REWARD_MONEY): Recv_gsv_REWARD_MONEY(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_QUEST_REWARD_ADD_VALUE): {
      Recv_gsv_REWARD_ADD_ABILITY(packet.get());
      break;
    }
    case to_underlying(ePacketType::PAKWC_QUEST_REWARD_SET_VALUE): {
      Recv_gsv_REWARD_SET_ABILITY(packet.get());
      break;
    }

    case to_underlying(ePacketType::PAKWC_FAIRY): {
      Recv_gsv_GODDNESS_MODE(packet.get());
      break;
    }

      //----------------------------------------------------------------------------------------------------
      /// @brief 아이템 재밍관련
      //----------------------------------------------------------------------------------------------------
    case to_underlying(ePacketType::PAKWC_CRAFT_ENHANCE_REPLY): Recv_gsv_CRAFT_ITEM_REPLY(packet.get());
      break;

      //----------------------------------------------------------------------------------------------------
      /// @brief 이벤트 오브젝트 관련
      //----------------------------------------------------------------------------------------------------
    case to_underlying(ePacketType::PAKWC_EVENT_ADD): Recv_gsv_ADD_EVENTOBJ(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_APPRAISAL_REPLY): Recv_gsv_APPRAISAL_REPLY(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_SET_HP_AND_MP): Recv_gsv_SET_HPnMP(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_NPC_EVENT): Recv_gsv_CHECK_NPC_EVENT(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_ALLIED_CHAT): Recv_gsv_ALLIED_CHAT(packet.get());
      break;
    case to_underlying(ePacketType::PAKWC_ALLIED_SHOUT_CHAT): Recv_gsv_ALLIED_SHOUT(packet.get());
      break;

    case to_underlying(ePacketType::PAWKC_CHAR_STATE_CHANGE): Recv_gsv_CHARSTATE_CHANGE(packet.get());
      break;

    case to_underlying(ePacketType::PAKSC_SCREEN_SHOT_TIME_REPLY): Recv_gsv_SCREEN_SHOT_TIME(packet.get());
      break;

    case to_underlying(ePacketType::PAKWC_UPDATE_NAME): Recv_gsv_UPDATE_NAME(packet.get());
      break;

    case to_underlying(ePacketType::PAKCC_CHAN_CHAR_REPLY): { //Fixed by Davidixx
        using RoseCommon::Packet::SrvChanCharReply;
        Recv_wsv_CHAR_CHANGE(SrvChanCharReply::create(reinterpret_cast<const uint8_t*>(packet.get())));
        m_btZoneSocketSTATUS = 0;
        CshoClientSOCK* pSocket = &this->m_ZoneSOCKET;
        pSocket->Close();
        break;
    }

    default:
      //_ASSERT(0);
      LogString(LOG_NORMAL,
                "received Invalid packet type ... type: 0x%x , size: %d \n",
                packet->m_HEADER.m_wType, packet->m_HEADER.m_nSize);
  }
}

//-------------------------------------------------------------------------------------------------
void CNetwork::Proc() {
  this->Proc_WorldPacket();

  CshoClientSOCK* pSocket = &this->m_ZoneSOCKET;
  std::unique_ptr<t_PACKET> packet(new t_PACKET);
  while ( m_ZoneSOCKET.Peek_Packet( packet.get(), true ) ) {
    LogString(LOG_DEBUG, "Packet_Proc:: Type: 0x%x, Size: %d \n",
              packet->m_HEADER.m_wType, packet->m_HEADER.m_nSize);
    switch ( packet->m_HEADER.m_wType ) {
      case to_underlying(ePacketType::PAKSS_ACCEPT_REPLY): {
        m_btZoneSocketSTATUS = packet->m_NetSTATUS.m_btStatus;
        switch ( packet->m_NetSTATUS.m_btStatus ) {
          case NETWORK_STATUS_ACCEPTED: {
            pSocket->OnAccepted( (int *)packet->m_NetSTATUS.m_dwSocketIDs );
            _ASSERT(0);
            continue;
          }
          case NETWORK_STATUS_CONNECT: {
            // 존 서버와 연결됐다...
            // 케릭터 선택후 실제 존에 들어간다.
            m_bWarping = false;
            pSocket->mF_Init( m_dwGSV_IDs[1] );
            this->Send_cli_JOIN_SERVER_REQ( m_dwGSV_IDs[0] );
            continue;
          }
          case NETWORK_STATUS_DISCONNECT: {
            // 서버를 옮기기 위해 접속을 끊은것인가 ? 끊긴것인가 ?
            if ( m_bMoveingZONE ) {
              LogString(LOG_DEBUG,
                        "m_bMoveingZONE set to true, reconnecting.... \n");
              this->MoveZoneServer();
              continue;
            }
            CGame::GetInstance().ProcWndMsg( WM_USER_SERVER_DISCONNECTED, 0, 0 );
            // SendServerDisconnectMsg( "ZoneServer" );
            break;
          }
          case NETWORK_STATUS_DERVERDEAD: {
            break;
          }
        }

        LogString(LOG_NORMAL, "존 서버와의 접속에 실패했습니다.\n");
        break;
      }

      case to_underlying(ePacketType::PAKSC_JOIN_SERVER_REPLY): // 존 서버와의 접속에 실패했습니다.
      {
        DWORD dwRet = Recv_srv_JOIN_SERVER_REPLY(packet.get());
        if ( dwRet ) {
          pSocket->OnAccepted( (int *)&dwRet );
          // 처음 접속한 존 서버? 면...
          CLiveCheck::GetSingleton().ResetTime();
        } else {
          // TODO:: error
          this->DisconnectFromServer();
          return;
        }
        break;
      }

      default: this->Proc_ZonePacket(std::move(packet));
    }
  }
}

//-------------------------------------------------------------------------------------------------
// 2005. 5. 23 박 지호
void CNetwork::Send_AuthMsg(void) {

  //m_pSendPacket->m_HEADER.m_wType = CLI_CHECK_AUTH;
  //m_pSendPacket->m_HEADER.m_nSize = sizeof( cli_CHECK_AUTH );
  // Packet_AppendData( m_pSendPacket, &m_nProtectSys.GetAuthData(),sizeof(
  // GG_AUTH_DATA ) );
  //Send_PACKET( m_pSendPacket );
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
