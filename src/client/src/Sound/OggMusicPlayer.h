#ifndef _OGG_MUSIC_PLAYER_
#define _OGG_MUSIC_PLAYER_

#include "MusicPlayer.h"
#include "../Util/ADSStream.h"

//---------------------------------------------------------------------------------------------
///
/// class COggMusicPlayer
/// @brief ogg file player
///
//---------------------------------------------------------------------------------------------

class COggMusicPlayer : public CMusicPlayer {
private:
  AOggSTREAM* m_pOggStream;

public:
  COggMusicPlayer(void );
  ~COggMusicPlayer(void);

  void HandleEvent() override;

  bool Init() override;
  bool Play(const char* fName) override;
  void Stop() override;

  void Run() override;
  void Pause() override;

  void SetVolume(long lVolume) override;
};

#endif //_OGG_MUSIC_PLAYER_
