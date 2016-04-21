
#include "StdAfx.h"
#include "QuickToolBAR.h"
#include "../it_mgr.h"
#include "../CDragItem.h"
#include "../CTDrawImpl.h"
#include "../IO_ImageRes.h"
#include "../../GameCommon/Skill.h"	

#include "../../OBJECT.h"
#include "../../Network/CNetwork.h"
#include "../../Common/IO_STB.h"


#include "../../System/CGame.h"
#include "../../JCommandState.h"	
#include "../../GameCommon/CSkillCommand.h"
#include "../../gamecommon/item.h"
#include "../Icon/CIconQuick.h"
#include "../Icon/CIconItem.h"
#include "../command/dragcommand.h"

#include "../../GameData/Event/CTEventItem.h"
#include "../../Country.h"

#include "TPane.h"
#include "TEditBox.h"
#include "TImage.h"
#include "ResourceMgr.h"
#include "TControlMgr.h"

//#define TYPE1_BODY_SIZE		4	//몸통
//#define TYPE1_TAIL_SIZE		33	//슬롯박스 크기 
//#define TYPE2_BODY_SIZE		30
//#define TYPE2_TAIL_SIZE		33

#ifdef _NEWUI

CQuickBAR::CQuickBAR( int iType )
{
	SetDialogType( iType );
	m_nCurrentPage	= 0;
	m_nType			= TYPE_HORIZONTAL;
	m_nCurrentPage	= 0;	

	m_pDragItem = new CDragItem;
	CTCommand* pCmd = new CTCmdDragItemFromQuickBar;
	m_pDragItem->AddTarget( CDragItem::TARGET_ALL, pCmd );
	pCmd = new CTCmdMoveIconInQuickBar;
	m_pDragItem->AddTarget( DLG_TYPE_QUICKBAR , pCmd );

	m_pCmdDragItemFromQuickBar = new CTCmdDragItemFromQuickBar;

	for( int i = 0 ; i < HOT_ICONS_PER_PAGE; ++i )
	{
		m_QuickSlot[i].SetParent( iType );
		m_QuickSlot[i].SetDragAvailable();
		m_QuickSlot[i].SetDragItem( m_pDragItem );		
		m_QuickSlot[i].SetControlID( IID_SLOT + i );
		this->Add( &m_QuickSlot[i] );		
	}
}

CQuickBAR::~CQuickBAR()
{	
	if( m_pDragItem )
	{
		delete m_pDragItem;
		m_pDragItem = NULL;
	}
}

void CQuickBAR::Show()
{
	CTDialog::Show();
	ChangeType( m_nType );
}

