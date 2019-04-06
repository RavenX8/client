#ifndef _DIRECT_MUSIC_PLAYER_
#define _DIRECT_MUSIC_PLAYER_

#include "MusicPlayer.h"
#include "../util/CMMPlayer.h"

#define MAX_MUSIC_VOLUME		0

#define	WM_MUSIC_EVENT	( WM_APP + 9238 )

//---------------------------------------------------------------------------------------------
///
/// class CDirectMusicPlayer
/// @brief use direct show..
///
//---------------------------------------------------------------------------------------------

class CDirectMusicPlayer : public CMusicPlayer {
private:
  classMMPLAYER m_MMPlayer;
  IBasicAudio*  m_pBA;

public:
  CDirectMusicPlayer(void );
  ~CDirectMusicPlayer(void);

  bool Init() override;
  bool Play(const char* fName) override;
  void Stop() override;

  void Run() override;
  void Pause() override;

  void SetVolume(long lVolume) override;

  void HandleEvent() override;
};

#endif //_DIRECT_MUSIC_PLAYER_
