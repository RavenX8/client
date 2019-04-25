#ifndef _EXCHANGEDLG_
#define _EXCHANGEDLG_

#include "tgamectrl/TDialog.h"
#include "../../GameCommon/IObserver.h"
#include "../../GameData/CExchange.h"
#include "subclass/cslot.h"

class CObservable;
class CTObject;
class CTCmdNumberInput;
class CTCommand;
class CDragItem;

/**
* 다른 유저와 1:1거래시 사용되는 다이얼로그
*	- 사기를 막기 위해 둘다 준비버튼을 눌렀을때 확인 버튼이 활성화 되는 2중 확인을 한다.
*	- Observable : CExchange
*
* @Author		최종진
* @Date			2005/9/14
*/

class CExchangeDLG : public CTDialog, public IObserver {
public:
          CExchangeDLG(int  iType);
  virtual ~CExchangeDLG(void);

  void         Draw() override;
  void         Show() override;
  unsigned int Process(UINT uiMsg, WPARAM wParam, LPARAM lParam) override;

  void Hide() override;
  int  IsInValidShow() override;
  void MoveWindow(POINT pt) override;
  void Update(POINT     ptMouse) override;

  void Update(CObservable* pObservable, CTObject* pObj) override;

  enum {
    IID_BTN_CLOSE = 10,
    IID_BTN_EXCHANGE = 11,
    IID_BTN_OK = 12,
    IID_BTN_OTHER_CONFIRM = 13,
    IID_BTN_MONEY = 14
  };

protected:
  void OnLButtonDown(unsigned iProcID, WPARAM wParam, LPARAM lParam);
  void OnLButtonUp(unsigned   iProcID, WPARAM wParam, LPARAM lParam);

  void InitExchangeState();
  void SetOtherReadyState(bool b);
  void SetMeReadyState(bool    b);

protected:
  CSlot m_MySlots[TOTAL_EXCHANGE_INVENTORY];
  CSlot m_OtherSlots[TOTAL_EXCHANGE_INVENTORY];

  CTCommand*        m_pCmdRemoveMyItemFromExchange;
  CTCmdNumberInput* m_pCmdRemoveMyMoneyFromExchange;
  CDragItem*        m_pDragItem;
  CDragItem*        m_pOtherItemDragItem;

};

#endif // _EXCHANGEDLG_	
