#ifndef _CSLOT_
#define _CSLOT_

#include "tgamectrl/WinCtrl.h"

class CIcon;
class CTCommand;
class CDragItem;

/**
*  CIcon�� Attach�Ͽ� ������ �ִ� Class
*	- �ѹ��� �Ѱ����� ������ �ִ�.
*	- Detach�ÿ� Icon�� delete�Ѵ�.
*	- CTCommand�� ������ �ִ�.
*	- CDragItem�� ������ Icon�� Drag&Drop�� ���۽�Ų��
*
* @Author	������
* @Date		2005/9/15
*/
//-------------------------------------------------------------------------------------------
class CSlot : public CWinCtrl {
public:
                 CSlot(void        );
  virtual        ~CSlot(void       );
  virtual bool   AttachIcon(CIcon* pIcon);
  virtual void   DetachIcon();
  virtual CIcon* MoveIcon();

  void         Draw() override;
  unsigned int Process(UINT     uiMsg, WPARAM wParam, LPARAM lParam) override;
  void         MoveWindow(POINT pt) override; ///Parent Dialog�� Left-Top Point
  void         Update(POINT     ptMouse) override;

  CIcon* GetIcon() { return m_pIcon; }

  void SetParent(int iID);
  int  GetParent();
  void SetDragAvailable();
  void SetDragDisable();

  void       SetDragItem(CDragItem* pDragItem);
  CDragItem* GetDragItem();
  void       SetCommand(CTCommand* pCmd);
  void       ResetClicked();

  void SetTooltipType(DWORD dwType);
protected:
  void SetIconPosition(POINT pt);
  void SetPosition(POINT     pt);

protected:
  CIcon*     m_pIcon;
  int        m_iParentID;      /// Slot�� Parent ID( ����� TDialog�� ���� ..)
  bool       m_bDragAvailable; /// Slot�� Item�� ������ Drag�����Ѱ�?
  CDragItem* m_pDragItem;
  CTCommand* m_pCommand; /// Slot�� ���������쿡 ����Ǵ� Command
  bool       m_bClicked;
  POINT      m_ptClicked;
  DWORD      m_dwTooltipType;
};

/*
* ������ â�� Pat �ǿ��� ������ ������ �����ִ� ���� ����( ������� �ʰ� �ִ� )
*/
//class CSlotPatEquip : public CSlot
//{
//public:
//	CSlotPatEquip(void){}
//	virtual ~CSlotPatEquip(void){}
//	virtual unsigned int Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
//	virtual void Draw();
//};

/**
* CPrivateStoreDlg���� �����ǿ� ���Ǵ� Slot
*/
class CSlotBuyPrivateStore : public CSlot {
public:
               CSlotBuyPrivateStore(void );
  virtual      ~CSlotBuyPrivateStore(void) {}
  void         DetachIcon() override;
  void         Draw() override;
  unsigned int Process(UINT       uiMsg, WPARAM wParam, LPARAM lParam) override;
  void         SetExhibition(bool b);
  bool         IsExhibition();
  bool         IsSelected();
  void         SetSelected() override;
protected:
  bool                         m_bExhibition; ///���Ͽ��� �����ؼ� ���� ����Ʈ�� �÷��� �������� ������ �ִ°�?
  static CSlotBuyPrivateStore* s_pSelectedSlot;

};
#endif