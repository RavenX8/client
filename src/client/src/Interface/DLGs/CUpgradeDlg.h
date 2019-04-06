#ifndef _CUpgradeDlg_
#define _CUpgradeDlg_
#include "tgamectrl/TDialog.h"
#include "subclass/CSlot.h"
#include "../../GameCommon/IObserver.h"

class CDragItem;
class CUpgradeDlgState;
class CUpgradeDlgStateNormal;
class CUpgradeDlgStateWait;
class CUpgradeDlgStateResult;

/**
* 제련에 사용되는 다이얼로그
*	- Observable : CUpgrade
* 
* @Author		최종진
* @Date			2005/9/14
*/
class CUpgradeDlg : public CTDialog, public IObserver {
  friend CUpgradeDlgStateNormal;
  friend CUpgradeDlgStateWait;
  friend CUpgradeDlgStateResult;
public:
          CUpgradeDlg(int  iType);
  virtual ~CUpgradeDlg(void);

  void     Update(POINT ptMouse) override;
  void     Show() override;
  void     Hide() override;
  void     Draw() override;
  void     MoveWindow(POINT pt) override;
  unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;

  void Update(CObservable* pObservable, CTObject* pObj) override;

  void ChangeState(int iID);

private:

  enum {
    IID_TEXT_COST = 5
  };

  enum STATE_TYPE {
    STATE_NORMAL,
    STATE_WAIT,
    STATE_RESULT,
    STATE_MAX
  };

  CUpgradeDlgState* m_pCurrState;
  CUpgradeDlgState* m_pStates[STATE_MAX];

  CDragItem* m_pDragItemTarget;
  CDragItem* m_pDragItemMaterial;

  CSlot m_TargetItemSlot;
  CSlot m_MaterialSlots[3];
};
#endif
