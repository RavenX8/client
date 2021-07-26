/*
  $Header: /Client/Game.h 98    05-08-25 1:50p Choo0219 $
*/
#ifndef	__GAME_H
#define	__GAME_H

#include "Util/JSingleton.h"
#include <string>

//-------------------------------------------------------------------------------------------------

#define	SHADER_NOLIT_SKIN	0
#define	SHADER_NOLIT		1

enum {
  FONT_NORMAL = 0,
  FONT_LARGE,
  FONT_SMALL,
  FONT_NORMAL_BOLD,
  FONT_LARGE_BOLD,
  FONT_SMALL_BOLD,
  FONT_TITLE,
  FONT_DESCRIPTION,
  FONT_NORMAL_OUTLINE,
  FONT_OUTLINE_18_BOLD,
  FONT_OUTLINE_14_BOLD,
  FONT_OUTLINE_24_BOLD,
  FONT_OUTLINE_16_BOLD,
  FONT_OUTLINE_11_BOLD,
  MAX_FONT,
};

enum {
  PVP_CANT = 0,
  PVP_PERMITION_ALL = 1,
  PVP_NON_PARTY_ALL = 2,
};

class CGAMEDATA {
  CGAMEDATA();
public:
  ~CGAMEDATA();

  CGAMEDATA(const CGAMEDATA&) = delete;
  CGAMEDATA& operator=(const CGAMEDATA &) = delete;
  CGAMEDATA(CVFSManager &&) = delete;
  CGAMEDATA & operator=(CGAMEDATA &&) = delete;

  static auto& GetSingleton(){
      static CGAMEDATA inst;
      return inst;
  }

public:

  bool m_bWireMode;
  bool m_bDrawBoundingVolume;

  HNODE m_hShader_nolit_skin;
  HNODE m_hShader_nolit;
  HNODE m_hShader_terrain;
  HNODE m_hShader_terrain_Rough;
  HNODE m_hShader_lightmap;
  HNODE m_hShader_ocean;
  HNODE m_hShader_specular;
  HNODE m_hShader_specular_skin;
  HNODE m_hShader_lit_skin;
  HNODE m_hShader_lit;
  HNODE m_hShader_sky;

  bool m_bNoUI;     // �������̽� ǥ������ ����. ����Ʈ false - zho
  bool m_bNoWeight; ///�������� ���� flag: ���� Ŭ���̾�Ʈ���� ���� 

  bool    m_bTranslate; // Raven - Translate Tool
  CStrVAR m_cLang;      // Raven- Language

  bool m_bFilmingMode;      // �÷��� ������ �Կ� ���
  bool m_bShowCursor;       // ���콺 Ŀ�� ���̳� �Ⱥ��̳�
  bool m_bShowDropItemInfo; // ��������� ���� ���̱�( ���� alt Ű�� ������� Ȱ��ȭ )

#ifdef _DEBUG
  bool				m_bShowCurPos;
#endif

  int m_iPvPState; /// PvP State

  /// �ý��� Ÿ��.
  SYSTEMTIME m_SystemTime;

  bool  m_bDisplayDebugInfo;
  float m_fTestVal;
  bool  m_bDirectLogin;
  bool  m_bForOpenTestServer;

  int  m_iTemp;
  bool m_bCheckDupRUN; /// �ߺ����� ����...

  HNODE m_hLight;
  HNODE m_hCharacterLight;
  HNODE m_hCharacterLight2;

  CStrVAR m_TrailTexture;

  HNODE m_hFONT[ MAX_FONT ];

  tPOINTF          m_PosCENTER;
  POINTS           m_PosPATCH;
  CRITICAL_SECTION m_csNZIN;

  /// �þ߰��� ������.
  short m_nSightRange;

  /// ���¹̳� ������
  int m_iWorldStaminaVal;
  int AdjustAvatarStamina(int iGetExp);

  short         m_nServerID;
  CStrVAR       m_ServerIP;
  WORD          m_wServerPORT;
  CStrVAR       m_Account;
  CStrVAR       m_Password;
  std::string   m_PasswordMD5;
  std::string   m_OTPToken;

  void Init() { InitializeCriticalSection( &m_csNZIN ); }
  void Free() { DeleteCriticalSection( &m_csNZIN ); }
  void Lock() { EnterCriticalSection( &m_csNZIN ); }
  void Unlock() { LeaveCriticalSection( &m_csNZIN ); }

  ///
  ///	Game time ����..
  ///

  //DWORD				GetGameTime(){ return ::timeGetTime(); }
  DWORD m_dwGameStartTime;    /// ���� ���� �ð�.
  DWORD m_dwElapsedGameTime;  /// ���� ������ ����ð�.
  DWORD m_dwFrevFrameTime;    /// ���� ������ Ÿ��
  DWORD m_dwElapsedFrameTime; /// ���� �����ӿ����� ����ð�..

  void  UpdateGameTime();
  DWORD GetTime() { return timeGetTime(); }
  DWORD GetGameTime() { return GetTime(); }
  DWORD GetElapsedFrameTime() { return m_dwElapsedFrameTime; } // ���� ���������κ����� ��� �ð�.

  /// ���ӽ����� ����� �����Ӽ�
  DWORD m_dwElapsedGameFrame;

  ///
  /// Random number
  ///

  int  Random(unsigned int         iRand) { return rand() % iRand; };
  void RandomSeedInit(unsigned int iSeed) { srand( iSeed ); }

  void Update();

  //--------------------------------------------------------------------------------------
  /// ���� �������
  //--------------------------------------------------------------------------------------
  bool m_bJustObjectLoadMode; /// ���� ����� ������� ���� ���ΰ�?

#if defined( _DEBUG ) || defined( _D_RELEASE )
  bool				m_bObserverCameraMode; /// ������ ������ ī�޶�..
#endif

  //--------------------------------------------------------------------------------------
  /// ����ġ ȹ�� �����̸� ���� ����
  //--------------------------------------------------------------------------------------
  int64_t m_iReceivedAvatarEXP;

  //--------------------------------------------------------------------------------------
  /// ������ ä�� ����
  //--------------------------------------------------------------------------------------
  std::string m_strJoinServerName;
  std::string m_strJoinChannelName;

  //--------------------------------------------------------------------------------------
  /// NHN Japan�� ���� Data
  //--------------------------------------------------------------------------------------
  bool m_is_NHN_JAPAN;
private:

};

#define g_GameDATA (CGAMEDATA::GetSingleton())

extern D3DCOLOR g_dwRED;
extern D3DCOLOR g_dwGREEN;
extern D3DCOLOR g_dwBLUE;
extern D3DCOLOR g_dwBLACK;
extern D3DCOLOR g_dwWHITE;
extern D3DCOLOR g_dwYELLOW;
extern D3DCOLOR g_dwGRAY;
extern D3DCOLOR g_dwVIOLET;
extern D3DCOLOR g_dwORANGE;
extern D3DCOLOR g_dwPINK;
extern D3DCOLOR g_dwCOLOR[];

//void AddMsgToChatWND (char *szMsg, D3DCOLOR Color, int iType = 0 );///iType: 0 == CChatDLG::CHAT_TYPE_NORMAL
void DrawLoadingImage();
void DestroyWaitDlg();
//-------------------------------------------------------------------------------------------------
#endif
