#include "StdAfx.h"
#include "./ObjectActionProcessor.h"
#include "CGameOBJ.h"
#include "OBJECT.h"
#include "Network/CNetwork.h"
#include "ObjFixedEvent.h"
#include "Event/Quest_FUNC.h"

//------------------------------------------------------------------------------------------------
/// Managing for CActionProcessChain
//------------------------------------------------------------------------------------------------

//CObjectActionProcessor __objectActionProcess;

CObjectActionProcessor::CObjectActionProcessor(void) {
  m_pRootActionChain = nullptr;
}

CObjectActionProcessor::~CObjectActionProcessor(void) {
  if ( m_pRootActionChain != nullptr ) {
    delete m_pRootActionChain;
    m_pRootActionChain = nullptr;
  }
}

void                          CObjectActionProcessor::MakeChain() {
  CWarpObjectActionProcessor* pWarpObjActionProc = new CWarpObjectActionProcessor();
  SetNextChain( pWarpObjActionProc );

  CEventObjectActionProcessor* pEventObjActionProc = new CEventObjectActionProcessor();
  SetNextChain( pEventObjActionProc );
}

void CObjectActionProcessor::ClearChain() {
  if ( m_pRootActionChain != nullptr ) {
    m_pRootActionChain->DeleteNextChian();

    delete m_pRootActionChain;
    m_pRootActionChain = nullptr;
  }
}

void CObjectActionProcessor::SetNextChain(CActionProcessChain* pChain) {
  if ( m_pRootActionChain == nullptr ) {
    m_pRootActionChain = pChain;
    return;
  }

  m_pRootActionChain->AddNextChain( pChain );
}

void CObjectActionProcessor::ProcessChain(HNODE hNode) {
  if ( m_pRootActionChain != nullptr )
    m_pRootActionChain->ProcessChain( hNode );
}

//------------------------------------------------------------------------------------------------
/// Interface for "getUserdata function"
//------------------------------------------------------------------------------------------------

CActionProcessChain::CActionProcessChain(void) {
  m_pNextActionChain = nullptr;
}

CActionProcessChain::~CActionProcessChain(void) {
  if ( m_pNextActionChain != nullptr ) {
    delete m_pNextActionChain;
    m_pNextActionChain = nullptr;
  }
}

void CActionProcessChain::DeleteNextChian() {
  if ( m_pNextActionChain != nullptr ) {
    m_pNextActionChain->DeleteNextChian();

    delete m_pNextActionChain;
    m_pNextActionChain = nullptr;

  }
}

void CActionProcessChain::AddNextChain(CActionProcessChain* pNext) {
  if ( m_pNextActionChain == nullptr ) {
    m_pNextActionChain = pNext;
    return;
  }

  m_pNextActionChain->AddNextChain( pNext );
}

//------------------------------------------------------------------------------------------------
/// Warp object process
//------------------------------------------------------------------------------------------------

CWarpObjectActionProcessor::CWarpObjectActionProcessor(void) {}

CWarpObjectActionProcessor::~CWarpObjectActionProcessor(void) {}

void    CWarpObjectActionProcessor::ProcessChain(HNODE hNode) {
  HNODE hUserData = getUserData( hNode );
  if ( hUserData ) {
    CGameOBJ* pObj = reinterpret_cast<CGameOBJ*>(hUserData);

    /// warp object ���� �˻�..
    if ( pObj->IsA( OBJ_GROUND ) ) {
      int iSpecialData = pObj->GetSpecialUserNumericData();

      if ( iSpecialData >= WARP_OBJECT_INDEX_OFFSET ) {
        g_pNet->Send_cli_TELEPORT_REQ( g_pAVATAR, iSpecialData - WARP_OBJECT_INDEX_OFFSET );
        assert( 0 && " This is warp object" );
        setUserData( hNode, (HNODE)0 );
      }
    }
  }

  if ( m_pNextActionChain )
    m_pNextActionChain->ProcessChain( hNode );
}

//------------------------------------------------------------------------------------------------
/// Event object process
//------------------------------------------------------------------------------------------------

CEventObjectActionProcessor::CEventObjectActionProcessor(void) {}

CEventObjectActionProcessor::~CEventObjectActionProcessor(void) {}

void CEventObjectActionProcessor::ProcessChain(HNODE hNode) {
  if ( g_pAVATAR == nullptr )
    return;

  if ( intersectNode( g_pAVATAR->GetZMODEL(), hNode ) == 0 )
    return;

  HNODE hUserData = getUserData( hNode );
  if ( hUserData ) {
    CGameOBJ* pObj = reinterpret_cast<CGameOBJ*>(hUserData);

    /// Event object ���� �˻�..
    if ( pObj->IsA( OBJ_EVENTOBJECT ) ) {
      CObjFixedEvent* pEventObj = (CObjFixedEvent*)pObj;

      //----------------------------------------------------------------------------------------
      /// �ѹ� �浹�� 10�ʵ���( 30 FPS ���� ) �ٽ� �浹���� ����
      //----------------------------------------------------------------------------------------
      if ( pEventObj->GetCollisionCheckFrame() < (g_GameDATA.m_dwElapsedGameFrame - 1) )
        ///if( pEventObj->CanCheckCollision() )
      {

        /// Ʈ���Ű� ������ ������Ʈ���..
        if ( pEventObj->GetTriggerName() != nullptr ) {
          QF_doQuestTrigger( pEventObj->GetTriggerName() );
          ///::setUserData( hNode, (HNODE)0 );
        }

        if ( pEventObj->GetScriptName() != nullptr ) {
          pEventObj->ExecEventScript( pEventObj->GetCurrentState() + 1, false, true );
        }

        pEventObj->SetCollisionCheckFrame( g_GameDATA.m_dwElapsedGameFrame + 300 );
      }
      ///pEventObj->SetCheckCollisionFlag( false );

    }
  }

  if ( m_pNextActionChain )
    m_pNextActionChain->ProcessChain( hNode );
}
