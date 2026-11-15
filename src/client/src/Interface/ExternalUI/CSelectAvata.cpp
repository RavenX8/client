#include "StdAfx.h"
#include "CSelectAvata.h"
#include "ExternalUILobby.h"

#include "../Command/ExUICommand.h"

#include "../../Network/CNetwork.h"
#include "../../Network/GeneratedItems.h"

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
/// 캐릭터가 선택되고 서버에 알리기 전에 Msg Queue를 모두 지워서 두번 보내어지는것을 막는다. 2004 /6 /20
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

void             CSelectAvata::RecvAvataList(const RoseCommon::Packet::SrvCharListReply& packet) {
  CJustModelAVT* pAvt = nullptr;

  CGameDataCreateAvatar::GetInstance().Clear();

  for ( const auto& info : packet.get_characters() ) {
    // The equipped items in packet order: helmet, armor, gauntlet, boots,
    // goggles, backpack, right and left weapon (BODY_PART_HELMET onwards).
    const auto& items = info.get_items();
    auto        item  = [&items](int bodyPart) { return items[bodyPart - BODY_PART_HELMET].get_id(); };

    pAvt = new CJustModelAVT();

    pAvt->Init( info.get_name().c_str(),
                GeneratedItems::equipped( info.get_face() ).id,
                GeneratedItems::equipped( info.get_hair() ).id,
                item( BODY_PART_HELMET ),
                item( BODY_PART_ARMOR ),
                item( BODY_PART_GAUNTLET ),
                item( BODY_PART_BOOTS ),
                item( BODY_PART_FACE_ITEM ),
                item( BODY_PART_KNAPSACK ),
                0/*item( BODY_PART_WEAPON_R )*/,
                0 /*item( BODY_PART_WEAPON_L )*/
    );

    pAvt->SetLevel( info.get_level() );
    pAvt->SetJob( info.get_job() );
    pAvt->SetDeleteRemainSEC( info.get_remainSecsUntilDelete() );
    pAvt->SetPlatinum( (info.get_platinium() == 0) ? false : true );
    pAvt->SetRace( info.get_race() );

    pAvt->UpdateModel();
    pAvt->LoadModelVisible();
    pAvt->InsertToScene();

    CGameDataCreateAvatar::GetInstance().AddCreateAvatar( pAvt );

    HNODE hModel = pAvt->GetModelNode();
    setScale( hModel, 1.5, 1.5, 1.5 );
    setModelDirection( hModel, 180.f, false );

    if ( info.get_remainSecsUntilDelete() )
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

/// 현재 게임 해상도에 따른 버튼 위치 재조정 xml에 있는 x,y좌표는 무시된다.
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
