#include "StdAfx.h"
#include "./TargetManager.h"
#include "../CViewMSG.h"

#include "../Interface/CTDrawImpl.h"
#include "../Interface/IO_ImageRes.h"
#include "../Interface/it_mgr.h"
#include "../OBJECT.h"

#include "../JCommandState.h"
#include "tgamectrl/ResourceMgr.h"
#include "../Interface/CNameBox.h"

//CTargetManager _TargetManager;

CTargetManager::CTargetManager(void) {}

CTargetManager::~CTargetManager(void) {}

void CTargetManager::SetMouseTargetObject(int iObjectIDX) {
  m_iCurrentMouseTargetObject = iObjectIDX;
}

void CTargetManager::Proc() {
  if ( m_iCurrentMouseTargetObject ) {
    g_pViewMSG->AddObjIDX( m_iCurrentMouseTargetObject );
  }

  /// ���� ���õ� ������Ʈ�� �ְ�
  if ( g_UserInputSystem.GetCurrentTarget() ) {
    /// �װ��� ���̶��
    CObjCHAR* pObj = (CObjCHAR*)g_pObjMGR->Get_CharOBJ( g_UserInputSystem.GetCurrentTarget(), true );
    /// ��ȿ���� ���� Ÿ���̴�.. ���콺 �ĸǵ� �ʱ�ȭ
    if ( pObj != nullptr ) {
      if ( pObj->IsA( OBJ_MOB ) ) {
        g_pViewMSG->AddObjIDX( g_UserInputSystem.GetCurrentTarget() );
      }
    }
  }
}

void CTargetManager::Draw() {
  /// ���õ� Ÿ�� ���.
  //if ( g_UserInputSystem.GetCurrentTarget() ) 
  //{		
  //	
  //	CResourceMgr*   pResourceMgr	= CResourceMgr::GetInstance();
  //	CTDrawImpl* pDrawObj			= g_itMGR.GetDrawImplPtr();
  //	if( pDrawObj == NULL )
  //		return;

  //	CObjCHAR *pObj = (CObjCHAR*)g_pObjMGR->Get_CharOBJ( g_UserInputSystem.GetCurrentTarget(), true );
  //	/// ��ȿ���� ���� Ÿ���̴�.. ���콺 �ĸǵ� �ʱ�ȭ
  //	if( pObj == NULL )
  //	{
  //		g_UserInputSystem.ClearMouseState();
  //		return;
  //	}

  //	if( pObj->Get_TYPE() == OBJ_MOB ) 
  //	{
  //		int iCurrentHP = pObj->Get_HP();
  //		int iMaxHP = pObj->Get_MaxHP();
  //		
  //		if( iCurrentHP > iMaxHP )
  //		{
  //			iCurrentHP = iMaxHP;
  //			pObj->Set_HP( iCurrentHP );
  //		}
  //		
  //		if( iMaxHP != 0 )
  //		{
  //			D3DVECTOR   PosSCR;	
  //			pObj->GetScreenPOS (PosSCR);

  //			int iWidthBackImage = 116;
  //			int iWidthGuage		= 116;

  //			g_DrawImpl.Draw( PosSCR.x - iWidthBackImage / 2, PosSCR.y - NAMEBOX_HEIGHT / 2 + 4, PosSCR.z, IMAGE_RES_UI, CResourceMgr::GetInstance()->GetImageNID( IMAGE_RES_UI,"ID_GUAGE_BLACK_SMALL" ));
  //			
  //			int iHP = g_pAVATAR->Get_HP();
  //			if ( iHP < 0 ) 
  //				iHP = 0;

  //			int iWidth = iWidthGuage * g_pAVATAR->Get_PercentHP() / 100;

  //			g_DrawImpl.Draw( PosSCR.x - iWidthGuage / 2 , PosSCR.y - NAMEBOX_HEIGHT / 2 + 4 , PosSCR.z, iWidth, IMAGE_RES_UI, CResourceMgr::GetInstance()->GetImageNID( IMAGE_RES_UI,"ID_GUAGE_RED_SMALL" ));
  //		}
  //	}
  //}
}
