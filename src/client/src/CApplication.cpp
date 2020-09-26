/*
  $Header: /Client/CApplication.cpp 82    05-10-13 11:32a Choo0219 $
*/

#include "StdAfx.h"
#include "resource.h"
#include "CApplication.h"
//#include "CMouse.h"
//#include "CKeyboard.h"
#include "Network/CNetwork.h"
#include "Game.h"
#include "tgamectrl/TGameCtrl.h"
#include "tgamectrl/TTIme.h"

#include "Interface/it_mgr.h"
#include "System/CGame.h"
#include "Interface/cursor/CCursor.h"
#include "Sound/MusicMgr.h"
#include "Sound/DirectMusicPlayer.h"
#include "Util/classMD5.h"
#include "packetfactory.h"


CApplication* CApplication::m_pInstance = nullptr;

//#define DEFAULT_WINDOWED_STYLE ( WS_OVERLAPPEDWINDOW )
//#define DEFAULT_WINDOWED_STYLE ( WS_OVERLAPPEDWINDOW | ~WS_MAXIMIZEBOX )

//---------------------------------------------------------------------------------------------------------
/// WS_OVERLAPPEDWINDOW - WS_MAXIMIZEBOX
//---------------------------------------------------------------------------------------------------------
#define DEFAULT_WINDOWED_STYLE ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX )

#define DEFAULT_FULLSCREEN_STYLE (/* WS_SYSMENU |*/ WS_VISIBLE | WS_POPUP /*| WS_MAXIMIZE */)

//-----------------------------------------------------------------------------
// Name: WndProc()
// Desc: Static msg handler which passes messages to the application class.
//-----------------------------------------------------------------------------
LRESULT CALLBACK Window_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  return g_pCApp->MessageProc( hWnd, uMsg, wParam, lParam );
}

//-------------------------------------------------------------------------------------------------
int   CApplication::wm_COMMAND(WPARAM wParam) {
  int wmId, wmEvent;

  wmId    = LOWORD(wParam);
  wmEvent = HIWORD(wParam);
  // Parse the menu selections:
  switch ( wmId ) {
    case IDM_WIRE:
      //--------------------------[ engine related ]-----------------------//
      m_bViewWireMode = !m_bViewWireMode;
      useWireMode( m_bViewWireMode );
      break;
    case IDM_EXIT: DestroyWindow( this->m_hWND );
      break;
  }
  return 1;
}

//-------------------------------------------------------------------------------------------------
LRESULT      CApplication::MessageProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
  static int nWidth  = m_nScrWidth;
  static int nHeight = m_nScrHeight;

  if ( CTIme::GetInstance().Process( hWnd, uiMsg, wParam, lParam ) )
    return S_OK;

  // 키占쏙옙占쏙옙 占쌨쇽옙占쏙옙 占쏙옙占쏙옙
  if ( CGame::GetInstance().AddWndMsgQ( uiMsg, wParam, lParam ) )
    return 0;

  switch ( uiMsg ) {
    case WM_SYSCHAR: ///systemkey占쏙옙 占싹뱄옙 키占쏙옙 占쏙옙占쏙옙占쌔쇽옙 占쏙옙占쏙옙占쏙옙 "占쏙옙"占쌀몌옙 占쏙옙占쌍깍옙
      return 0;
    case WM_SETCURSOR: if ( CCursor::GetInstance().RefreshCursor() )
        return S_OK;
      break;
    case WM_ACTIVATE: {
      m_wActive = ((LOWORD( wParam ) != WA_INACTIVE) && (HIWORD( wParam ) == 0)); // INVACIVE 占쏙옙 占싣니곤옙, 占싱니몌옙占쏙옙占쏘도 占싣댐옙 占쏙옙荑∽옙占� 활占쏙옙화
      //m_wActive = true;

      LogString(LOG_DEBUG, "WM_ACTIVATE: [%s]\n",
                (HIWORD(wParam) != 0) ? "[MINIMIZED]" : (LOWORD(wParam) == WA_ACTIVE) ? "ACTIVATE" : (LOWORD(wParam) == WA_CLICKACTIVE) ? "CLICKACTIVE" : "DEACTIVE");

      //-----------------------------------------------------------------------------------------
      ///
      //-----------------------------------------------------------------------------------------
      CMusicMgr& musicMgr = CMusicMgr::GetSingleton();
      if ( musicMgr.bIsReady() ) {
        if ( m_wActive ) {
          musicMgr.Run();
        } else {
          musicMgr.Pause();
        }

      }

    }

    break;

    case WM_MUSIC_EVENT: {
      //-----------------------------------------------------------------------------------------
      ///
      //-----------------------------------------------------------------------------------------
      CMusicMgr& musicMgr = CMusicMgr::GetSingleton();
      if ( musicMgr.bIsReady() ) {
        musicMgr.HandleEvent();
      }
    }

    break;

    case WM_CLOSE: g_pCApp->SetExitGame();
      //CGame::GetInstance().ProcWndMsg( uiMsg, wParam, lParam );
      return 0;

    case WM_ERASEBKGND:
    case WM_SYSKEYUP: // ALT키 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙 !!!
    case WM_PALETTECHANGED:
    case WM_QUERYNEWPALETTE: return 0;

    case WM_MOVE: if ( !IsFullScreenMode() ) {
        RECT sRECT;
        GetClientRect( hWnd, &sRECT );
        ClientToScreen( hWnd, (POINT*)&sRECT );
        ClientToScreen( hWnd, (POINT*)&sRECT + 1 );
      }
      break;
    case WM_COMMAND: this->wm_COMMAND( wParam );
      break;
    case WM_SIZE:
    {
      if (!IsFullScreenMode() &&  wParam == SIZE_RESTORED)
      {
        nWidth = LOWORD(lParam);
        nHeight = HIWORD(lParam);
        if (nWidth != m_nScrWidth || nHeight != m_nScrHeight)
        {
          UpdateWindowSize(nWidth, nHeight, m_iScrDepth, false);
          ResizeWindowByClientSize(nWidth, nHeight, m_iScrDepth, true);
          resetScreen();
          CCursor::GetInstance().ReloadCursor();
          SIZE sizeScreen = { nWidth, nHeight };
          CTIme::GetInstance().ChangeScreenSize(sizeScreen);
          g_itMGR.InitInterfacePos();
        }
        //return 0;
      }
    }
    break;
  }
  return ::DefWindowProc( hWnd, uiMsg, wParam, lParam );
}

