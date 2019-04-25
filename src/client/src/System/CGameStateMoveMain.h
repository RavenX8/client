#ifndef _CGAMESTATEMOVEMAIN_
#define _CGAMESTATEMOVEMAIN_

#include "CGameState.h"

/**
* 캐릭터 선택후 게임에 들어가기전 카메라 이동시의 State Class
*	- Next State : CGameStatePrepareMain
*
* @Author		최종진
* @Date			2005/9/15
*/
class CGameStateMoveMain : public CGameState {
public:
  CGameStateMoveMain(int  iID);
  ~CGameStateMoveMain(void);

  int Update(bool bLostFocus) override;
  int Enter(int   iPrevStateID) override;
  int Leave(int   iNextStateID) override;

  ///CGame의 Msg Queue에 넣기 전에 즉시 처리해야할 Msg를 처리하는 Method
  ///각 State는 Method를 Overriding하여 처리해야할 Message를 처리한다
  int ProcWndMsgInstant(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;

  void ServerDisconnected() override;

  int ProcMouseInput(UINT    uiMsg, WPARAM wParam, LPARAM lParam) override;
  int ProcKeyboardInput(UINT uiMsg, WPARAM wParam, LPARAM lParam) override;

};
#endif