void CQuickBAR::Draw()
{
	if( !IsVision() ) return;

	CTDialog::Draw();

	/// 게이지 진행상태를 보여주기 위해서..
	::endSprite();
	::beginSprite( D3DXSPRITE_ALPHABLEND ); 

	/// quick slot rendering
	DrawQuickSlot();
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief 각 CSlot 들의 MoveWindow 호출..
//----------------------------------------------------------------------------------------------------

void CQuickBAR::MoveWindow( POINT pt )
{
	/// offset 먼저 설정..
	UpdateCSlotPosition();

	CTDialog::MoveWindow( pt );
}


/// quick slot rendering
void CQuickBAR::DrawQuickSlot()
{
}

//----------------------------------------------------------------------------------------------------
/// @param PINT ptMouse
/// @brief	매프레임 호출..
//----------------------------------------------------------------------------------------------------

void CQuickBAR::Update( POINT ptMouse )
{
	if( !IsVision() ) return;

	CTDialog::Update( ptMouse );
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief 유져 이벤트( 마우스, 키보드.. ) 처리..
//----------------------------------------------------------------------------------------------------

unsigned int CQuickBAR::Process( UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uiMsg )
	{
	case WM_SYSKEYDOWN :
		switch( wParam )
		{
		case '1':
			m_nCurrentPage = 0;
			UpdateHotIconSlot();
			return uiMsg;
			break;
		case '2':
		case VK_F10:
			m_nCurrentPage = 1;
			UpdateHotIconSlot();
			return uiMsg;
		case '3':
			m_nCurrentPage = 2;
			UpdateHotIconSlot();
			return uiMsg;
		case '4':
			m_nCurrentPage = 3;
			UpdateHotIconSlot();
			return uiMsg;

		default:
			break;
		}
		break;
	case WM_KEYUP:
		{
			switch ( wParam ) 
			{
			case '1':
			case '2':
			case '3':
			case '4':
				{
					if( CCountry::GetSingleton().IsApplyNewVersion() && CTControlMgr::GetInstance()->GetKeyboardInputType() == CTControlMgr::INPUTTYPE_NORMAL && NULL == CTEditBox::s_pFocusEdit )
					{
						m_nCurrentPage = wParam - '1';
						UpdateHotIconSlot();
						return uiMsg;
					}
				}
				break;
			case VK_F1:
			case VK_F2:
			case VK_F3:
			case VK_F4:
			case VK_F5:
			case VK_F6:
			case VK_F7:
			case VK_F8:
				{
					CIcon* pIcon = m_QuickSlot[ wParam - VK_F1 ].GetIcon();			
					if( pIcon )
						pIcon->ExecuteCommand();
					return uiMsg;
				}
			case VK_F9:
				m_nCurrentPage = 0;
				UpdateHotIconSlot();
				return uiMsg;
			case VK_F11:
				m_nCurrentPage = 2;
				UpdateHotIconSlot();
				return uiMsg;
			case VK_F12:
				m_nCurrentPage = 3;
				UpdateHotIconSlot();
				return uiMsg;

			default:
				break;
			}
			break;
		}
	default:
		break;
	}


	
	if(!IsVision()) return 0;

	unsigned iProcID = 0;
	if( iProcID = CTDialog::Process(uiMsg,wParam,lParam) )
	{
		switch(uiMsg)
		{
		case WM_LBUTTONUP:
			On_LButtonUP( iProcID, wParam, lParam );
			break;
		default:
			break;
		}
		return uiMsg;
	}
	
	return 0;
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief update cslot of hoticon
//----------------------------------------------------------------------------------------------------
void CQuickBAR::UpdateHotIconSlot()
{
	for( int i = 0 ; i < HOT_ICONS_PER_PAGE; i++ )
	{
		m_QuickSlot[ i ].DetachIcon();

		CHotIconSlot* pHotIconSlot	= g_pAVATAR->GetHotIconSlot();
		CIcon* pIcon				= pHotIconSlot->GetHotIcon( m_nCurrentPage * HOT_ICONS_PER_PAGE + i );

		m_QuickSlot[ i ].AttachIcon( pIcon );
	}
	
	UpdateCSlotPosition();
	

	
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief 현재의 상태에 따라 각 Slot 객체들의 위치를 갱신한다.
//----------------------------------------------------------------------------------------------------

void CQuickBAR::UpdateCSlotPosition()
{
	POINT pt = { 0, 0 };

	if( m_nType == TYPE_HORIZONTAL )
	{
		pt.y = 2; 
		for( int i = 0 ; i < HOT_ICONS_PER_PAGE ; ++i )
		{			
			pt.x = 21 + i * 42;
			m_QuickSlot[i].SetOffset( pt );
		}
	}
	else
	{
		pt.x = 2;
		for( int i = 0; i < HOT_ICONS_PER_PAGE; ++i )
		{			
            pt.y = 21 + i * 42;
			m_QuickSlot[i].SetOffset( pt );
		}
	}

	CWinCtrl * pCtrl = NULL;

	if( m_nType == TYPE_HORIZONTAL )
	{
		pCtrl = Find( "HORIZONTAL" );
	}
	else
	{
		pCtrl = Find( "VERTICAL" );
	}

	if( pCtrl == NULL )	return;

	///현재 페이지번호를 바꾸어 준다.
	CWinCtrl* pCtrl1	= pCtrl->Find( "1" );
	CWinCtrl* pCtrl2	= pCtrl->Find( "2" );
	CWinCtrl* pCtrl3	= pCtrl->Find( "3" );
	CWinCtrl* pCtrl4	= pCtrl->Find( "4" );	

	if( pCtrl1 && pCtrl2 && pCtrl3 && pCtrl4 )
	{
		pCtrl1->Hide();	pCtrl2->Hide();	pCtrl3->Hide();	pCtrl4->Hide();

		switch( m_nCurrentPage )
		{
		case 0:
			pCtrl1->Show();
			break;
		case 1:
			pCtrl2->Show();
			break;
		case 2:
			pCtrl3->Show();
			break;
		case 3:
			pCtrl4->Show();
			break;
		}
	}
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief 현재 선택된 퀵 슬롯을 얻는다. 절대위치 슬롯번호이다.
//----------------------------------------------------------------------------------------------------

short CQuickBAR::GetMouseClickSlot( POINT& ptMouse )
{
	/// 현재 페이지에 등록된 슬롯들을 돌며 체크
	for( int i = 0 ; i < HOT_ICONS_PER_PAGE; i++ )
	{
		if( m_QuickSlot[ i ].IsInside( ptMouse.x, ptMouse.y ) == true )
		{
			return m_nCurrentPage * HOT_ICONS_PER_PAGE + i;
		}
	}

	return -1;
}
//----------------------------------------------------------------------------------------------------
/// @param
/// @brief process each child control..
//----------------------------------------------------------------------------------------------------

bool CQuickBAR::On_LButtonUP( unsigned iProcID, WPARAM wParam, LPARAM lParam )
{	
	switch( iProcID )
	{
	case IID_BTN_HORIZONTAL_PREV:
	case IID_BTN_VERTICAL_PREV:
		{
			--m_nCurrentPage;
			if( m_nCurrentPage < 0 )
				m_nCurrentPage = MAX_ICONS_PAGES -1;

			UpdateHotIconSlot();
			break;
		}
	case IID_BTN_HORIZONTAL_NEXT:
	case IID_BTN_VERTICAL_NEXT:
		{
			m_nCurrentPage++;

			if( m_nCurrentPage >= MAX_ICONS_PAGES ) 
				m_nCurrentPage = 0;

			UpdateHotIconSlot();
		}
		return true;
	case IID_BTN_ROTATE:
		{
			if( m_nType == TYPE_HORIZONTAL )
				ChangeType( TYPE_VERTICAL );
			else
				ChangeType( TYPE_HORIZONTAL );

			UpdateCSlotPosition();	

			break;
		}
	default:
		break;
	}

	

	return false;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief Change quick bar type ( vertical <-> horizontal 
/// @todo  Number Image Offset, Rotate Button Offset Change
//----------------------------------------------------------------------------------------------------

void CQuickBAR::ChangeType( int iType , bool bInit )
{
	RECT rc = {0,0,0,0};
	CWinCtrl * ctrl = NULL;

	if( iType == TYPE_HORIZONTAL )
	{
		ShowChild( "HORIZONTAL" );
		HideChild( "VERTICAL");
		ctrl = Find( "HORIZONTAL" );			
	}
	else
	{
		HideChild( "HORIZONTAL" );
		ShowChild( "VERTICAL");
		ctrl = Find( "VERTICAL" );		
	}

	if( ctrl )
	{
		//dlg size.
		SetHeight( ctrl->GetHeight() );
		SetWidth( ctrl->GetWidth() );

		//dlg caption.
		SetRect( &rc, 0, 0, ctrl->GetWidth(), ctrl->GetHeight() );
		ChangeCaptionRect( rc );		
	}

	POINT pt = GetPosition();
	if( pt.x < 0 ) pt.x = 1;
	if( pt.y < 0 ) pt.y = 1;
	if( pt.x + GetWidth()  > g_pCApp->GetWIDTH() )
		pt.x = g_pCApp->GetWIDTH() - GetWidth();

	if( pt.y + GetHeight()  > g_pCApp->GetHEIGHT() )
		pt.y = g_pCApp->GetHEIGHT() - GetHeight();

	m_nType = iType;
	MoveWindow( pt );

}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief CItemSlot이 변경되어질때마다 호출되는 Method : 추후 CSkillSlot에서도 호출되어질수 있다( Skill이 없어지는 경우가 생긴다면 )
//----------------------------------------------------------------------------------------------------
void CQuickBAR::Update( CObservable* pObservable, CTObject* pObj )
{
	////등록된 아이콘중 아이템이고 위와 같은것일경우 지운다.
	assert( pObservable );

	if( pObj && strcmp( pObj->toString(), "CTEventItem" ) == 0 )
	{
		CTEventItem* pEvent = (CTEventItem*)pObj;

		if( pEvent->GetID() == CTEventItem::EID_DEL_ITEM )///지워지는것만 처리하면된다.
		{
			CIcon* pIcon			= NULL;
			CIconItem* pItemIcon	= NULL;
			CIconQuick* pQuickIcon	= NULL;	

			int iIndex = pEvent->GetIndex();

			for( int i = 0 ; i < HOT_ICONS_PER_PAGE; ++i )
			{
				if( g_pAVATAR->m_HotICONS.m_IconPAGE[ m_nCurrentPage ][ i ].m_cType == 1 )
				{
					if( pIcon = m_QuickSlot[i].GetIcon() )
					{
						pQuickIcon = (CIconQuick*)pIcon;
						pItemIcon  = (CIconItem*)pQuickIcon->GetIcon();
						///인벤토리 인덱스가 같고 지워지는 아이템일경우
						///같은 아이템이 2군데 등록되어 있을수 있다.
						if( pItemIcon->GetIndex() == iIndex  )
						{
							/// Detach시에 pQuickIcon은 파괴된다.( Detach전에 Command실행할것 )
							m_pCmdDragItemFromQuickBar->Exec( pQuickIcon );					
							m_QuickSlot[i].DetachIcon();
						}
					}
				}
			}
		}
	}
	else
	{
		assert( 0 && "CTEvent is NULL or Invalid Type@CQuickBAR::Update" );
	}
}

//*-----------------------------------------------------------------------------------------------------/
/// 서버에 패킷 보내지 않고 클라이언트에서만 Data를 삭제할 경우
/// 게임내에서 캐릭터 선택창으로 이동시 
//*-----------------------------------------------------------------------------------------------------/
void CQuickBAR::Clear()
{
	for( int i = 0; i < HOT_ICONS_PER_PAGE; ++i )
		m_QuickSlot[ i ].DetachIcon();
}

void CQuickBAR::SetInterfacePos_After()
{	
	for( int i = 0 ; i < HOT_ICONS_PER_PAGE; ++i )
	{
		MoveCtrl2ListEnd( m_QuickSlot[i].GetControlID() );		
	}	
}

void CQuickBAR::SetQuickBarType( short nType )
{
	m_nType = nType;
}




#else		// old ui~~~




CQuickBAR::CQuickBAR( int iType )
{
	SetDialogType( iType );
	m_nCurrentPage	= 0;
	m_nType			= TYPE_HORIZONTAL;
	m_nCurrentPage	= 0;	

	m_pDragItem = new CDragItem;
	CTCommand* pCmd = new CTCmdDragItemFromQuickBar;
	m_pDragItem->AddTarget( CDragItem::TARGET_ALL, pCmd );
	pCmd = new CTCmdMoveIconInQuickBar;
	m_pDragItem->AddTarget( DLG_TYPE_QUICKBAR , pCmd );

	m_pCmdDragItemFromQuickBar = new CTCmdDragItemFromQuickBar;



	for( int i = 0 ; i < HOT_ICONS_PER_PAGE; ++i )
	{
		m_QuickSlot[i].SetParent( iType );
		m_QuickSlot[i].SetDragAvailable();
		m_QuickSlot[i].SetDragItem( m_pDragItem );


	}
}

CQuickBAR::~CQuickBAR()
{	
	if( m_pDragItem )
	{
		delete m_pDragItem;
		m_pDragItem = NULL;
	}
}

void CQuickBAR::Show()
{
	CTDialog::Show();
	ChangeType( m_nType );
}

void CQuickBAR::Draw()
{
	if( !IsVision() ) return;

	CTDialog::Draw();

	/// 게이지 진행상태를 보여주기 위해서..
	::endSprite();
	::beginSprite( D3DXSPRITE_ALPHABLEND ); 

	/// quick slot rendering
	DrawQuickSlot();

	//	DrawItemInfo();
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief 각 CSlot 들의 MoveWindow 호출..
//----------------------------------------------------------------------------------------------------

void CQuickBAR::MoveWindow( POINT pt )
{
	CTDialog::MoveWindow( pt );

	/// offset 먼저 설정..
	UpdateCSlotPosition();

	for( int i = 0 ; i < HOT_ICONS_PER_PAGE; i++ )
	{
		m_QuickSlot[ i ].MoveWindow( this->m_sPosition );
	}	
}


/// quick slot rendering
void CQuickBAR::DrawQuickSlot()
{
	for( int i = 0 ; i < HOT_ICONS_PER_PAGE; i++ )
	{
		m_QuickSlot[ i ].Draw();
	}
}

//----------------------------------------------------------------------------------------------------
/// @param PINT ptMouse
/// @brief	매프레임 호출..
//----------------------------------------------------------------------------------------------------

void CQuickBAR::Update( POINT ptMouse )
{
	if( !IsVision() ) return;

	CTDialog::Update( ptMouse );

	for( int i = 0 ; i < HOT_ICONS_PER_PAGE; ++i )
		m_QuickSlot[i].Update( ptMouse );
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief 유져 이벤트( 마우스, 키보드.. ) 처리..
//----------------------------------------------------------------------------------------------------

unsigned int CQuickBAR::Process( UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uiMsg )
	{
	case WM_SYSKEYDOWN :
		switch( wParam )
		{
		case '1':
			m_nCurrentPage = 0;
			UpdateHotIconSlot();
			return uiMsg;
			break;
		case '2':
		case VK_F10:
			m_nCurrentPage = 1;
			UpdateHotIconSlot();
			return uiMsg;
		case '3':
			m_nCurrentPage = 2;
			UpdateHotIconSlot();
			return uiMsg;
		case '4':
			m_nCurrentPage = 3;
			UpdateHotIconSlot();
			return uiMsg;

		default:
			break;
		}
		break;
	case WM_KEYUP:
		{
			switch ( wParam ) 
			{
			case '1':
			case '2':
			case '3':
			case '4':
				{
					if( CCountry::GetSingleton().IsApplyNewVersion() && CTControlMgr::GetInstance()->GetKeyboardInputType() == CTControlMgr::INPUTTYPE_NORMAL && NULL == CTEditBox::s_pFocusEdit )
					{
						m_nCurrentPage = wParam - '1';
						UpdateHotIconSlot();
						return uiMsg;
					}
				}
				break;
			case VK_F1:
			case VK_F2:
			case VK_F3:
			case VK_F4:
			case VK_F5:
			case VK_F6:
			case VK_F7:
			case VK_F8:
				{
					CIcon* pIcon = m_QuickSlot[ wParam - VK_F1 ].GetIcon();			
					if( pIcon )
						pIcon->ExecuteCommand();
					return uiMsg;
				}
			case VK_F9:
				m_nCurrentPage = 0;
				UpdateHotIconSlot();
				return uiMsg;
			case VK_F11:
				m_nCurrentPage = 2;
				UpdateHotIconSlot();
				return uiMsg;
			case VK_F12:
				m_nCurrentPage = 3;
				UpdateHotIconSlot();
				return uiMsg;

			default:
				break;
			}
			break;
		}
	default:
		break;
	}



	if(!IsVision()) return 0;

	unsigned iProcID = 0;
	if( iProcID = CTDialog::Process(uiMsg,wParam,lParam) )
	{
		for( int i  = 0; i < HOT_ICONS_PER_PAGE; ++i )
		{
			if( m_QuickSlot[i].Process( uiMsg, wParam, lParam ) )
				return uiMsg;
		}
		switch(uiMsg)
		{
		case WM_LBUTTONUP:
			On_LButtonUP( iProcID, wParam, lParam );
			break;
		default:
			break;
		}
		return uiMsg;
	}

	return 0;
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief update cslot of hoticon
//----------------------------------------------------------------------------------------------------
void CQuickBAR::UpdateHotIconSlot()
{
	for( int i = 0 ; i < HOT_ICONS_PER_PAGE; i++ )
	{
		m_QuickSlot[ i ].DetachIcon();

		CHotIconSlot* pHotIconSlot	= g_pAVATAR->GetHotIconSlot();
		CIcon* pIcon				= pHotIconSlot->GetHotIcon( m_nCurrentPage * HOT_ICONS_PER_PAGE + i );

		m_QuickSlot[ i ].AttachIcon( pIcon );
	}


	UpdateCSlotPosition();

	///현재 페이지번호를 바꾸어 준다.	

	CWinCtrl* pCtrl = Find( IID_NUMBER );
	if( pCtrl )
	{
		CTImage* pImage = (CTImage*)pCtrl;
		int iGraphicID = 0;
		int iModuleID  = IMAGE_RES_UI;
		switch( m_nCurrentPage )
		{
		case 0:
			iGraphicID = CResourceMgr::GetInstance()->GetImageNID( iModuleID, "UI21_NUMBER_1");
			break;
		case 1:
			iGraphicID = CResourceMgr::GetInstance()->GetImageNID( iModuleID, "UI21_NUMBER_2");
			break;
		case 2:
			iGraphicID = CResourceMgr::GetInstance()->GetImageNID( iModuleID, "UI21_NUMBER_3");
			break;
		case 3:
			iGraphicID = CResourceMgr::GetInstance()->GetImageNID( iModuleID, "UI21_NUMBER_4");
			break;
		default:
			break;
		}
		pImage->SetImage(iGraphicID, iModuleID );
	}
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief 현재의 상태에 따라 각 Slot 객체들의 위치를 갱신한다.
//----------------------------------------------------------------------------------------------------

void CQuickBAR::UpdateCSlotPosition()
{
	POINT pt = { 0, 0 };

	if( m_nType == TYPE_HORIZONTAL )
	{
		pt.x = 40; pt.y = 21;
		for( int i = 0 ; i < HOT_ICONS_PER_PAGE ; ++i )
		{
			pt.x = 40 + ( m_QuickSlot[0].GetWidth() + 1 ) * i + ( 2 * i / HOT_ICONS_PER_PAGE ) * 9;
			m_QuickSlot[i].SetOffset( pt );
		}
	}
	else
	{
		pt.x = 3; pt.y = 40;
		for( int i = 0; i < HOT_ICONS_PER_PAGE; ++i )






























		{
			pt.y = 40 + (m_QuickSlot[0].GetHeight() + 1) * i + ( 2 * i / HOT_ICONS_PER_PAGE ) * 10;
			m_QuickSlot[i].SetOffset( pt );










		}
	}
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief 현재 선택된 퀵 슬롯을 얻는다. 절대위치 슬롯번호이다.
//----------------------------------------------------------------------------------------------------

short CQuickBAR::GetMouseClickSlot( POINT& ptMouse )
{
	/// 현재 페이지에 등록된 슬롯들을 돌며 체크
	for( int i = 0 ; i < HOT_ICONS_PER_PAGE; i++ )
	{
		if( m_QuickSlot[ i ].IsInside( ptMouse.x, ptMouse.y ) == true )
		{
			return m_nCurrentPage * HOT_ICONS_PER_PAGE + i;
		}
	}

	return -1;
}
//----------------------------------------------------------------------------------------------------
/// @param
/// @brief process each child control..
//----------------------------------------------------------------------------------------------------

bool CQuickBAR::On_LButtonUP( unsigned iProcID, WPARAM wParam, LPARAM lParam )
{	
	switch( iProcID )
	{
	case IID_BTN_HORIZONTAL_PREV:
	case IID_BTN_VERTICAL_PREV:
		{
			--m_nCurrentPage;
			if( m_nCurrentPage < 0 )
				m_nCurrentPage = MAX_ICONS_PAGES -1;

			UpdateHotIconSlot();
			break;
		}
	case IID_BTN_HORIZONTAL_NEXT:
	case IID_BTN_VERTICAL_NEXT:
		{
			m_nCurrentPage++;

			if( m_nCurrentPage >= MAX_ICONS_PAGES ) 
				m_nCurrentPage = 0;

			UpdateHotIconSlot();
		}
		return true;
	case IID_BTN_ROTATE:
		{

			if( m_nType == TYPE_HORIZONTAL )
				ChangeType( TYPE_VERTICAL );
			else
				ChangeType( TYPE_HORIZONTAL );

			UpdateCSlotPosition();	

			break;
		}
	default:
		break;
	}



	return false;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief Change quick bar type ( vertical <-> horizontal 
/// @todo  Number Image Offset, Rotate Button Offset Change
//----------------------------------------------------------------------------------------------------

void CQuickBAR::ChangeType( int iType , bool bInit )
{
	RECT rc = {0,0,0,0};


	if( iType == TYPE_HORIZONTAL )
	{
		ShowChild( IID_BG_HORIZONTAL );
		ShowChild( IID_BTN_HORIZONTAL_PREV );
		ShowChild( IID_BTN_HORIZONTAL_NEXT );
		HideChild( IID_BG_VERTICAL );
		HideChild( IID_BTN_VERTICAL_PREV );
		HideChild( IID_BTN_VERTICAL_NEXT );

		POINT pt = GetPosition();

		int iWidth = GetWidth();
		int iHeight = GetHeight();
		if( iType != m_nType )
		{
			if( !bInit )
			{
				pt.x += GetWidth() / 2 - GetHeight() / 2 ;
				pt.y += GetHeight() / 2 - GetWidth() / 2;
			}

			SetHeight( iWidth );
			SetWidth( iHeight );
		}			

		SetRect( &rc,0,0,18,63 );
		ChangeCaptionRect( rc );

		if( pt.x < 0 ) pt.x = 1;
		if( pt.y < 0 ) pt.y = 1;
		if( pt.x + GetWidth()  > g_pCApp->GetWIDTH() )
			pt.x = g_pCApp->GetWIDTH() - GetWidth();

		if( pt.y + GetHeight()  > g_pCApp->GetHEIGHT() )
			pt.y = g_pCApp->GetHEIGHT() - GetHeight();

		/// 타입을 먼저 바꾼후에 MoveWindow 호출 04/5/31 jeddli
		m_nType = iType;	


		CWinCtrl* pCtrl = Find( IID_BTN_ROTATE );
		if( pCtrl )
			pCtrl->SetOffset( 377, 27);

		pCtrl = Find( IID_NUMBER );
		if( pCtrl )
			pCtrl->SetOffset( 19, 24 );


		MoveWindow( pt );
	}
	else
	{
		HideChild( IID_BG_HORIZONTAL );
		HideChild( IID_BTN_HORIZONTAL_PREV );
		HideChild( IID_BTN_HORIZONTAL_NEXT );
		ShowChild( IID_BG_VERTICAL );
		ShowChild( IID_BTN_VERTICAL_PREV );
		ShowChild( IID_BTN_VERTICAL_NEXT );

		POINT pt = GetPosition();
		int iWidth = GetWidth();
		int iHeight = GetHeight();
		if( iType != m_nType )
		{
			if( !bInit )
			{
				pt.x += GetWidth() / 2 - GetHeight() / 2 ;
				pt.y += GetHeight() / 2 - GetWidth() / 2;
			}
			SetHeight( iWidth );
			SetWidth( iHeight );
		}			

		SetRect( &rc,1,1, 63 ,18 );
		ChangeCaptionRect( rc );

		if( pt.x < 0 ) pt.x = 1;
		if( pt.y < 0 ) pt.y = 1;
		if( pt.x + GetWidth()  > g_pCApp->GetWIDTH() )
			pt.x = g_pCApp->GetWIDTH() - GetWidth();


		if( pt.y + GetHeight()  > g_pCApp->GetHEIGHT() )
			pt.y = g_pCApp->GetHEIGHT() - GetHeight();



		/// 타입을 먼저 바꾼후에 MoveWindow 호출 04/5/31 jeddli
		m_nType = iType;




		CWinCtrl* pCtrl = Find( IID_BTN_ROTATE );
		if( pCtrl )
			pCtrl->SetOffset( 17, 377);

		pCtrl = Find( IID_NUMBER );
		if( pCtrl )
			pCtrl->SetOffset( 21, 20 );


		MoveWindow( pt );
	}
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief CItemSlot이 변경되어질때마다 호출되는 Method : 추후 CSkillSlot에서도 호출되어질수 있다( Skill이 없어지는 경우가 생긴다면 )
//----------------------------------------------------------------------------------------------------
void CQuickBAR::Update( CObservable* pObservable, CTObject* pObj )
{
	////등록된 아이콘중 아이템이고 위와 같은것일경우 지운다.
	assert( pObservable );

	if( pObj && strcmp( pObj->toString(), "CTEventItem" ) == 0 )
	{
		CTEventItem* pEvent = (CTEventItem*)pObj;

		if( pEvent->GetID() == CTEventItem::EID_DEL_ITEM )///지워지는것만 처리하면된다.
		{
			CIcon* pIcon			= NULL;
			CIconItem* pItemIcon	= NULL;
			CIconQuick* pQuickIcon	= NULL;	

			int iIndex = pEvent->GetIndex();

			for( int i = 0 ; i < HOT_ICONS_PER_PAGE; ++i )
			{
				if( g_pAVATAR->m_HotICONS.m_IconPAGE[ m_nCurrentPage ][ i ].m_cType == 1 )
				{
					if( pIcon = m_QuickSlot[i].GetIcon() )
					{
						pQuickIcon = (CIconQuick*)pIcon;
						pItemIcon  = (CIconItem*)pQuickIcon->GetIcon();
						///인벤토리 인덱스가 같고 지워지는 아이템일경우
						///같은 아이템이 2군데 등록되어 있을수 있다.
						if( pItemIcon->GetIndex() == iIndex  )
						{
							/// Detach시에 pQuickIcon은 파괴된다.( Detach전에 Command실행할것 )
							m_pCmdDragItemFromQuickBar->Exec( pQuickIcon );					
							m_QuickSlot[i].DetachIcon();
						}
					}
				}
			}
		}
	}
	else
	{
		assert( 0 && "CTEvent is NULL or Invalid Type@CQuickBAR::Update" );
	}
}

//*-----------------------------------------------------------------------------------------------------/
/// 서버에 패킷 보내지 않고 클라이언트에서만 Data를 삭제할 경우
/// 게임내에서 캐릭터 선택창으로 이동시 
//*-----------------------------------------------------------------------------------------------------/
void CQuickBAR::Clear()
{
	for( int i = 0; i < HOT_ICONS_PER_PAGE; ++i )
		m_QuickSlot[ i ].DetachIcon();
}


#endif