//-------------------------------------------------------------------------------------------------
CApplication::CApplication() {
  RoseCommon::register_send_packets();
  RoseCommon::register_recv_packets();

  m_hWND = nullptr;

  m_bExitGame = false;

  m_bViewWireMode   = false;
  m_bFullScreenMode = true;

  m_nScrWidth  = 0;
  m_nScrHeight = 0;
  m_iScrDepth  = 0;

  m_wActive = false;

  CStr::Init();
}

CApplication::~CApplication() {
  if ( m_hWND ) {
    DestroyWindow( m_hWND );
  }

  CStr::Free();
}

//-------------------------------------------------------------------------------------------------
CApplication* CApplication::Instance() {
  if ( m_pInstance == nullptr ) {
    m_pInstance = new CApplication;
  }

  return m_pInstance;
}

void CApplication::Destroy() {
  SAFE_DELETE( m_pInstance );
}

//-------------------------------------------------------------------------------------------------
void CApplication::ErrorBOX(char* szText, char* szCaption, UINT uType) {
  // #ifdef	_DEBUG???
  ::MessageBox( m_hWND, szText, szCaption, uType );
  //	_ASSERT( 0 );
}

//-------------------------------------------------------------------------------------------------
void CApplication::SetCaption(char* szStr) {
  m_Caption.Set( szStr );
  ::SetWindowText( m_hWND, szStr );
}

//-------------------------------------------------------------------------------------------------
DWORD          CApplication::DisplayFrameRate(void) {
  static DWORD l_dwFrameCount;
  static DWORD l_dwFrameTime = 0;
  static DWORD l_dwFrames    = 0;

  DWORD time2;

  l_dwFrameCount++;
  time2 = g_GameDATA.GetGameTime() - l_dwFrameTime;
  if ( time2 >= 1000 ) {
    l_dwFrames     = (l_dwFrameCount * 1000) / time2;
    l_dwFrameTime  = g_GameDATA.GetGameTime();
    l_dwFrameCount = 0;
  }

  if ( !m_bFullScreenMode ) {
    char* pStr = CStr::Printf( "[ %s ] FPS: %d", m_Caption.Get(), l_dwFrames );
    ::SetWindowText( m_hWND, pStr );
  }

  return l_dwFrames;
}

