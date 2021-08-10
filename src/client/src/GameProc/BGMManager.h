#ifndef _BGM_MANAGER_
#define _BGM_MANAGER_
#include "../util/JSingleton.h"

//---------------------------------------------------------------------------------------
///
/// class CBGManager
/// ������� �÷��̾� ����, ���� ����..
///
//---------------------------------------------------------------------------------------
class CBGMManager {
private:
  int  m_iCurrentVolumeIdx;
  int  m_iStartVolumeIdx;
  bool m_bToFadeOut;

  DWORD m_dwTransitionPrevTime;
  DWORD m_dwElapsedTime;

  bool m_bEnableTransition;
  bool m_bToNight;

  int m_iZoneNO;

  CBGMManager(void );
public:
  ~CBGMManager(void);

  CBGMManager(const CBGMManager&) = delete;
  CBGMManager& operator=(const CBGMManager &) = delete;
  CBGMManager(CBGMManager &&) = delete;
  CBGMManager & operator=(CBGMManager &&) = delete;

  static auto& GetSingleton(){
    static CBGMManager inst;
    return inst;
  }

  /// �㳷 ���濡 ���� ������� ����..
  void ChangeBGMMusicWithDayAndNight(int iZoneNO, bool bToNight);

  void EndTransition();
  void Proc();
};

#endif //_BGM_MANAGER_
