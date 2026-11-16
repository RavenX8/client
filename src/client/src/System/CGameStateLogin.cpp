#include "StdAfx.h"
#include "CGameStateLogin.h"
#include "CGame.h"

#include "../Game.h"
#include "tgamectrl/TMsgBox.h"
#include "../Network/CNetwork.h"

#include "../Interface/ExternalUI/ExternalUILobby.h"
#include "../Interface/CTFontImpl.h"
#include "../Interface/IO_ImageRes.h"
#include "../Interface/CUIMediator.h"
#include "../Interface/it_mgr.h"
#include "../Interface/Command/CTCmdHotExec.h"
#include "../Interface/cursor/CCursor.h"

#include "../Common/IO_Quest.h"

#include "../IO_Terrain.h"
#include "../CCamera.h"
#include "../CSkyDOME.h"
//#include "../nProtect/nProtect.h"
#include "../Misc/GameUtil.h"
#include "tgamectrl/TTIme.h"
#include "SystemProcScript.h"

CGameStateLogin::CGameStateLogin(int iID) {
  m_iStateID = iID;
}

CGameStateLogin::~CGameStateLogin(void) { }

int CGameStateLogin::Update(bool bLostFocus) {
  // A token login that failed closes the game after a while (ExitAfter),
  // counting down in its message box; OK closes it at once.
  if ( g_GameDATA.m_dwExitAt ) {
    DWORD now = g_GameDATA.GetGameTime();
    if ( now >= g_GameDATA.m_dwExitAt ) {
      g_GameDATA.m_dwExitAt = 0;
      g_pCApp->SetExitGame();
    } else {
      static int shown = -1;
      int left = (int)((g_GameDATA.m_dwExitAt - now + 999) / 1000);
      if ( left != shown ) {
        shown = left;
        g_EUILobby.SetMsgBoxText( CStr::Printf( "%s (closing in %d s)", g_GameDATA.m_ExitText.c_str(), left ) );
      }
    }
  }
  g_EUILobby.Update();

  g_pCamera->Update();

  D3DVECTOR PosENZIN = g_pCamera->Get_Position();
  g_pTerrain->SetCenterPosition( PosENZIN.x, PosENZIN.y );

  g_DayNNightProc.ShowAllEffect( false );

  updateScene();

  // processing  ...
  if ( !bLostFocus ) {
    if ( !beginScene() ) //  ����̽��� �սǵ� ���¶�� 0�� �����ϹǷ�, ��� ������ ��ŵ
    {
      return 0;
    }

    clearScreen();
    renderScene();

    beginSprite( D3DXSPRITE_ALPHABLEND );

    /// Screen message display
    g_UIMed.Draw();

    /// UI display
    g_EUILobby.Draw();

    CTIme::GetInstance().Draw();

    endSprite();

    endScene();
    swapBuffers();
  }
  return 0;
}

int CGameStateLogin::ProcMouseInput(UINT uiMsg, WPARAM wParam, LPARAM lParam) {
  if ( g_EUILobby.MsgProc( uiMsg, wParam, lParam ) )
    return 1;

  return 0;
}

int CGameStateLogin::ProcKeyboardInput(UINT uiMsg, WPARAM wParam, LPARAM lParam) {
  switch ( uiMsg ) {
    case WM_CHAR: {
      switch ( wParam ) {
        case '1': {
          //g_pCamera->SetMotion("Cameras\\camera01.zmo");
          //g_pCamera->Set_Position ( 520000.0f, 520000.0f, 0.0f );
        }
        break;
        case '2': break;
      }
    }
    break;
  }

  if ( g_EUILobby.MsgProc( uiMsg, wParam, lParam ) )
    return 1;

  return 0;
}

int CGameStateLogin::Enter(int iPrevStateID) {
  CSystemProcScript::GetSingleton().CallLuaFunction( "EnterLogin", ZZ_PARAM_INT, (int)g_GameDATA.GetTime(), ZZ_PARAM_END );

  g_EUILobby.InitEUIManager();
  g_EUILobby.CreateLoginDlg();

  //	CTCommand* pCmd = new CTCmdExit;
  //	g_pTerrain->SetLoadingMode( false );
  //	::setDelayedLoad( 2 );	

  int iOldSightRange       = g_GameDATA.m_nSightRange;
  g_GameDATA.m_nSightRange = 50;

  g_pCamera->Update();
  D3DVECTOR PosENZIN = g_pCamera->Get_Position();
  g_pTerrain->InitZONE( PosENZIN.x, PosENZIN.y );
  g_pTerrain->SetCenterPosition( PosENZIN.x, PosENZIN.y );

  /// Init skybox
  CSkyDOME::Init( g_GameDATA.m_hShader_sky, g_GameDATA.m_hLight, 0 );

  //::setDelayedLoad( 1 );

  g_GameDATA.m_nSightRange = iOldSightRange;

  if ( g_GameDATA.m_is_NHN_JAPAN ) {
    CTDialog* pDlg      = g_EUILobby.GetEUI( EUI_LOGIN );
    CLogin*   pLoginDlg = (CLogin*)pDlg;
    pLoginDlg->SetID( g_GameDATA.m_Account.Get() );
    ///Decoding�� �ִ´�.
    //pLoginDlg->SetPassword( g_GameDATA.m_Password.Get() );
    pLoginDlg->ConnectLoginServer();
    pLoginDlg->Hide();
  }

  if (g_GameDATA.m_bDirectLogin)
  {
    CTDialog* pDlg      = g_EUILobby.GetEUI( EUI_LOGIN );
    CLogin*   pLoginDlg = (CLogin*)pDlg;
    pLoginDlg->SetID( g_GameDATA.m_Account.Get() );
    pLoginDlg->ConnectLoginServer();
    pLoginDlg->Hide();
  }
  /// exec Enter script

  return 0;
}

int CGameStateLogin::Leave(int iNextStateID) {
  /// exec Leave script
  CSystemProcScript::GetSingleton().CallLuaFunction( "LeaveLogin", ZZ_PARAM_END );
  //m_npKeyCrypt.CloseHandle();
  return 0;
}

void CGameStateLogin::ServerDisconnected() {
  //g_EUILobby.ServerDisconnected();
  // With a token there is no login form to go back to: say so, OK exits, and
  // the game closes by itself after 30 seconds (unless a refusal said it first).
  if ( g_GameDATA.m_bDirectLogin && !g_GameDATA.m_dwExitAt ) {
    g_EUILobby.ShowMsgBox( STR_LOGIN_FAIL, CTMsgBox::BT_OK, true, EUI_LOGIN, new CTCmdExit );
    g_GameDATA.ExitAfter( 30 * 1000, STR_LOGIN_FAIL );
  }
}

void CGameStateLogin::AcceptedConnectLoginSvr() {
  CGame::GetInstance().ProcWndMsg( WM_USER_CLOSE_MSGBOX, 0, 0 );
  ///���ӵǾ��� ID�� �н����带 ������.

  CTDialog* pDlg = g_EUILobby.GetEUI( EUI_LOGIN );
  assert( pDlg );
  if ( pDlg ) {
    CLogin* pLoginDlg = (CLogin*)pDlg;
    pLoginDlg->SendLoginReq();
  }
}