//-------------------------------------------------------------------------------------------------
bool    CApplication::ParseArgument(char* pStr) {
  char* pToken = nullptr;
  char* pDelimiters = " ,\t\n";

  bool bPassLuncher = false;
#ifdef _DEBUG
  bPassLuncher = true;
#endif

  g_GameDATA.m_nServerID   = 0;
  g_GameDATA.m_nSightRange = 13;

  g_GameDATA.m_bDirectLogin = false;
  g_GameDATA.m_bCheckDupRUN = true;

  pToken = CStr::GetTokenFirst( pStr, pDelimiters );
  while ( pToken ) {
    if ( !strcmp( pToken, "@TRIGGER_SOFT@" ) ) {
      bPassLuncher = true;
    }

    if ( !strcmp( pToken, "_WINDOW_MODE_" ) )
      m_bFullScreenMode = false;
    if ( !strcmp( pToken, "_800" ) ) {
      m_nScrWidth  = 800;
      m_nScrHeight = 600;
    }

    if ( !strcmp( pToken, "_1024" ) ) {
      m_nScrWidth  = 1024;
      m_nScrHeight = 768;
    }

    if ( !strcmp( pToken, "_1280" ) ) {
      m_nScrWidth  = 1280;
      m_nScrHeight = 1024;
    }

    if ( !strcmp( pToken, "_1280P" ) ) {
      m_nScrWidth  = 1280;
      m_nScrHeight = 720;
    }

    if ( !_strcmpi( pToken, "_sight" ) ) {
      pToken = CStr::GetTokenNext( pDelimiters );
      if ( pToken ) {
        g_GameDATA.m_nSightRange = atoi( pToken );
      }
    }
    if ( !_strcmpi( pToken, "_noui" ) ) {
      // 占쏙옙占쏙옙占쏙옙占싱쏙옙 占쏙옙占쌩깍옙. - zho
      g_GameDATA.m_bNoUI = true;
    }

    if ( !_strcmpi( pToken, "_server" ) ) {
      // Get server ip...
      pToken = CStr::GetTokenNext( pDelimiters );
      if ( pToken ) {
        g_GameDATA.m_ServerIP.Set( pToken );
      }
    }

    if ( !_strcmpi( pToken, "_port" ) ) {
      pToken = CStr::GetTokenNext( pDelimiters );
      if ( pToken ) {
        g_GameDATA.m_wServerPORT = atoi( pToken );
      }
    }

    if ( !_strcmpi( pToken, "_serverID" ) ) {
      pToken = CStr::GetTokenNext( pDelimiters );
      if ( pToken ) {
        g_GameDATA.m_nServerID = atoi( pToken );
      }
    }

    if ( !_strcmpi( pToken, "_direct" ) ) {
      g_GameDATA.m_bDirectLogin = true;
    }

    if ( !_strcmpi( pToken, "_test" ) )
      g_GameDATA.m_bForOpenTestServer = true;

    if ( !_strcmpi( pToken, "_dup" ) )
      g_GameDATA.m_bCheckDupRUN = false;

    /// 占쏙옙占쏙옙 占싹븝옙 NHN JAPAN占쏙옙 占쏙옙占쏙옙 Argument Setting( 2005/5/18 )
    if ( !_strcmpi( pToken, "_RCODE_JP_HG" ) )
      g_GameDATA.m_is_NHN_JAPAN = true;

    if ( !_strcmpi( pToken, "_userid" ) ) {
      pToken = CStr::GetTokenNext( pDelimiters );
      if ( pToken ) {
        g_GameDATA.m_Account.Set( pToken );
      }
    }

    if ( !_strcmpi( pToken, "_pw" ) ) {
      pToken = CStr::GetTokenNext( pDelimiters );
      if ( pToken ) {
        g_GameDATA.m_PasswordMD5 = pToken;
      }
    }

    if (!_strcmpi(pToken, "_otp")) {
      pToken = CStr::GetTokenNext(pDelimiters);
      if (pToken) {
        g_GameDATA.m_OTPToken = pToken;
      }
    }

    pToken = CStr::GetTokenNext( pDelimiters );
  }

  if ( !bPassLuncher )
    return false;

  return true;
}

