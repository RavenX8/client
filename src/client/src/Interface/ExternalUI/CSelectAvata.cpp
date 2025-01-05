#include "StdAfx.h"
#include "CSelectAvata.h"
#include "ExternalUILobby.h"

#include "../Command/ExUICommand.h"

#include "../../Network/CNetwork.h"

#include "../../Game.h"

#include "../../System/CGame.h"
#include "../../System/SystemProcScript.h"
#include "../../GameData/CGameDataCreateAvatar.h"

#include "../CJustModelAVT.h"
#include "../Country.h"

#include "tgamectrl/TMsgBox.h"

CSelectAvata::CSelectAvata() {}

CSelectAvata::~CSelectAvata() {}

void CSelectAvata::SetEUIObserver(CExternalUIManager* pObserver) {}

UINT CSelectAvata::Process(UINT uiMsg, WPARAM wParam, LPARAM lParam) {
  if ( !IsVision() ) return 0;

  switch ( uiMsg ) {
    case WM_KEYDOWN: {
      switch ( wParam ) {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5': {
          CGameDataCreateAvatar::GetInstance().SelectAvatar( wParam - '1' );
          break;
        }
        case VK_RETURN: {
          SendSelectAvataReq();
          break;
        }
        default: break;
      }
    }
    default: break;
  }

  unsigned iProcID;
  if ( iProcID = CTDialog::Process( uiMsg, wParam, lParam ) ) {
    switch ( uiMsg ) {
      case WM_LBUTTONUP: OnLButtonUp( iProcID );
        return uiMsg;
      default: break;
    }
    return uiMsg;
  }
  return 0;
}

///
///	Send select Avata request
/// ĳ���Ͱ� ���õǰ� ������ �˸��� ���� Msg Queue�� ��� ������ �ι� ���������°��� ���´�. 2004 /6 /20
void                     CSelectAvata::SendSelectAvataReq() {
  CGameDataCreateAvatar& refData = CGameDataCreateAvatar::GetInstance();

  if ( CJustModelAVT* pSelectedAvt = refData.GetSelectedAvatar() ) {
    if ( pSelectedAvt->IsPlatinum()
         && CCountry::GetSingleton().IsJapan()
         && !(CGame::GetInstance().GetPayType() & PLAY_FLAG_EXTRA_CHAR) ) {
      g_EUILobby.ShowMsgBox( STR_JP_BILL_CANT_SELECT_CHARACTER, CTMsgBox::BT_OK, true, GetDialogType() );
      return;
    }
    if ( pSelectedAvt->IsPlatinum()
         && !CCountry::GetSingleton().IsJapan()
         && !CCountry::GetSingleton().IsPhilipplines()
         && CGame::GetInstance().GetPayType() != CGame::PAY_PLATINUM ) {
      g_EUILobby.ShowMsgBox( STR_ONLY_PLATINUM_SELECT_CHAR, CTMsgBox::BT_OK, true, GetDialogType() );
    } else if ( !m_bWaitServerReply ) {
      if ( pSelectedAvt->GetDeleteRemainSEC() ) {
        CTCommand* pCmd = new CTCmdReviveCharacter( refData.GetSelectedAvatarIndex(), refData.GetSelectedAvatarName() );
        g_EUILobby.ShowMsgBox( STR_QUERY_REVIVE_CHARACTER,
                               CTMsgBox::BT_OK | CTMsgBox::BT_CANCEL, true, GetDialogType(), pCmd );
      } else {
        CGame::GetInstance().ClearWndMsgQ();
        g_EUILobby.ShowWaitMsgBox();
        g_pNet->Send_cli_SELECT_CHAR( refData.GetSelectedAvatarIndex(), (char*)refData.GetSelectedAvatarName() );
        LogString(LOG_NORMAL, "Send_cli_SELECT_CHAR\n");
        m_bWaitServerReply = true;
      }
    }
  } else
    g_EUILobby.ShowMsgBox( STR_REQUEST_SELECT_CHARACTER, CTMsgBox::BT_OK | CTMsgBox::BT_CANCEL, true, GetDialogType() );
}

