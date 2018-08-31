#ifndef _QUICK_TOOL_BAR_H
#define _QUICK_TOOL_BAR_H

#include "../../../TGameCtrl/TDialog.h"
#include "SubClass/CSlot.h"
#include "../Common/CHotIcon.h"
#include "../gamecommon/IObserver.h"

/**
* 스킬, 아이템등을 등록하여 단축키및 마우스클릭으로 사용할수 있는 다이얼로그
*	- Observable : CItemSlot
*
* @Author		이동현, 최종진
* @Date			2005/9/15
*/

#ifdef _NEWUI

class CTCmdDragItemFromQuickBar;
class CQuickBAR : public CTDialog, public IObserver
{
private:
	short			m_nType;
	short			m_nCurrentPage;
	short			m_nQuickBarType;


	//----------------------------------------------------------------------------------------------------
/// 한페이지만 보유한다.
	//----------------------------------------------------------------------------------------------------
	CSlot						m_QuickSlot[ HOT_ICONS_PER_PAGE ];
	CDragItem*					m_pDragItem;

	CTCmdDragItemFromQuickBar*	m_pCmdDragItemFromQuickBar;
public:
	CQuickBAR( int iType );
	virtual ~CQuickBAR();


	virtual void	Draw();
	virtual void	Show();
	virtual void	Update( POINT ptMouse );
	virtual	void	MoveWindow( POINT pt );
	virtual unsigned int Process(UINT uiMsg,WPARAM wParam,LPARAM lParam);


	virtual void Update( CObservable* pObservable, CTObject* pObj );
	virtual void SetInterfacePos_After();


	void Clear();
	//----------------------------------------------------------------------------------------------------
/// @brief update cslot of hoticon
	//----------------------------------------------------------------------------------------------------
	void		UpdateHotIconSlot();
	//----------------------------------------------------------------------------------------------------	
/// @brief 현재의 상태에 따라 각 Slot 객체들의 위치를 갱신한다.
	//----------------------------------------------------------------------------------------------------
	void		UpdateCSlotPosition();
	//----------------------------------------------------------------------------------------------------	
/// @brief 현재의 상태에 따라 각 Slot 객체들을 랜더링
	//----------------------------------------------------------------------------------------------------
	void		DrawQuickSlot();	
	//----------------------------------------------------------------------------------------------------	
/// @brief Key accelerator..
	//----------------------------------------------------------------------------------------------------

	enum{
		IID_BG_VERTICAL	  = 5,
		IID_BG_HORIZONTAL = 6,
		IID_BTN_ROTATE			= 10,
		IID_BTN_HORIZONTAL_PREV = 11,
		IID_BTN_HORIZONTAL_NEXT = 12,
		IID_BTN_VERTICAL_PREV	= 13,
		IID_BTN_VERTICAL_NEXT	= 14,
		IID_NUMBER				= 20,
		
		IID_SLOT				= 30,
	};

	enum{
		TYPE_VERTICAL,
		TYPE_HORIZONTAL
	};

	enum
	{
		QUICKBAR_TYPE_NORMAL = 0,
		QUICKBAR_TYPE_EXTENSION,
	};
	
	void  SetType( short nType ){ m_nType = nType; }
	int   GetType(){ return m_nType; }


	///내부적으로 불려질때와 저장된 Data에서 Type이 결정될때에 따라 내부 처리가 달라진다.
	void  ChangeType( int iType , bool bInit = false );

	//마우스가 클릭한 슬롯을 얻어온다 
	short GetMouseClickSlot( POINT& ptMouse );

	void SetQuickBarType( short nType );


protected:
	bool On_LButtonUP( unsigned iProcID, WPARAM wParam, LPARAM lParam );

};


class CQuickBAR_EXT : public CQuickBAR
{

};

#else

class CTCmdDragItemFromQuickBar;

class CQuickBAR : public CTDialog, public IObserver {
private:
  short m_nType;
  short m_nCurrentPage;

  //----------------------------------------------------------------------------------------------------
  /// 한페이지만 보유한다.
  //----------------------------------------------------------------------------------------------------
  CSlot      m_QuickSlot[ HOT_ICONS_PER_PAGE ];
  CDragItem* m_pDragItem;

  CTCmdDragItemFromQuickBar* m_pCmdDragItemFromQuickBar;
public:
          CQuickBAR(int iType);
  virtual ~CQuickBAR();

  void         Draw() override;
  void         Show() override;
  void         Update(POINT     ptMouse) override;
  void         MoveWindow(POINT pt) override;
  unsigned int Process(UINT     uiMsg, WPARAM wParam, LPARAM lParam) override;

  void Update(CObservable* pObservable, CTObject* pObj) override;

  void Clear();
  //----------------------------------------------------------------------------------------------------
  /// @brief update cslot of hoticon
  //----------------------------------------------------------------------------------------------------
  void UpdateHotIconSlot();
  //----------------------------------------------------------------------------------------------------	
  /// @brief 현재의 상태에 따라 각 Slot 객체들의 위치를 갱신한다.
  //----------------------------------------------------------------------------------------------------
  void UpdateCSlotPosition();
  //----------------------------------------------------------------------------------------------------	
  /// @brief 현재의 상태에 따라 각 Slot 객체들을 랜더링
  //----------------------------------------------------------------------------------------------------
  void DrawQuickSlot();

  //----------------------------------------------------------------------------------------------------	
  /// @brief Key accelerator..
  //----------------------------------------------------------------------------------------------------

  enum {
    IID_BG_VERTICAL = 5,
    IID_BG_HORIZONTAL = 6,
    IID_BTN_ROTATE = 10,
    IID_BTN_HORIZONTAL_PREV = 11,
    IID_BTN_HORIZONTAL_NEXT = 12,
    IID_BTN_VERTICAL_PREV = 13,
    IID_BTN_VERTICAL_NEXT = 14,
    IID_NUMBER = 20,

  };

  enum {
    TYPE_VERTICAL,
    TYPE_HORIZONTAL
  };

  void SetType(short nType) { m_nType = nType; }
  int  GetType() { return m_nType; }

  ///내부적으로 불려질때와 저장된 Data에서 Type이 결정될때에 따라 내부 처리가 달라진다.
  void ChangeType(int iType, bool bInit = false);

  //마우스가 클릭한 슬롯을 얻어온다 
  short GetMouseClickSlot(POINT& ptMouse);

protected:
  bool On_LButtonUP(unsigned iProcID, WPARAM wParam, LPARAM lParam);
};

#endif

#endif
