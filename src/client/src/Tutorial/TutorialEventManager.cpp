#include "StdAfx.h"
#include "./TutorialEventManager.h"
#include "TutorialEventUtility.h"
#include "TutorialEventManager.h"
#include "../Game.h"
//---------------------------------------------------------------------------------------
///  Tutorial Image������� �߰�			2005/5/29		nAvy
//---------------------------------------------------------------------------------------

//CTutorialEventManager _TutorialEventManager;

CTutorialEventManager::CTutorialEventManager(void) {}

CTutorialEventManager::~CTutorialEventManager(void) {}

bool CTutorialEventManager::Init() {
  if ( CTutorialEventUtility::GetSingleton().Init() == false )
    return false;

  /// Init tutorial event
  if ( m_TutorialEvent.Init() == false )
    return false;

  /// Init Levelup event
  if ( m_LevelUpEvent.Init() == false )
    return false;

  return true;
}

void CTutorialEventManager::Release() {
  CTutorialEventUtility::GetSingleton().Release();

  m_TutorialEvent.Release();
  m_LevelUpEvent.Release();

  ///������ draw ���̰ų� ���� ������� ��� �̹����� �����Ѵ�.
  if ( !m_tutorial_images.empty() ) {
    stopSpriteSFX(); ///znzin
    S_TutorialImage item;
    while ( !m_tutorial_images.empty() ) {
      item = m_tutorial_images.front();
      unloadTexture( item.m_hNode ); //znzin
      m_tutorial_images.pop_front();
    }
  }

}

void CTutorialEventManager::Proc() {
  /// Check Tutorial event
  m_TutorialEvent.CheckEvent();
  ProcImage();
}

void CTutorialEventManager::CheckLevelUpEvent(int iLevel) {
  m_LevelUpEvent.CheckLevelEvent( iLevel );
}

//---------------------------------------------------------------------------------------------------------------------------------
/// Ʃ�丮�� �̹����� ȭ�鿡 ������ ��ġ�� �����ð����� �����ְų� ť�� �����Ѵ�.
/// @param const char* filename	: �̹��� ȭ�ϳ���( path/filename )
/// @param int		x			: ����� ȭ���� x��ǥ
/// @param int		y			: ����� ȭ���� y��ǥ
/// @param float	fade_time	: ������� �̹����� ȭ�鿡�� ������� �����ϴ� �ð�( max_time ���ٴ� �۾ƾ� �Ѵ� )
/// @param float	max_time	: �̹����� ȭ�鿡 ǥ�õ� �� �ð�
/// @param int		append_or_renewal	: ���� ������� �̹����� ������� �����ϰ� �����ٰ��ΰ�? ����� ������ �����ٰ��ΰ�?
//---------------------------------------------------------------------------------------------------------------------------------
void CTutorialEventManager::RegistImage(const char* filename, int x, int y, float fadein_endtime, float fadeout_starttime, float max_time, int append_or_renewal) {
  assert( filename );
  if ( nullptr == filename )
    return;

  if ( append_or_renewal ) {
    S_TutorialImage newitem;

    newitem.m_drawstart_time    = 0;
    newitem.m_filename          = filename;
    newitem.m_displayscreen_x   = x;
    newitem.m_displayscreen_y   = y;
    newitem.m_fadein_endtime    = fadein_endtime;
    newitem.m_fadeout_starttime = fadeout_starttime;
    newitem.m_max_time          = max_time;

    if ( LoadImage( newitem ) ) {
      ZZ_RECT   SrcRect  = { 0, 0, newitem.m_width, newitem.m_height };
      ZZ_VECTOR Center   = { 0, 0, 0 };
      ZZ_VECTOR Position = { (float)x, (float)y, 0 };

      m_tutorial_images.push_back( newitem );
    }
  } else {
    S_TutorialImage item;

    ///������ draw ���̰ų� ���� ������� ��� �̹����� �����Ѵ�.
    if ( !m_tutorial_images.empty() ) {
      stopSpriteSFX(); ///znzin

      while ( !m_tutorial_images.empty() ) {
        item = m_tutorial_images.front();
        unloadTexture( item.m_hNode ); //znzin
        m_tutorial_images.pop_front();
      }
    }

    /// znzin�� Draw ��û�� queue�� ����
    S_TutorialImage newitem;
    newitem.m_drawstart_time    = 0;
    newitem.m_filename          = filename;
    newitem.m_displayscreen_x   = x;
    newitem.m_displayscreen_y   = y;
    newitem.m_fadein_endtime    = fadein_endtime;
    newitem.m_fadeout_starttime = fadeout_starttime;
    newitem.m_max_time          = max_time;

    if ( LoadImage( newitem ) ) {
      ZZ_RECT   SrcRect  = { 0, 0, newitem.m_width, newitem.m_height };
      ZZ_VECTOR Center   = { 0, 0, 0 };
      ZZ_VECTOR Position = { (float)x, (float)y, 0 };
      inputSpriteSFX( newitem.m_hNode, &SrcRect, &Center, &Position, D3DCOLOR_RGBA( 255, 255, 255, 255 ), fadein_endtime, fadeout_starttime, max_time );

      newitem.m_drawstart_time = g_GameDATA.GetTime();
      m_tutorial_images.push_back( newitem );
    }
  }
}

