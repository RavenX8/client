#ifndef _CGAMESTATESELECTAVATAR_
#define _CGAMESTATESELECTAVATAR_
#include "CGameState.h"

/**
* 캐릭터 선택 State Class
*	- Next State : CGameStateCreateAvatar, CGameStateMoveMain, CGameStateLoginVirtual
*
* @Author	최종진	
* @Date		2005/9/15
*/
class CGameStateSelectAvatar : public CGameState {
public:
  CGameStateSelectAvatar(int  iID);
  ~CGameStateSelectAvatar(void);

  int Update(bool bLostFocus) override;
  int Enter(int   iPrevStateID) override;
  int Leave(int   iNextStateID) override;

  void ServerDisconnected() override;

  int ProcMouseInput(UINT    uiMsg, WPARAM wParam, LPARAM lParam) override;
  int ProcKeyboardInput(UINT uiMsg, WPARAM wParam, LPARAM lParam) override;

};
#endif