void             CSelectAvata::RecvAvataList(t_PACKET* recvPacket) {
  uint64_t          nI            = 0;
  char*          szServerName  = nullptr;
  char*          pszCharName   = nullptr;
  tagCHARINFO*   pCharINFO     = nullptr;
  tagPartITEM*   pnPartItemIDX = nullptr;
  CJustModelAVT* pAvt          = nullptr;

  CGameDataCreateAvatar::GetInstance().Clear();
  short nOffset = sizeof( wsv_CHAR_LIST );

  for ( nI        = 0; nI < recvPacket->m_wsv_CHAR_LIST.m_btCharCNT; nI++ ) {
    pszCharName   = Packet_GetStringPtr( recvPacket, nOffset );
    pCharINFO     = (tagCHARINFO*)Packet_GetDataPtr( recvPacket, nOffset, sizeof( tagCHARINFO ) );
    pnPartItemIDX = (tagPartITEM*)Packet_GetDataPtr( recvPacket, nOffset, sizeof( tagPartITEM ) * MAX_BODY_PART );

    pAvt = new CJustModelAVT();

    pAvt->Init( pszCharName,
                pnPartItemIDX[BODY_PART_FACE].m_nItemNo,
                pnPartItemIDX[BODY_PART_HAIR].m_nItemNo,
                pnPartItemIDX[BODY_PART_HELMET].m_nItemNo,
                pnPartItemIDX[BODY_PART_ARMOR].m_nItemNo,
                pnPartItemIDX[BODY_PART_GAUNTLET].m_nItemNo,
                pnPartItemIDX[BODY_PART_BOOTS].m_nItemNo,
                pnPartItemIDX[BODY_PART_FACE_ITEM].m_nItemNo,
                pnPartItemIDX[BODY_PART_KNAPSACK].m_nItemNo,
                0/*pnPartItemIDX[BODY_PART_WEAPON_R].m_nItemNo*/,
                0 /*pnPartItemIDX[BODY_PART_WEAPON_L].m_nItemNo*/
    );

    pAvt->SetLevel( pCharINFO->m_nLEVEL );
    pAvt->SetJob( pCharINFO->m_nJOB );
    pAvt->SetDeleteRemainSEC( pCharINFO->m_dwRemainSEC );
    pAvt->SetPlatinum( (pCharINFO->m_btIsPlatinumCHAR == 0) ? false : true );
    pAvt->SetRace( pCharINFO->m_btCharRACE );

    pAvt->UpdateModel();
    pAvt->LoadModelVisible();
    pAvt->InsertToScene();

    CGameDataCreateAvatar::GetInstance().AddCreateAvatar( pAvt );

    HNODE hModel = pAvt->GetModelNode();
    setScale( hModel, 1.5, 1.5, 1.5 );
    setModelDirection( hModel, 180.f, false );

    if ( pCharINFO->m_dwRemainSEC )
      pAvt->SetMotion( 6 );

  }
}

void             CSelectAvata::RecvAvataList(RoseCommon::Packet::SrvCharListReply& packet) {
  uint64_t          nI            = 0;
  std::string pszCharName = nullptr;
  CJustModelAVT* pAvt          = nullptr;

  CGameDataCreateAvatar::GetInstance().Clear();

  auto characters = packet.get_characters();

  for ( nI        = 0; nI < characters.size(); nI++ ) {
    pszCharName   = characters[nI].get_name();
    auto items = characters[nI].get_items();

    pAvt = new CJustModelAVT();

    pAvt->Init( pszCharName.c_str(),
                items[BODY_PART_FACE].get_id(),
                items[BODY_PART_HAIR].get_id(),
                items[BODY_PART_HELMET].get_id(),
                items[BODY_PART_ARMOR].get_id(),
                items[BODY_PART_GAUNTLET].get_id(),
                items[BODY_PART_BOOTS].get_id(),
                items[BODY_PART_FACE_ITEM].get_id(),
                items[BODY_PART_KNAPSACK].get_id(),
                0/*items[BODY_PART_WEAPON_R].get_id()*/,
                0 /*items[BODY_PART_WEAPON_L].get_id()*/
    );

    pAvt->SetLevel( characters[nI].get_level() );
    pAvt->SetJob( characters[nI].get_job() );
    pAvt->SetDeleteRemainSEC( characters[nI].get_remainSecsUntilDelete() );
    pAvt->SetPlatinum( (characters[nI].get_platinium() != 0));
    pAvt->SetRace( characters[nI].get_race() );

    pAvt->UpdateModel();
    pAvt->LoadModelVisible();
    pAvt->InsertToScene();

    CGameDataCreateAvatar::GetInstance().AddCreateAvatar( pAvt );

    HNODE hModel = pAvt->GetModelNode();
    setScale( hModel, 1.5, 1.5, 1.5 );
    setModelDirection( hModel, 180.f, false );

    if ( characters[nI].get_remainSecsUntilDelete() )
      pAvt->SetMotion( 6 );

  }
}

