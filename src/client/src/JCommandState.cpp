#include "StdAfx.h"
#include "JCommandState.h"
#include "Network//CNetwork.h"
#include "System//CGame.h"
#include "GameProc/ObjectTargetEffect.h"
#include "CCamera.h"
#include "IO_Terrain.h"
#include "GameData/CParty.h"
#include "../Country.h"

#include "Interface/it_mgr.h"

//CUserInputSystem _UserInputSystem;

const int CAMERA_MOVE_SPEED = 10;
const int KEYMOVE_TIME      = 500;
const int KEYMOVE_DISTANCE  = 1000;

CUserInputSystem::CUserInputSystem() {
  m_pUserInput             = nullptr;
  m_iCurrentUserInputStyle = SEVENHEARTS_USER_INPUT_STYLE;
  m_pObjectTargetEffect    = nullptr;

  ChangeUserInputStyle( m_iCurrentUserInputStyle );
}

CUserInputSystem::~CUserInputSystem() {
  SAFE_DELETE( m_pUserInput );
}

void CUserInputSystem::Init() {
  /*m_pObjectTargetEffect = new CObjectTargetEffect;
  m_pObjectTargetEffect->Init();*/
}

void CUserInputSystem::Clear() {
  // Destruct
  /*m_pObjectTargetEffect->Clear();
  delete m_pObjectTargetEffect;*/
}

