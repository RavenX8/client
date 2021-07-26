#ifndef _SFX_MANAGER_
#define _SFX_MANAGER_

#include <list>
#include "../Util/JSingleton.h"

class ISFX;

//-------------------------------------------------------------------------------------------
///
/// class CSFXManager
/// @brief manager of sfx in game.
///
//-------------------------------------------------------------------------------------------

class CSFXManager {
private:
  std::list<ISFX*> m_SFXList;

  CSFXManager(void );
public:
  ~CSFXManager(void);

  CSFXManager(const CSFXManager&) = delete;
  CSFXManager& operator=(const CSFXManager &) = delete;
  CSFXManager(CSFXManager &&) = delete;
  CSFXManager & operator=(CSFXManager &&) = delete;

  static auto& GetSingleton(){
    static CSFXManager inst;
    return inst;
  }

  //-----------------------------------------------------------------------------------------
  /// General purpose..
  //-----------------------------------------------------------------------------------------

  void Update();
  void Draw();
  void Clear();

  ISFX* AddSFX(ISFX*    pSFX);
  bool  DeleteSFX(ISFX* pSFX);

  ISFX* FindSFXWithType(int iSFXType);

  //-----------------------------------------------------------------------------------------
  /// Specific special effect
  //-----------------------------------------------------------------------------------------

  bool StartWideScreenEffect();
  void StopWideScreenEffect();

};

#endif //_SFX_MANAGER_