//-----------------------------------------------------------------------------------------------------------------
/// 1. resetScreen()占쏙옙 MoveWindow占쏙옙占쏙옙 占쏙옙占쏙옙 호占쏙옙퓸占쏙옙 client Window占쏙옙 Size占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占� 占싫댐옙.
/// resetScreen占쏙옙 MoveWindow占쏙옙占쏙옙 占쏙옙占쌩울옙 占심곤옙占� MoveWindow占쏙옙占쏙옙 Size占쏙옙占쏙옙占쏙옙  Window占쏙옙 占쏙옙占쏙옙�쇽옙 占쌍댐옙 占쌍댐옙占쏙옙占쏙옙載�
/// 占쏙옙占쏙옙 占쏙옙占싹댐옙 占쏙옙占쏙옙占쏙옙 占쌜곤옙 占실억옙 Size 占쏙옙占썸에 占쏙옙占쏙옙占싼댐옙( navy : 2005/3/11 )
/// 2. 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙 占쌔상도븝옙占쏙옙 占쏙옙占쏙옙크占쏙옙 占싱삼옙占쏙옙占쏙옙 占쏙옙占쏙옙占쎌를 占쏙옙占쏙옙占싹거놂옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占십는댐옙.
///		- 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占쌔상도몌옙 占쏙옙占쌔쇽옙 占쏙옙占쏙옙占싹곤옙占쏙옙占싹댐옙 크占썩를 占쏙옙占쏙옙占쏙옙
//-----------------------------------------------------------------------------------------------------------------
void CApplication::ResizeWindowByClientSize(int& iClientWidth, int& iClientHeight, int iDepth, bool update_engine) {
  if ( m_bFullScreenMode ) {
    if ( update_engine ) {
      setScreen( iClientWidth, iClientHeight, iDepth, g_pCApp->IsFullScreenMode() ); // getFullScreen() );
      setBuffer( iClientWidth, iClientHeight, iDepth );
      resetScreen();
    }

    MoveWindow( m_hWND, 0, 0, GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ), FALSE );
    SetWIDTH( iClientWidth );
    SetHEIGHT( iClientHeight );
    SetDEPTH( iDepth );
  } else {
    _RPT2( _CRT_WARN,"want rect %d %d\n", iClientWidth, iClientHeight );
    _RPT2( _CRT_WARN,"max rect %d %d\n", GetSystemMetrics (SM_CXSCREEN), GetSystemMetrics (SM_CYSCREEN) );

    RECT client_rect = { 0, 0, iClientWidth, iClientHeight };
    if ( AdjustWindowRect( &client_rect, DEFAULT_WINDOWED_STYLE, FALSE ) ) {
      int window_width  = client_rect.right - client_rect.left;
      int window_height = client_rect.bottom - client_rect.top;

      _RPT2( _CRT_WARN,"window rect %d %d\n", window_width, window_height );
      _RPT2( _CRT_WARN,"client rect %d %d\n", iClientWidth, iClientHeight );

      if ( update_engine ) {
        setScreen( iClientWidth, iClientHeight, iDepth, g_pCApp->IsFullScreenMode() ); // getFullScreen() );
        setBuffer( iClientWidth, iClientHeight, iDepth );
        resetScreen();
      }

      MoveWindow( m_hWND, 0, 0, window_width, window_height, TRUE );

      SetWIDTH( iClientWidth );
      SetHEIGHT( iClientHeight );
      SetDEPTH( iDepth );
    } else {
      assert( 0 && "AdjustWindowRect is Failed" );
    }
  }

  ShowWindow( m_hWND, SW_SHOW );
  UpdateWindow( m_hWND );
}