void CUserInputSystem::ChangeUserInputStyle(int iStyle) {
  m_iCurrentUserInputStyle = iStyle;

  switch ( iStyle ) {
    case DEFAULT_USER_INPUT_STYLE: m_pUserInput = new CDefaultUserInput();
      break;

    case SEVENHEARTS_USER_INPUT_STYLE: m_pUserInput = new CSevenHeartUserInput();
      break;
  }
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief �Ϲ� Ŭ��
//----------------------------------------------------------------------------------------------------

bool CUserInputSystem::ClickObject(int iTarget, D3DXVECTOR3& PosPICK, WPARAM wParam) {
  if ( m_pUserInput->ClickObject( iTarget, PosPICK, wParam ) ) {
    /*CObjCHAR* pCHAR = g_pObjMGR->Get_CharOBJ( this->GetCurrentTarget(), false );
    m_pObjectTargetEffect->Attach( pCHAR );*/
    return true;
  }

  ///m_pObjectTargetEffect->Detach();
  return false;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief R button click
//----------------------------------------------------------------------------------------------------

bool CUserInputSystem::RButtonDown(int iTarget, D3DXVECTOR3& PosPICK, WPARAM wParam) {
  if ( m_pUserInput->RButtonDown( iTarget, PosPICK, wParam ) ) {
    return true;
  }

  return false;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ���� Ŭ��
//----------------------------------------------------------------------------------------------------

bool CUserInputSystem::DBClickObject(int iTarget, D3DXVECTOR3& PosPICK, WPARAM wParam) {
  if ( m_pUserInput ) {
    if ( m_pUserInput->DBClickObject( iTarget, PosPICK, wParam ) ) {
      /*CObjCHAR* pCHAR = g_pObjMGR->Get_CharOBJ( this->GetCurrentTarget(), false );
      m_pObjectTargetEffect->Attach( pCHAR );*/
      return true;
    }
  }

  //m_pObjectTargetEffect->Detach();
  return false;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief
//----------------------------------------------------------------------------------------------------

bool CUserInputSystem::OnKeyDown(WPARAM wParam, LPARAM lParam) {
  if ( m_pUserInput ) {
    if ( m_pUserInput->OnKeyDown( wParam, lParam ) ) {
      return true;
    }
  }

  return false;
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
///
/// Interface for user input class
///
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

CUserInputState::CUserInputState() {
  m_iCommandType = 0;
  m_iCommand     = 0;

  m_iMouseState        = 0;
  m_iCurrentTargetType = 0;
  m_iCurrentTarget     = 0;

  m_iCurrentActiveSkillSlot = -1;
}

CUserInputState::~CUserInputState() {}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief Current active skill
//----------------------------------------------------------------------------------------------------

void CUserInputState::SetCurrentActiveSkillSlot(int iSkillSlotNO) {
  m_iCurrentActiveSkillSlot = iSkillSlotNO;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief PVP ����϶� Ÿ�ٿ� ���� ����� ����..
///			Avatar �϶��� ����..
//----------------------------------------------------------------------------------------------------

void       CUserInputState::PVPTarget_Click(int iTargetType, int iTargetObj, D3DVECTOR& pickPos, bool bDBClick) {
  CObjAVT* pObjAVT = g_pObjMGR->Get_CharAVT( iTargetObj, true );
  if ( pObjAVT == nullptr )
    return;

  /// PVP ����ϰ��..
  if ( g_GameDATA.m_iPvPState ) {
    /// ���� ���̶��..
    if ( IsEnemy( pObjAVT ) ) {
      m_iCurrentTarget = iTargetObj;
      g_pNet->Send_cli_ATTACK( iTargetObj );
    }
  }
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief �� �ƹ�Ÿ�� ���� ������ ����
//----------------------------------------------------------------------------------------------------

bool CUserInputState::IsEnemy(CObjCHAR* pTarget) {
  if ( pTarget == nullptr )
    return false;

  //----------------------------------------------------------------------------------------------------
  /// PVP �������ϰ�쿡�� PVP �÷��װ� ON ���°� �ƴ϶�� ��� ���ݸ�� ����
  //----------------------------------------------------------------------------------------------------
  if ( g_pTerrain->IsPVPZone() ) {
    if ( g_GameDATA.m_iPvPState == PVP_CANT )
      return false;
  }

  switch ( g_GameDATA.m_iPvPState ) {
    case PVP_PERMITION_ALL: {
      if ( !(g_pAVATAR->Is_ALLIED( pTarget )) )
        return true;
      return false;
    }
    break;
    case PVP_NON_PARTY_ALL: {
      int iServeObjIDX = g_pObjMGR->Get_ServerObjectIndex( pTarget->Get_INDEX() );

      PartyMember partMember;
      /// ����Ƽ�� �ƴ϶��..
      if ( CParty::GetInstance().GetMemberInfoByObjSvrIdx( iServeObjIDX, partMember ) == false ) {
        return true;
      }

      return false;
    }
    break;
    case PVP_CANT: break;
  }

  if ( g_pAVATAR->Is_ALLIED( pTarget ) )
    return false;

  return true;
}

//----------------------------------------------------------------------------------------------------
///
/// Default user input class
///
//----------------------------------------------------------------------------------------------------

CDefaultUserInput::CDefaultUserInput() {}

CDefaultUserInput::~CDefaultUserInput() {}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief Default Type �� � ������Ʈ�� ���õ��� �ʴ��� ���� Ÿ���� Ŭ���� ���� �ʴ´�.
///		   �׷��� ���ϰ��� true.
//----------------------------------------------------------------------------------------------------

bool CDefaultUserInput::ClickObject(int iTarget, D3DXVECTOR3& PickPos, WPARAM wParam) {
  if ( iTarget ) {
    switch ( g_pObjMGR->m_pOBJECTS[iTarget]->Get_TYPE() ) {
      case OBJ_GROUND:
      case OBJ_CNST: CGame::GetInstance().SetMouseTargetEffect( PickPos.x, PickPos.y, PickPos.z );
        SetTargetObject( g_pObjMGR->m_pOBJECTS[iTarget]->Get_TYPE(),
                         iTarget,
                         PickPos,
                         wParam );
        break;
      case OBJ_AVATAR:
      case OBJ_NPC:
      case OBJ_MOB: if ( ((CObjCHAR*)g_pObjMGR->m_pOBJECTS[iTarget])->CanClickable() ) {
          SetTargetObject( g_pObjMGR->m_pOBJECTS[iTarget]->Get_TYPE(),
                           iTarget,
                           PickPos,
                           wParam );
          /*
          //����ȣ::�� Ÿ���� ���� �Ǿ��ٸ� ������. 
          #if	defined(_GBC)
                if(g_pObjMGR->m_pOBJECTS[ iTarget ]->Get_TYPE()  == OBJ_MOB)
                  g_pNet->Send_cli_ATTACK(iTarget);
          #endif
          */
        }
        return true;
    }

  }
  //------------------------------------------------------------------------------
  //����ȣ::����� ��ų�� ����ؼ� ������ Ŭ���ϸ� Ÿ���� �Ҿ� ������. 
  //���� ���� ��Ų��. 
  /*
  #if defined(_GBC)
    else
    {
      if(g_pAVATAR->GetPetMode() >= 0)
      {
        if(g_pAVATAR->Get_COMMAND() == CMD_SKILL2OBJ)
        {
          g_pAVATAR->m_pObjCART->Set_COMMAND(CMD_STOP);	
          g_pAVATAR->Set_COMMAND(CMD_STOP);
          g_pNet->Send_cli_CANTMOVE();
          return false;
        } 
        
      }
      
    } 
  #endif
  */
  //------------------------------------------------------------------------------

  g_pNet->Send_cli_MOUSECMD( iTarget, PickPos );
  CGame::GetInstance().SetMouseTargetEffect( PickPos.x, PickPos.y, PickPos.z );

  return false;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief 
//----------------------------------------------------------------------------------------------------

bool CDefaultUserInput::RButtonDown(int iTarget, D3DXVECTOR3& PickPos, WPARAM wParam) {
  ClearMouseState();

  return false;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ����Ŭ���� ��쿡�� Ÿ���� �����Ѵ�.
//----------------------------------------------------------------------------------------------------

bool CDefaultUserInput::DBClickObject(int iTarget, D3DXVECTOR3& PickPos, WPARAM wParam) {
  /// ���õȰ� ���ٸ� ���콺 ��� Ŭ����..
  /// ����Ŭ���� ��츸..
  {
    ///	ClearMouseState();
  }

  if ( iTarget ) {
    CGameOBJ* pObj = g_pObjMGR->m_pOBJECTS[iTarget];
    if ( pObj ) {
      switch ( pObj->Get_TYPE() ) {
        case OBJ_GROUND:
        case OBJ_CNST: CGame::GetInstance().SetMouseTargetEffect( PickPos.x, PickPos.y, PickPos.z );
          SetTargetObject( pObj->Get_TYPE(),
                           iTarget,
                           PickPos,
                           wParam,
                           true );
          break;
        case OBJ_AVATAR:
        case OBJ_NPC:
        case OBJ_MOB: if ( ((CObjCHAR*)pObj)->CanClickable() ) {
            SetTargetObject( pObj->Get_TYPE(),
                             iTarget,
                             PickPos,
                             wParam,
                             true );
          }
          return true;
      }
    } else {
      assert( 0 && " DBClickOjbect[ pObj is Invalid ] " );
    }
  }

  g_pNet->Send_cli_MOUSECMD( iTarget, PickPos );
  CGame::GetInstance().SetMouseTargetEffect( PickPos.x, PickPos.y, PickPos.z );

  /// ���õȰ� ���ٸ� ���콺 ��� Ŭ����..
  /// ����Ŭ���� ��츸..
  {
    ///	ClearMouseState();
  }

  return false;
}

bool CDefaultUserInput::OnKeyDown(WPARAM wParam, LPARAM lParam) {
  //----------------------------------------------------------------------------------------------------
  /// �������� üũ�ؾߵ� �޼�����..
  //----------------------------------------------------------------------------------------------------

  ///
  /// 2004/12/10 KeyMove Ŭ������ �̵�.. ����Ű ó���� ���ؼ�
  ///	

  //switch ( wParam ) 
  //{		
  //	/// Ű���� ����..
  //	case VK_UP:		
  //		{
  //			if( g_pAVATAR == NULL )
  //				return false;

  //			static int sLastShiftGoFrame = 0;
  //			int CurrentShiftGoFrame = g_GameDATA.GetGameTime();

  //			if( CurrentShiftGoFrame - sLastShiftGoFrame > 500 )
  //			{				
  //				D3DVECTOR Dir = g_pAVATAR->Get_CurPOS() - g_pCamera->Get_Position();
  //				D3DXVECTOR3 vDir = D3DXVECTOR3( Dir.x, Dir.y, Dir.z );				
  //				D3DXVec3Normalize( &vDir, &vDir );
  //				vDir.z = 0.0f;

  //				vDir = vDir * 1500.0f;               
  //				Dir.x = vDir.x;
  //				Dir.y = vDir.y;
  //				Dir.z = vDir.z;

  //				D3DVECTOR vPos = g_pAVATAR->Get_CurPOS() + Dir;
  //				g_pNet->Send_cli_MOUSECMD( 0, vPos );

  //				///CGame::GetInstance().SetMouseTargetEffect( vPos.x, vPos.y, vPos.z );

  //				sLastShiftGoFrame = CurrentShiftGoFrame ;
  //			}
  //		}
  //		return true;
  //	case VK_LEFT:
  //		g_pCamera->Add_YAW( -CAMERA_MOVE_SPEED/2 );
  //		break;
  //	case VK_RIGHT:
  //		g_pCamera->Add_YAW( CAMERA_MOVE_SPEED/2 );
  //		break;

  //	default:
  //		{
  //			
  //		}
  //		break;		
  //}

  if ( lParam & 0x40000000 ) {
    // ������ ���� �ִ� Ű��....
    return false;
  }

  return false;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ���콺 ���� �ʱ�ȭ.
//----------------------------------------------------------------------------------------------------
void CDefaultUserInput::ClearMouseState() {
  m_iMouseState    = MOUSE_STATE_NONE;
  m_iCurrentTarget = 0;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief
//----------------------------------------------------------------------------------------------------

void CDefaultUserInput::SetTargetObject(int iTargetObjType, int iTargetObj, D3DVECTOR& pickPos, DWORD wVKeyState, bool bDBClick) {

  m_iCurrentTargetType = iTargetObjType;

  //----------------------------------------------------------------------------------------------------
  /// Ctrl �� Ŭ���� ���¿����� Ÿ�ټ���, ����
  //----------------------------------------------------------------------------------------------------
  if ( wVKeyState & MK_CONTROL ) {
    SetTargetObject_CtrlClick( iTargetObjType, iTargetObj, pickPos, bDBClick );
    return;
  }

  //----------------------------------------------------------------------------------------------------
  /// shift �� Ŭ���� ���¿����� Ÿ�ټ���, ����
  //----------------------------------------------------------------------------------------------------
  /*if( wVKeyState&MK_SHIFT )
  {	
    SetTargetObject_ShiftClick( iTargetObjType, iTargetObj, pickPos, bDBClick );	
    return;
  }*/

  //----------------------------------------------------------------------------------------------------
  /// �Ϲ����� ���¿����� Ÿ�ټ���, ����
  //----------------------------------------------------------------------------------------------------
  SetTargetObject_Normal( iTargetObjType, iTargetObj, pickPos, bDBClick );
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief �Ϲ����� ��Ȳ������ Ÿ�ټ���, ����
//----------------------------------------------------------------------------------------------------

void CDefaultUserInput::SetTargetObject_Normal(int iTargetType, int iTargetObj, D3DVECTOR& pickPos, bool bDBClick) {
  /// Ÿ���� �ǹ��̰ų�, ����������Ʈ���.. �̰����� �������� �̵�...
  switch ( m_iCurrentTargetType ) {
    case OBJ_NPC: if ( m_iCurrentTarget == iTargetObj || bDBClick ) {
        g_pNet->Send_cli_MOUSECMD( iTargetObj, pickPos );
      } else {
        ///�ܼ��� Ÿ���ø� �����Ѵ�.
        m_iCurrentTarget = iTargetObj;
      }
      break;

    case OBJ_AVATAR: {
      if ( g_pAVATAR->Get_COMMAND() == CMD_ATTACK && m_iCurrentTarget == iTargetObj )
        break;

      /// ���� Ÿ���� �� ��븦 �ѹ��� Ŭ��������..
      /// Ȥ�� ���Ŭ���� ������..
      if ( m_iCurrentTarget == iTargetObj || bDBClick ) {
        //----------------------------------------------------------------------------------------------------
        /// PVP ��尡 Ȱ��ȭ �������...
        //----------------------------------------------------------------------------------------------------
        PVPTarget_Click( iTargetType, iTargetObj, pickPos, bDBClick );

        m_iCurrentTarget = iTargetObj;
        //g_pAVATAR->m_iServerTarget = g_pObjMGR->Get_ServerObjectIndex( iTargetObj );
        CObjAVT* pAvatar = g_pObjMGR->Get_CharAVT( iTargetObj, false );
        if ( pAvatar && pAvatar->IsPersonalStoreMode() )
          g_pNet->Send_cli_MOUSECMD( iTargetObj, pickPos );
      } else {
        ///�ܼ��� Ÿ���ø� �����Ѵ�.
        m_iCurrentTarget = iTargetObj;
      }
      break;
    }
    case OBJ_MOB: {
      CObjMOB* pObjMOB = (CObjMOB*)g_pObjMGR->Get_CharOBJ( iTargetObj, true );
      if ( pObjMOB != nullptr ) {
        /// ���� Ÿ���� �� ��븦 �ѹ��� Ŭ��������..
        /// Ȥ�� ���Ŭ���� ������..
        if ( m_iCurrentTarget == iTargetObj || bDBClick ) {
          /// ���ݺҰ� NPC �� ���ݸ���..
          if ( g_pAVATAR->Is_ALLIED( pObjMOB ) == false && (NPC_CAN_TARGET( pObjMOB->Get_CharNO() ) != 1) ) {
            //----------------------------------------------------------------------------------------------------
            /// PAT�� Ÿ�� �ִ� ���߿��� �Ұ�
            /// �׷��� Castle Gear �� ����
            //----------------------------------------------------------------------------------------------------
            int iPetMode = g_pAVATAR->GetPetMode();
            if ( iPetMode < 0 ) {
              g_pNet->Send_cli_ATTACK( iTargetObj );
            } else {
              /// Pet mode �� ��쿡��..
              if ( g_pAVATAR->CanAttackPetMode() ) {
                g_pNet->Send_cli_ATTACK( iTargetObj );
              } else {
                g_pNet->Send_cli_MOUSECMD( iTargetObj, pickPos );
              }
            }

          } else
            g_pNet->Send_cli_MOUSECMD( iTargetObj, pickPos );

          m_iCurrentTarget = iTargetObj;
          //g_pAVATAR->m_iServerTarget = g_pObjMGR->Get_ServerObjectIndex( iTargetObj );
        } else {
          ///�ܼ��� Ÿ���ø� �����Ѵ�.
          m_iCurrentTarget = iTargetObj;
        }
      }
    }
    break;
    case OBJ_GROUND:
    case OBJ_CNST: g_pNet->Send_cli_MOUSECMD( 0, pickPos );
      ///ClearMouseState();
      break;
  }
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ���ͷ� Ű�� �������¿����� Ÿ�ټ���, ����
//----------------------------------------------------------------------------------------------------

void CDefaultUserInput::SetTargetObject_CtrlClick(int iTargetType, int iTargetObj, D3DVECTOR& pickPos, bool bDBClick) {
  if ( m_iCurrentTarget != iTargetObj ) {
    switch ( m_iCurrentTargetType ) {
      case OBJ_MOB: {
        m_iCurrentTarget = iTargetObj;
        ///g_pAVATAR->m_iServerTarget = g_pObjMGR->Get_ServerObjectIndex( iTargetObj );

        g_pNet->Send_cli_HP_REQ( iTargetObj );
      }
      break;

      case OBJ_NPC: {
        m_iCurrentTarget = iTargetObj;
        ///g_pAVATAR->m_iServerTarget = g_pObjMGR->Get_ServerObjectIndex( iTargetObj );
      }
      break;
      case OBJ_AVATAR: {
        m_iCurrentTarget = iTargetObj;
      }
      break;
    }
  }
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ����Ʈ Ű�� �������¿����� Ÿ�ټ���, ����
//----------------------------------------------------------------------------------------------------

void CDefaultUserInput::SetTargetObject_ShiftClick(int iTargetType, int iTargetObj, D3DVECTOR& pickPos, bool bDBClick) {
  if ( g_GameDATA.m_iPvPState == PVP_PERMITION_ALL ) {
    switch ( m_iCurrentTargetType ) {
      case OBJ_AVATAR: {
        CObjAVT* pObjAVT = g_pObjMGR->Get_CharAVT( iTargetObj, true );
        if ( pObjAVT == nullptr )
          return;

        if ( !(g_pAVATAR->Is_ALLIED( pObjAVT )) ) {
          m_iCurrentTarget = iTargetObj;

          //----------------------------------------------------------------------------------------------------
          /// PAT�� Ÿ�� �ִ� ���߿��� �Ұ�
          //----------------------------------------------------------------------------------------------------
          if ( g_pAVATAR->GetPetMode() < 0 )
            g_pNet->Send_cli_ATTACK( iTargetObj );

        }
      }
      break;
    }
  }
}

//----------------------------------------------------------------------------------------------------
///
/// Sevenhearts user input class
///
//----------------------------------------------------------------------------------------------------

CSevenHeartUserInput::CSevenHeartUserInput() {}

CSevenHeartUserInput::~CSevenHeartUserInput() {}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief
//----------------------------------------------------------------------------------------------------

bool CSevenHeartUserInput::ClickObject(int iTarget, D3DXVECTOR3& PickPos, WPARAM wParam) {
  if ( iTarget ) {
    switch ( g_pObjMGR->m_pOBJECTS[iTarget]->Get_TYPE() ) {
      case OBJ_GROUND:
      case OBJ_CNST: CGame::GetInstance().SetMouseTargetEffect( PickPos.x, PickPos.y, PickPos.z );
        SetTargetObject( g_pObjMGR->m_pOBJECTS[iTarget]->Get_TYPE(),
                         iTarget,
                         PickPos,
                         wParam );
        break;
      case OBJ_AVATAR:
      case OBJ_NPC:
      case OBJ_MOB: if ( ((CObjCHAR*)g_pObjMGR->m_pOBJECTS[iTarget])->CanClickable() ) {
          SetTargetObject( g_pObjMGR->m_pOBJECTS[iTarget]->Get_TYPE(),
                           iTarget,
                           PickPos,
                           wParam );
        }
        return true;
    }

  }

  //20050902 ȫ�� 2�ν� īƮ : ������ ž�½� Ÿ�� ���� ���� (��, ������ Ÿ���� ����)
  if ( g_pAVATAR->IsRideUser() ) {
    g_itMGR.AppendChatMsg( STR_BOARDING_CANT_USE, IT_MGR::CHAT_TYPE_SYSTEM, D3DCOLOR_ARGB( 255, 206, 223, 136) );
    ClearMouseState();
    return false;
  }

  g_pNet->Send_cli_MOUSECMD( iTarget, PickPos );
  CGame::GetInstance().SetMouseTargetEffect( PickPos.x, PickPos.y, PickPos.z );

  /// ���õȰ� ���ٸ� ���콺 ��� Ŭ����..
  {
    ClearMouseState();
  }

  return false;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief 
//----------------------------------------------------------------------------------------------------

bool CSevenHeartUserInput::RButtonDown(int iTarget, D3DXVECTOR3& PickPos, WPARAM wParam) {
  ClearMouseState();

  return false;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief
//----------------------------------------------------------------------------------------------------

bool CSevenHeartUserInput::DBClickObject(int iTarget, D3DXVECTOR3& PickPos, WPARAM wParam) {
  if ( iTarget ) {
    CGameOBJ* pObj = g_pObjMGR->m_pOBJECTS[iTarget];
    if ( pObj ) {
      switch ( pObj->Get_TYPE() ) {
        case OBJ_GROUND:
        case OBJ_CNST: CGame::GetInstance().SetMouseTargetEffect( PickPos.x, PickPos.y, PickPos.z );
          SetTargetObject( pObj->Get_TYPE(),
                           iTarget,
                           PickPos,
                           wParam,
                           true );
          break;
        case OBJ_AVATAR:
        case OBJ_NPC:
        case OBJ_MOB: if ( ((CObjCHAR*)pObj)->CanClickable() ) {
            SetTargetObject( pObj->Get_TYPE(),
                             iTarget,
                             PickPos,
                             wParam,
                             true );
          }
          return true;
      }
    } else {
      assert( 0 && " DBClickOjbect[ pObj is Invalid ] " );
    }
  }

  g_pNet->Send_cli_MOUSECMD( iTarget, PickPos );
  CGame::GetInstance().SetMouseTargetEffect( PickPos.x, PickPos.y, PickPos.z );

  /// ���õȰ� ���ٸ� ���콺 ��� Ŭ����..
  {
    ClearMouseState();
  }

  return false;
}

bool CSevenHeartUserInput::OnKeyDown(WPARAM wParam, LPARAM lParam) {
  //----------------------------------------------------------------------------------------------------
  /// �������� üũ�ؾߵ� �޼�����..
  //----------------------------------------------------------------------------------------------------

  //switch ( wParam ) 
  //{		
  //	/// Ű���� ����..
  //	case VK_UP:		
  //		{
  //			if( g_pAVATAR == NULL )
  //				return false;

  //			static int sLastShiftGoFrame = 0;
  //			int CurrentShiftGoFrame = g_GameDATA.GetGameTime();

  //			if( CurrentShiftGoFrame - sLastShiftGoFrame > KEYMOVE_TIME )
  //			{				
  //				D3DVECTOR Dir = g_pAVATAR->Get_CurPOS() - g_pCamera->Get_Position();
  //				D3DXVECTOR3 vDir = D3DXVECTOR3( Dir.x, Dir.y, Dir.z );				
  //				D3DXVec3Normalize( &vDir, &vDir );
  //				vDir.z = 0.0f;

  //				vDir = vDir * KEYMOVE_DISTANCE;               
  //				Dir.x = vDir.x;
  //				Dir.y = vDir.y;
  //				Dir.z = vDir.z;

  //				D3DVECTOR vPos = g_pAVATAR->Get_CurPOS() + Dir;
  //				g_pNet->Send_cli_MOUSECMD( 0, vPos );

  //				///CGame::GetInstance().SetMouseTargetEffect( vPos.x, vPos.y, vPos.z );

  //				sLastShiftGoFrame = CurrentShiftGoFrame ;
  //			}
  //		}
  //		return true;

  //	/*case VK_LEFT:
  //		{
  //			g_pCamera->Add_YAW( CAMERA_MOVE_SPEED );
  //		}
  //		return true;

  //	case VK_RIGHT:
  //		{
  //			g_pCamera->Add_YAW( -CAMERA_MOVE_SPEED );
  //		}
  //		return true;*/

  //	default:
  //		{
  //			
  //		}
  //		break;		
  //}

  if ( lParam & 0x40000000 ) {
    // ������ ���� �ִ� Ű��....
    return false;
  }

  return false;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ���콺 ���� �ʱ�ȭ.
//----------------------------------------------------------------------------------------------------
void CSevenHeartUserInput::ClearMouseState() {
  m_iMouseState    = MOUSE_STATE_NONE;
  m_iCurrentTarget = 0;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief
//----------------------------------------------------------------------------------------------------

void CSevenHeartUserInput::SetTargetObject(int iTargetObjType, int iTargetObj, D3DVECTOR& pickPos, DWORD wVKeyState, bool bDBClick) {

  m_iCurrentTargetType = iTargetObjType;

  //----------------------------------------------------------------------------------------------------
  /// Ctrl �� Ŭ���� ���¿����� Ÿ�ټ���, ����
  //----------------------------------------------------------------------------------------------------
  if ( wVKeyState & MK_CONTROL ) {
    SetTargetObject_CtrlClick( iTargetObjType, iTargetObj, pickPos, bDBClick );
    return;
  }

  //----------------------------------------------------------------------------------------------------
  /// shift �� Ŭ���� ���¿����� Ÿ�ټ���, ����
  //----------------------------------------------------------------------------------------------------
  /*if( wVKeyState&MK_SHIFT )
  {	
    SetTargetObject_ShiftClick( iTargetObjType, iTargetObj, pickPos, bDBClick );	
    return;
  }*/

  //----------------------------------------------------------------------------------------------------
  /// �Ϲ����� ���¿����� Ÿ�ټ���, ����
  //----------------------------------------------------------------------------------------------------
  SetTargetObject_Normal( iTargetObjType, iTargetObj, pickPos, bDBClick );
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief �Ϲ����� ��Ȳ������ Ÿ�ټ���, ����
//----------------------------------------------------------------------------------------------------

void CSevenHeartUserInput::SetTargetObject_Normal(int iTargetType, int iTargetObj, D3DVECTOR& pickPos, bool bDBClick) {
  /// Ÿ���� �ǹ��̰ų�, ����������Ʈ���.. �̰����� �������� �̵�...
  switch ( m_iCurrentTargetType ) {
    case OBJ_NPC: g_pNet->Send_cli_MOUSECMD( iTargetObj, pickPos );
      break;

    case OBJ_AVATAR: {
      if ( g_pAVATAR->Get_COMMAND() == CMD_ATTACK && m_iCurrentTarget == iTargetObj )
        break;

      //----------------------------------------------------------------------------------------------------
      /// PVP ��尡 Ȱ��ȭ �������...
      //----------------------------------------------------------------------------------------------------
      PVPTarget_Click( iTargetType, iTargetObj, pickPos, bDBClick );

      m_iCurrentTarget = iTargetObj;
      //g_pAVATAR->m_iServerTarget = g_pObjMGR->Get_ServerObjectIndex( iTargetObj );
      CObjAVT* pAvatar = g_pObjMGR->Get_CharAVT( iTargetObj, false );
      if ( pAvatar && pAvatar->IsPersonalStoreMode() )
        g_pNet->Send_cli_MOUSECMD( iTargetObj, pickPos );
      break;
    }
    case OBJ_MOB: {
      /// ���� ���� �������γ��� �ٽ� ��Ŷ�� ������ �ʴ´�.
      if ( g_pAVATAR->Get_COMMAND() == CMD_ATTACK && iTargetObj == g_pObjMGR->Get_ClientObjectIndex( g_pAVATAR->m_iServerTarget ) )
        break;

      CObjMOB* pObjMOB = (CObjMOB*)g_pObjMGR->Get_CharOBJ( iTargetObj, true );
      if ( pObjMOB != nullptr ) {
        /// ���ݺҰ� NPC �� ���ݸ���..
        if ( g_pAVATAR->Is_ALLIED( pObjMOB ) == false && (NPC_CAN_TARGET( pObjMOB->Get_CharNO() ) != 1) ) {
          //----------------------------------------------------------------------------------------------------
          /// PAT�� Ÿ�� �ִ� ���߿��� �Ұ�
          /// �׷��� Castle Gear �� ����
          //----------------------------------------------------------------------------------------------------
          int iPetMode = g_pAVATAR->GetPetMode();
          if ( iPetMode < 0 ) {
            g_pNet->Send_cli_ATTACK( iTargetObj );
          } else {
            /// Pet mode �� ��쿡��..
            if ( g_pAVATAR->CanAttackPetMode() ) {
              g_pNet->Send_cli_ATTACK( iTargetObj );
            } else {
              g_pNet->Send_cli_MOUSECMD( iTargetObj, pickPos );
            }
          }

        } else
          g_pNet->Send_cli_MOUSECMD( iTargetObj, pickPos );
      }

      m_iCurrentTarget = iTargetObj;
      //g_pAVATAR->m_iServerTarget = g_pObjMGR->Get_ServerObjectIndex( iTargetObj );
    }
    break;
    case OBJ_GROUND:
    case OBJ_CNST: g_pNet->Send_cli_MOUSECMD( 0, pickPos );
      ClearMouseState();
      break;
  }
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ���ͷ� Ű�� �������¿����� Ÿ�ټ���, ����
//----------------------------------------------------------------------------------------------------

void CSevenHeartUserInput::SetTargetObject_CtrlClick(int iTargetType, int iTargetObj, D3DVECTOR& pickPos, bool bDBClick) {
  if ( m_iCurrentTarget != iTargetObj ) {
    switch ( m_iCurrentTargetType ) {
      case OBJ_MOB: {
        m_iCurrentTarget = iTargetObj;
        ///g_pAVATAR->m_iServerTarget = g_pObjMGR->Get_ServerObjectIndex( iTargetObj );

        g_pNet->Send_cli_HP_REQ( iTargetObj );
      }
      break;

      case OBJ_NPC: {
        m_iCurrentTarget = iTargetObj;
        ///g_pAVATAR->m_iServerTarget = g_pObjMGR->Get_ServerObjectIndex( iTargetObj );
      }
      break;
      case OBJ_AVATAR: {
        m_iCurrentTarget = iTargetObj;
      }
      break;
    }
  }
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ����Ʈ Ű�� �������¿����� Ÿ�ټ���, ����
//----------------------------------------------------------------------------------------------------

void CSevenHeartUserInput::SetTargetObject_ShiftClick(int iTargetType, int iTargetObj, D3DVECTOR& pickPos, bool bDBClick) {
  if ( g_GameDATA.m_iPvPState == PVP_PERMITION_ALL ) {
    switch ( m_iCurrentTargetType ) {
      case OBJ_AVATAR: {
        CObjAVT* pObjAVT = g_pObjMGR->Get_CharAVT( iTargetObj, true );
        if ( pObjAVT == nullptr )
          return;

        if ( !(g_pAVATAR->Is_ALLIED( pObjAVT )) ) {
          m_iCurrentTarget = iTargetObj;

          //----------------------------------------------------------------------------------------------------
          /// PAT�� Ÿ�� �ִ� ���߿��� �Ұ�
          //----------------------------------------------------------------------------------------------------
          if ( g_pAVATAR->GetPetMode() < 0 )
            g_pNet->Send_cli_ATTACK( iTargetObj );

        }
      }
      break;
    }
  }
}

void CUserInputSystem::SetTargetSelf() {
  assert( g_pAVATAR);
  if ( nullptr == g_pAVATAR ) return;

  if ( g_pAVATAR->Get_COMMAND() == CMD_ATTACK && GetCurrentTarget() == g_pAVATAR->Get_INDEX() )
    return;

  SetCurrentTarget( g_pAVATAR->Get_INDEX() );
}
