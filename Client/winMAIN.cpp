// Client.cpp : Defines the entry point for the application.

//*-------------------------------------------------------------------------------------------------------------------*
// 2005 / 06 / 20 : nAvy 
// 사용자 정의 전처리기 설명 : 
//  1. _TAIWAN  : TAIWAN, PHILIPHIN 접속시 로그인서버에게 국가 코드( 현재 950 )를 보낼지를 구분하기 위한 정의
//  2. _DE		: 중복실행 가능한 버젼인지에 대한 정의( KR )에만 적용시킨다.
//
//  * 국가별 솔루션 구성
//	1. 한국 
//      1) 본섭   : Release_DE
//		2) 제니퍼 : Release
//
//  2. 일본, 미국 : Release
//
//  3. 대만, 필리핀 :Release_TAIWAN
//*-------------------------------------------------------------------------------------------------------------------*

#include "StdAfx.h"

#ifdef _DEBUG
extern "C" { 
    FILE __iob_func[3] = { *stdin,*stdout,*stderr };
    FILE _iob[3] = { __iob_func[0], __iob_func[1], __iob_func[2] };
}
#endif

#include "CApplication.h"
#include "Game.h"
//#include "CMouse.h"
//#include "CKeyboard.h"
#include "Network//CNetwork.h"
#include "Util//VFSManager.h"
#include "Util//SystemInfo.h"
#include "CClientStorage.h"
#include "System/CGame.h"
#include "Interface/ExternalUI/CLogin.h"
#include "Debug.h"
#include "triggerinfo/TriggerInfo.h"

#include "Util/CheckHack.h"
#include "Country.h"
//#include "nProtect/nProtect.h"

///#include "Util/JDebugNew.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "MiniDumper.h"

//MiniDumper g_MiniDump;

//*--------------------------------------------------------------------------------------*/
// 95,98, me에서 Unicode관련 Api Wrapper dll Load
//HMODULE LoadUnicowsProc(void); 
//HMODULE g_hUnicows = NULL;
//#ifdef _cplusplus
//extern "C" {
//#endif
//extern FARPROC _PfnLoadUnicows = (FARPROC) &LoadUnicowsProc;
//#ifdef _cplusplus
//}
//#endif
//
//
//// Unicode Load
//HMODULE LoadUnicowsProc(void)
//{
//	g_hUnicows = LoadLibraryA("unicows.dll");
//    return g_hUnicows;
//}
//*--------------------------------------------------------------------------------------*/

//-------------------------------------------------------------------------------------------------
bool   Init_DEVICE(void) {
  bool bRet = false;

  //--------------------------[ engine related ]-----------------------//
  initZnzin();
  openFileSystem( "data.idx" );

  doScript( "scripts/init.lua" );

  t_OptionResolution Resolution = g_ClientStorage.GetResolution();
  setDisplayQualityLevel( c_iPeformances[g_ClientStorage.GetVideoPerformance()] );
  t_OptionVideo Video;
  g_ClientStorage.GetVideoOption( Video );
  setFullSceneAntiAliasing( Video.iAntiAlising );

  if ( !g_pCApp->IsFullScreenMode() ) {
    RECT ClientRt;
    GetClientRect( g_pCApp->GetHWND(), &ClientRt );
    setScreen( ClientRt.right, ClientRt.bottom, Resolution.iDepth, g_pCApp->IsFullScreenMode() );
  } else
    setScreen( g_pCApp->GetWIDTH(), g_pCApp->GetHEIGHT(), Resolution.iDepth, g_pCApp->IsFullScreenMode() );

  bRet = attachWindow( (const void*)g_pCApp->GetHWND() );

  CD3DUtil::Init();

  g_pSoundLIST = new CSoundLIST( g_pCApp->GetHWND() );
  g_pSoundLIST->Load( "3DDATA\\STB\\FILE_SOUND.stb" );

  return bRet;
}

//-------------------------------------------------------------------------------------------------
void Free_DEVICE(void) {
  delete g_pSoundLIST;

  CD3DUtil::Free();

  //--------------------------[ engine related ]-----------------------//
  detachWindow();

  closeFileSystem();
  destZnzin();
}

//------------------------------------------------------------------------------------------------
// 2005. 5. 6. 조호동
// 중복 실행 체크용 소켓 해제
SOCKET listener;

void CloseDuplicateAppSocket(void) {
  closesocket( listener );
  WSACleanup();
}

// 중복 실행 체크 : 특정 포트가 중복 생성 안되는 점을 이용.
bool      IsDuplicateApp(void) {
  WSADATA wsadata;
  WSAStartup( MAKEWORD( 2, 2 ), &wsadata );

  // socket 생성
  listener = socket( AF_INET, SOCK_STREAM, 0 );
  // listening
  sockaddr_in addr;
  memset( &addr, 0, sizeof( sockaddr_in ) );
  addr.sin_family           = AF_INET;
  addr.sin_addr.S_un.S_addr = INADDR_ANY;
  addr.sin_port             = htons( 7777 );

  int result = ::bind( listener, (sockaddr*)&addr, sizeof( sockaddr_in ) );
  result     = listen( listener, 5 );

  if ( result == SOCKET_ERROR ) {
    CloseDuplicateAppSocket();
    MessageBox( nullptr, "이미 게임이 실행 중입니다 !", "에러", MB_OK );
    return TRUE;
  }

  return FALSE;
}