//-----------------------------------------------------------------------------------------------------
/// @brief Tutorial Image�� Loading�ϰ� queue�� push�� item�� �����Ѵ�.
//-----------------------------------------------------------------------------------------------------
bool CTutorialEventManager::LoadImage(S_TutorialImage& newitem) {

  setDelayedLoad( 0 ); //znzin : delay loading off
  newitem.m_hNode = loadTexture( newitem.m_filename.c_str(), newitem.m_filename.c_str(), 1, 0 );
  setDelayedLoad( 1 ); //znzin : delayed loading on

  if ( newitem.m_hNode ) {
    getTextureSize( newitem.m_hNode, newitem.m_width, newitem.m_height ); //znzin
    assert( newitem.m_width );
    assert( newitem.m_height );
    return true;
  }

  _RPT1( _CRT_ASSERT,"Tutorial Image loadTexture Error %s", newitem.m_filename.c_str() );
  return false;
}

//-----------------------------------------------------------------------------------------------------
/// @brief Tutorial Image���� Update Method
///			- queue�� ���� Display���� ���� Image�� �ִٸ� Load, znzin�� ��û
///			- queue�� front�� Display�� ���� Image�� �ִٸ� unload
//-----------------------------------------------------------------------------------------------------
void CTutorialEventManager::ProcImage() {
  if ( m_tutorial_images.empty() )
    return;

  DWORD           curr_time = g_GameDATA.GetTime();
  S_TutorialImage front_item;

  front_item = m_tutorial_images.front();

  DWORD dwDrawEndExpectTime = (DWORD)(front_item.m_drawstart_time + front_item.m_max_time * 1000);

  ///Show ��� ���� item�� �������
  if ( front_item.m_drawstart_time == 0 ) {
    if ( front_item.m_drawstart_time == 0 ) {
      m_tutorial_images.pop_front();

      ZZ_RECT   SrcRect  = { 0, 0, front_item.m_width, front_item.m_height };
      ZZ_VECTOR Center   = { 0, 0, 0 };
      ZZ_VECTOR Position = { (float)front_item.m_displayscreen_x, (float)front_item.m_displayscreen_y, 0 };
      inputSpriteSFX( front_item.m_hNode, &SrcRect, &Center, &Position, D3DCOLOR_RGBA( 255, 255, 255, 255 ), front_item.m_fadein_endtime, front_item.m_fadeout_starttime, front_item.m_max_time );
      front_item.m_drawstart_time = g_GameDATA.GetTime();
      m_tutorial_images.push_front( front_item );
    }
  } else if ( dwDrawEndExpectTime < curr_time ) ///Draw�� ���� item�� �������
  {
    HNODE hCurrentTextureNode = getSpriteSFXCurrentTexNode();
    if ( hCurrentTextureNode ) {
      if ( hCurrentTextureNode != front_item.m_hNode ) {
        unloadTexture( front_item.m_hNode ); //znzin
        m_tutorial_images.pop_front();
      }
    } else {
      unloadTexture( front_item.m_hNode ); //znzin
      m_tutorial_images.pop_front();
    }
  }
}