//-------------------------------------------------------------------------------------------------
bool CApplication::CreateWND(char* szClassName, char* szWindowName, short nWidth, short nHeight, int iDepth, HINSTANCE hInstance) {
  m_nScrWidth  = nWidth;
  m_nScrHeight = nHeight;
  m_iScrDepth  = iDepth;

  WNDCLASSEX wcex;

  wcex.cbSize        = sizeof( WNDCLASSEX );
  wcex.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
  wcex.lpfnWndProc   = (WNDPROC)Window_Proc;
  wcex.cbClsExtra    = 0;
  wcex.cbWndExtra    = 0;
  wcex.hInstance     = hInstance;
  wcex.hIcon         = LoadIcon( hInstance, (LPCTSTR)IDI_CLIENT );
  wcex.hCursor       = LoadCursor( nullptr, IDC_ARROW );
  wcex.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
  wcex.lpszMenuName  = (m_bFullScreenMode) ? nullptr : (LPCTSTR)IDC_CLIENT;
  wcex.lpszClassName = szClassName;
  wcex.hIconSm       = LoadIcon( wcex.hInstance, (LPCTSTR)IDI_SMALL );

  RegisterClassEx( &wcex );

  m_Caption.Set( szWindowName );

  if ( !m_bFullScreenMode ) {
    m_hWND = ::CreateWindowEx(
      0,                      // extended window style
      szClassName,            // pointer to registered class name
      szWindowName,           // pointer to window name
      DEFAULT_WINDOWED_STYLE, // window style
      0,                      // (GetSystemMetrics (SM_CXSCREEN)-START_SCR_XLEN)/2,     // horizontal position of window
      0,                      //(GetSystemMetrics (SM_CYSCREEN)-START_SCR_YLEN)/2,     // vertical position of window
      nWidth,                 // window width
      nHeight,                // window height
      nullptr,                // handle to parent or owner window
      nullptr,                // handle to menu, or child-window identifier
      hInstance,              // handle to application instance
      nullptr );              // pointer to window-creation data

    int client_width  = m_nScrWidth;
    int client_height = m_nScrHeight;
    ResizeWindowByClientSize( client_width, client_height, iDepth, false );

  } else {
    m_hWND = ::CreateWindowEx(
      WS_EX_TOPMOST,                   // extended window style
      szClassName,                     // pointer to registered class name
      szWindowName,                    // pointer to window name
      DEFAULT_FULLSCREEN_STYLE,        // window style
      0,                               // horizontal position of window
      0,                               // vertical position of window
      GetSystemMetrics( SM_CXSCREEN ), // window width
      GetSystemMetrics( SM_CYSCREEN ), // window height
      nullptr,                         // handle to parent or owner window
      nullptr,                         // handle to menu, or child-window identifier
      hInstance,                       // handle to application instance
      nullptr );                       // pointer to window-creation data

  }

  if ( m_hWND ) {
    ShowWindow( m_hWND, SW_SHOWNORMAL );
    UpdateWindow( m_hWND );
    SetFocus( m_hWND );
  }

  m_hINS = hInstance;

  return (m_hWND != nullptr);
}

//-------------------------------------------------------------------------------------------------
void CApplication::DestroyWND() {
  if ( m_hWND ) {
    DestroyWindow( m_hWND );
  }
  m_hWND = nullptr;
}

//-------------------------------------------------------------------------------------------------
bool CApplication::GetMessage(void) {
  while ( ::PeekMessage( &m_Message, nullptr, 0, 0, PM_REMOVE ) ) {
    if ( m_Message.message == WM_QUIT ) {
      SetExitGame();
      return false;
    }

    TranslateMessage( &m_Message );
    ::DispatchMessage( &m_Message );
  }

  if ( this->IsActive() ) {
    //
    // Game processing ...
    //
#pragma message ("TODO:: restore surface ..." __FILE__)

    return false;
  }

  return true;
}

void CApplication::SetExitGame() {
  m_bExitGame = true;
}

void CApplication::ResetExitGame() {
  m_bExitGame = false;
}

void CApplication::SetFullscreenMode(bool bFullScreenMode) {
  if ( m_bFullScreenMode == bFullScreenMode ) return; // already fullscreen mode

  if ( bFullScreenMode ) {
    SetWindowLongPtr( m_hWND, GWL_STYLE, DEFAULT_FULLSCREEN_STYLE );
    SetWindowPos( m_hWND, HWND_TOP, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE /*| SWP_NOZORDER*/ | SWP_FRAMECHANGED );

  } else {
    SetWindowLongPtr( m_hWND, GWL_STYLE, DEFAULT_WINDOWED_STYLE );
    SetWindowPos( m_hWND, HWND_NOTOPMOST, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE /*| SWP_NOZORDER*/ | SWP_FRAMECHANGED );
  }
  m_bFullScreenMode = bFullScreenMode;
}

void CApplication::UpdateWindowSize(int iClientWidth, int iClientHeight, int iDepth, bool update_engine) {
  if ( update_engine && iClientWidth == m_nScrWidth && iClientHeight == m_nScrHeight && iDepth == m_iScrDepth )
    return;

  m_nScrWidth  = iClientWidth;
  m_nScrHeight = iClientHeight;
  m_iScrDepth  = iDepth;

  if ( update_engine ) {
    setScreen( iClientWidth, iClientHeight, iDepth, g_pCApp->IsFullScreenMode() );
    setBuffer( iClientWidth, iClientHeight, iDepth );
    resetScreen();
  }

  HNODE hNode = getCameraDefault();
  if ( hNode ) {
    setCameraAspectRatio( hNode, (float)iClientWidth / (float)iClientHeight );
  }

  CCursor::GetInstance().ReloadCursor();

  SIZE sizeScreen = { iClientWidth, iClientHeight };
  CTIme::GetInstance().ChangeScreenSize( sizeScreen );
  g_itMGR.InitInterfacePos();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
