#pragma once

#include "cgamestate.h"

class CGameStateWarning : public CGameState {
private:

public:
  CGameStateWarning(int  iID);
  ~CGameStateWarning(void);

  int Update(bool bLostFocus) override;
  int Enter(int   iPrevStateID) override;
  int Leave(int   iNextStateID) override;

  int ProcMouseInput(UINT    uiMsg, WPARAM wParam, LPARAM lParam) override;
  int ProcKeyboardInput(UINT uiMsg, WPARAM wParam, LPARAM lParam) override;

};