//-------------------------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
#ifdef _DEBUG
  // This will check for memory leaks.  They will show up in your
  // output window along with the line number.  Replace the 
  // -1 argument in the second call with the allocation number
  // and then trace back through the call stack to find the leak.
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetBreakAlloc(-1L);
#endif
  MiniDumper g_MiniDump;

  CCountry::GetSingleton().CheckCountry();

  g_SystemInfo.CollectingSystemInfo();
  int iWindowVersion = g_SystemInfo.GetWindowsVersion();

  //-------------------------------------------------------------------------------
  /// Init Trigger VFS
  //-------------------------------------------------------------------------------
  VHANDLE hVFS = OpenVFS( "data.idx", "mr" );
  (CVFSManager::GetSingleton()).SetVFS( hVFS );
  (CVFSManager::GetSingleton()).InitVFS( VFS_TRIGGER_VFS );

  //-------------------------------------------------------------------------------
  /// Get Time
  //-------------------------------------------------------------------------------
  GetLocalTime( &g_GameDATA.m_SystemTime );

  //-------------------------------------------------------------------------------
  /// Init System object
  //-------------------------------------------------------------------------------
  g_pCApp   = CApplication::Instance();
  g_pNet    = CNetwork::Instance( hInstance );
  g_pCRange = CRangeTBL::Instance();

  //-------------------------------------------------------------------------------
  /// Load Range table
  //-------------------------------------------------------------------------------
  if ( !g_pCRange->Load_TABLE( "3DDATA\\TERRAIN\\O_Range.TBL" ) ) {
    g_pCApp->ErrorBOX( "3DDATA\\TERRAIN\\O_Range.TBL file open error", CUtil::GetCurrentDir(), MB_OK );
    return 0;
  }

  //-------------------------------------------------------------------------------
  /// IP/Port Setting
  //-------------------------------------------------------------------------------
  g_GameDATA.m_wServerPORT = TCP_LSV_PORT;
  g_GameDATA.m_ServerIP.Set( TCP_LSV_IP );

  if ( !g_pCApp->ParseArgument( lpCmdLine ) )
    return 0;

  //-------------------------------------------------------------------------------
  /// 윈도우 생성시 해상도에 관한 Data가 필요하여 이곳에서 로드한다.
  //-------------------------------------------------------------------------------	
  g_TblResolution.Load2( "3DDATA\\STB\\RESOLUTION.STB", false, false );
  g_TblCamera.Load2( "3DDATA\\STB\\LIST_CAMERA.STB", false, false );

  //-------------------------------------------------------------------------------
  ///클라이언트에 저장된 Data를 로드한다.
  //-------------------------------------------------------------------------------
  g_ClientStorage.Load();

  //-------------------------------------------------------------------------------
  ///이전 옵션에서 조정된 해상도의 인덱스를 가져와서 g_TblResolution을 참조하여
  ///해상도를 조정한다.
  //-------------------------------------------------------------------------------	
#pragma message("저장된 해상도를 테스트해서 현재 실행가능한지 판단하고 안되면 메세지 박스 띄우고 TriggerDetect를 실행시킨다")
  t_OptionResolution Resolution = g_ClientStorage.GetResolution();
  /// 범위값 체크 
  UINT iFullScreen = g_ClientStorage.GetVideoFullScreen();

  g_pCApp->SetFullscreenMode( iFullScreen );
  g_pCApp->CreateWND( "classCLIENT", "RoseOnline", Resolution.iWidth, Resolution.iHeight, Resolution.iDepth, hInstance );

#ifndef _DEBUG
  // 시스템 정보를 모음
  TI_ReadSysInfoFile();
#endif

  // *-------------------------------------------------------------------* //
  g_pObjMGR = CObjectMANAGER::Instance();
  g_pCApp->ResetExitGame();

  bool bDeviceInitialized = Init_DEVICE();

  if ( bDeviceInitialized ) {

    ///nProtect				박 지호
    //m_nProtectSys.Set_hWnd(g_pCApp->GetHWND());

    ///중복실행가능 버젼	
    CGame::GetInstance().GameLoop();
  }

  Free_DEVICE();

  // *-------------------------------------------------------------------* //
  g_TblCamera.Free();
  g_TblResolution.Free();
  // *-------------------------------------------------------------------* //

  g_pCApp->Destroy();
  g_pNet->Destroy();

  //-------------------------------------------------------------------------------
  ///2004/3/26/nAvy:Release 구성에서 Debug Mode(F5로실행)로 실행시 로그인 화면에서 취소하면
  ///Error난다. 이유 모름 , 하지만 그냥 실행시에는 (bat파일로 혹은 ctrl+f5) Error가 안난다.
  //-------------------------------------------------------------------------------		
  g_pCRange->Destroy();

  (CVFSManager::GetSingleton()).ReleaseAll();
  CloseVFS(hVFS);

  return 0;
}

//-------------------------------------------------------------------------------------------------
