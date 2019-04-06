#ifndef _CGAMESTATECREATEAVATAR_
#define _CGAMESTATECREATEAVATAR_
#include "CGameState.h"
#include <list>

class CJustModelAVT;

/**
* 캐릭터 생성 State Class
*	- Next State  : CGameStateSelectAvatar
*
* @Author		최종진
* @Date			2005/9/15
*/
class CGameStateCreateAvatar : public CGameState {
public:
  CGameStateCreateAvatar(int  iID);
  ~CGameStateCreateAvatar(void);

  int Update(bool bLostFocus) override;
  int Enter(int   iPrevStateID) override;
  int Leave(int   iNextStateID) override;

  int ProcMouseInput(UINT    uiMsg, WPARAM wParam, LPARAM lParam) override;
  int ProcKeyboardInput(UINT uiMsg, WPARAM wParam, LPARAM lParam) override;

};
#endif
