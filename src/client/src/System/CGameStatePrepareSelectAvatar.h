#ifndef _CGAMESTATEPREPARESELECTAVATAR_
#define _CGAMESTATEPREPARESELECTAVATAR_
#include "CGameState.h"

/**
* CGameStateSelectAvatar에 필요한 전처리를 하는 State Class
*	- Next State : CGameStateSelectAvatar
* @Author	최종진	
* @Date		2005/9/15
*/
class CGameStatePrepareSelectAvatar : public CGameState {
  HANDLE m_hThread;
public:
  CGameStatePrepareSelectAvatar(int  iID);
  ~CGameStatePrepareSelectAvatar(void);

  int Update(bool bLostFocus) override;
  int Enter(int   iPrevStateID) override;
  int Leave(int   iNextStateID) override;

  int ProcMouseInput(UINT    uiMsg, WPARAM wParam, LPARAM lParam) override { return 0; }
  int ProcKeyboardInput(UINT uiMsg, WPARAM wParam, LPARAM lParam) override { return 0; }

protected:
  static unsigned __stdcall ThreadFunc(void* pArguments);
  void                      Draw();

};
#endif
