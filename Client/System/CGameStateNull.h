#ifndef _CGAMESTATENULL_
#define _CGAMESTATENULL_
#include "CGameState.h"

class CGameStateNull : public CGameState {
public:
  CGameStateNull(int  iID);
  ~CGameStateNull(void);

  int Update(bool bLostFocus) override { return 0; }
  int Enter(int   iPrevStateID) override { return 0; }
  int Leave(int   iNextStateID) override { return 0; }

  int ProcMouseInput(UINT    uiMsg, WPARAM wParam, LPARAM lParam) override { return 0; }
  int ProcKeyboardInput(UINT uiMsg, WPARAM wParam, LPARAM lParam) override { return 0; }

protected:
  int ProcWndMsg() { return 0; }
};
#endif
