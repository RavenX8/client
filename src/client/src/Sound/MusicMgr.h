#ifndef _MUSIC_MENAGER_
#define _MUSIC_MENAGER_

#include "../Util/JSingleton.h"

class CMusicPlayer;

enum PLAYER_TYPE {
  DIRECT_MUSIC_TYPE = 0,
  OGG_MUSIC_TYPE,
  MSS_MUSIC_TYPE,
};

//---------------------------------------------------------------------------------------------
///
/// class CMusicMgr
/// @brief Music play manager..
///
//---------------------------------------------------------------------------------------------

class CMusicMgr {
private:
  /// ���� ���� �÷��̾�.
  CMusicPlayer* m_pPlayer;
  bool          m_bReadyDevice;

  CMusicMgr(void );
public:
  ~CMusicMgr(void);

  CMusicMgr(const CMusicMgr&) = delete;
  CMusicMgr& operator=(const CMusicMgr &) = delete;
  CMusicMgr(CMusicMgr &&) = delete;
  CMusicMgr & operator=(CMusicMgr &&) = delete;

  static auto& GetSingleton(){
    static CMusicMgr inst;
    return inst;
  }

  bool Init(PLAYER_TYPE playerType);
  bool bIsReady() { return m_bReadyDevice; }
  void Clear();

  bool Play(const char* fName);
  void Stop();

  void Run();
  void Pause();

  void SetVolume(long lVolume);

  void HandleEvent();
};

#endif //_MUSIC_MENAGER_
