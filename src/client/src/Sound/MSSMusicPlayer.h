#ifndef _MSS_MUSIC_PLAYER_
#define _MSS_MUSIC_PLAYER_

#include "MusicPlayer.h"
#ifdef _USE_MSS
#include "MSS.h"
#endif

//---------------------------------------------------------------------------------------------
///
/// class CMssMusicPlayer
/// @brief miles sound system BGM player
///
//---------------------------------------------------------------------------------------------

class CMSSMusicPlayer : public CMusicPlayer {
private:
#ifdef _USE_MSS
	HDIGDRIVER		m_hDig;
	HSTREAM			m_hStream;
	
	U32				m_Paused;	
#endif

public:
  CMSSMusicPlayer(void );
  ~CMSSMusicPlayer(void);

  void HandleEvent() override;

  bool Init() override;
  bool Play(const char* fName) override;
  void Stop() override;

  void Run() override;
  void Pause() override;

  void SetVolume(long lVolume) override;
};

#endif //_MSS_MUSIC_PLAYER_
