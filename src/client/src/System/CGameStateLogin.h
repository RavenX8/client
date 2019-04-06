#ifndef _CGAMESTATELOGIN_
#define _CGAMESTATELOGIN_
#include "cgamestate.h"

/**
* 로그인 State Class
*	- Next State : CGameStateSelectSvr
*
* @Author		최종진
* @Date			2005/9/15
*/
class CGameStateLogin : public CGameState {
private:

public:
  CGameStateLogin(int  iID);
  ~CGameStateLogin(void);

  int  Update(bool bLostFocus) override;
  int  Enter(int   iPrevStateID) override;
  int  Leave(int   iNextStateID) override;
  void ServerDisconnected() override;
  void AcceptedConnectLoginSvr() override;
  int  ProcMouseInput(UINT    uiMsg, WPARAM wParam, LPARAM lParam) override;
  int  ProcKeyboardInput(UINT uiMsg, WPARAM wParam, LPARAM lParam) override;

};
#endif