void CSelectAvata::OnLButtonUp(unsigned iProcID) {
  switch ( iProcID ) {
    case IID_BTN_CREATE: CGame::GetInstance().ChangeState( CGame::GS_CREATEAVATAR );
      break;
    case IID_BTN_DELETE: {
      CGameDataCreateAvatar& refData      = CGameDataCreateAvatar::GetInstance();
      if ( CJustModelAVT*    pSelectedAvt = refData.GetSelectedAvatar() ) {
        if ( pSelectedAvt->GetDeleteRemainSEC() ) {
          CTCommand* pCmd = new CTCmdReviveCharacter( refData.GetSelectedAvatarIndex(), refData.GetSelectedAvatarName() );
          g_EUILobby.ShowMsgBox( STR_QUERY_REVIVE_CHARACTER,
                                 CTMsgBox::BT_OK | CTMsgBox::BT_CANCEL, true, GetDialogType(), pCmd );
        } else {
          CTCommand* pCmd = new CTCmdDeleteCharacter( refData.GetSelectedAvatarIndex(), refData.GetSelectedAvatarName() );
          g_EUILobby.ShowMsgBox( CStr::Printf( "%s%s", refData.GetSelectedAvatarName(),STR_QUERY_DELETE_CHARACTER ),
                                 CTMsgBox::BT_OK | CTMsgBox::BT_CANCEL, true, GetDialogType(), pCmd );
        }
      } else {
        g_EUILobby.ShowMsgBox( STR_REQUEST_SELECT_CHARACTER, CTMsgBox::BT_OK | CTMsgBox::BT_CANCEL, true, GetDialogType() );
      }
    }
    break;
    case IID_BTN_OK: {
      SendSelectAvataReq();
    }
    break;
    case IID_BTN_CANCEL: CGame::GetInstance().ChangeState( CGame::GS_LOGINVIRTUAL );
      break;
    default: break;
  }

}

/// ���� ���� �ػ󵵿� ���� ��ư ��ġ ������ xml�� �ִ� x,y��ǥ�� ���õȴ�.
void CSelectAvata::Show() {
  CTDialog::Show();
  m_bWaitServerReply = false;

  POINT pt;
  int   iDialogHeight;

  CWinCtrl* pComponent = nullptr;
  if ( pComponent      = Find( IID_BTN_CANCEL ) ) {
    pt.x               = g_pCApp->GetWIDTH() / 5 - pComponent->GetWidth() / 2;
    pt.y               = g_pCApp->GetHEIGHT() - 24 - pComponent->GetHeight();
    pComponent->MoveWindow( pt );
  }

  if ( pComponent = Find( IID_BTN_CREATE ) ) {
    pt.x          = g_pCApp->GetWIDTH() * 2 / 5 - pComponent->GetWidth() / 2;
    pt.y          = g_pCApp->GetHEIGHT() - 24 - pComponent->GetHeight();
    pComponent->MoveWindow( pt );
  }

  if ( pComponent = Find( IID_BTN_DELETE ) ) {
    pt.x          = g_pCApp->GetWIDTH() * 3 / 5 - pComponent->GetWidth() / 2;
    pt.y          = g_pCApp->GetHEIGHT() - 24 - pComponent->GetHeight();
    pComponent->MoveWindow( pt );
  }

  if ( pComponent = Find( IID_BTN_OK ) ) {
    pt.x          = g_pCApp->GetWIDTH() * 4 / 5 - pComponent->GetWidth() / 2;
    pt.y          = g_pCApp->GetHEIGHT() - 24 - pComponent->GetHeight();
    pComponent->MoveWindow( pt );
    iDialogHeight = pComponent->GetHeight();
  }

  SetWidth( g_pCApp->GetWIDTH() );
  SetHeight( iDialogHeight );
}
