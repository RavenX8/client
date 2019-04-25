#ifndef _CGameStateSelectSvr_
#define _CGameStateSelectSvr_
#include "cgamestate.h"

/**
* 서버 선택 State Class
*
* @Author	최종진
* @Date		2005/9/15
*/
class CGameStateSelectSvr : public CGameState {
public:
          CGameStateSelectSvr(int  iID);
  virtual ~CGameStateSelectSvr(void);

  int  Update(bool bLostFocus) override;
  int  Enter(int   iPrevStateID) override;
  int  Leave(int   iNextStateID) override;
  void ServerDisconnected() override;

  int ProcMouseInput(UINT    uiMsg, WPARAM wParam, LPARAM lParam) override;
  int ProcKeyboardInput(UINT uiMsg, WPARAM wParam, LPARAM lParam) override;
private:
  int m_iPrevState;
};
#endif
