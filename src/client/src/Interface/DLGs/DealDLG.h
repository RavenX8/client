#ifndef _DEAL_DLG_H
#define _DEAL_DLG_H

#include "tgamectrl/TDialog.h"
#include "../../GameCommon/IObserver.h"
#include "SubClass/CSlot.h"
#include "../../GameData/CDealData.h"

class CDragItem;
class CTCommand;

/**
* NPC�������� �ŷ��� �������� ������ �Ѳ����� ��ų� �ȶ� ����ϴ� ���̾�α�( ����Ʈ�� ���� ��� )
*	- Observable : CDealData
* 
* @Author		������
* @Date			2005/9/14
*/
class CDealDLG : public CTDialog, public IObserver {
private:
  CSlot      m_Slots[MAX_DEAL_SELLBUY][ TOTAL_DEAL_INVENTORY ];
  CDragItem* m_pBuyDragItem;
  CDragItem* m_pSellDragItem;

  CTCommand* m_pCmdRemoveItemFromBuyList;
  CTCommand* m_pCmdRemoveItemFromSellList;
public:
          CDealDLG(int iType);
  virtual ~CDealDLG();

  void Draw() override;
  void Hide() override;
  void MoveWindow(POINT pt) override;
  void Update(POINT     ptMouse) override;

  unsigned int Process(UINT uiMsg, WPARAM wParam, LPARAM lParam) override;

  void Update(CObservable* pObservable, CTObject* pObj) override;

  enum dealDLG {
    DEAL_BTN_CLOSE = 10,
    //1.â�ݱ� 	
    DEAL_BTN_OK,
    //2.�ŷ�Ȯ�� 
  };

private:
  bool On_LButtonUP(unsigned ProcID, WPARAM wParam, LPARAM lParam);

};

#endif