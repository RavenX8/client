#include "StdAfx.h"
#include "./MouseTargetEffect.h"
#include "../IO_Effect.h"
#include "../Game.h"

const int s_iMouseTargetEffectIDX = 296;

//------------------------------------------------------------------------------------------------
/// @param
/// @brief Constructor
//-------------------------------------------------------------------------------------------------

CMouseTargetEffect::CMouseTargetEffect(void) {
  m_pEffect = nullptr;
}

//-------------------------------------------------------------------------------------------------
/// @param
/// @brief Destructor
//-------------------------------------------------------------------------------------------------

CMouseTargetEffect::~CMouseTargetEffect(void) {
  if ( m_pEffect != nullptr ) {
    delete m_pEffect;
    m_pEffect = nullptr;
  }
}

//-------------------------------------------------------------------------------------------------
/// @param
/// @brief 이펙트를 읽어 들이고, 클래스 초기화
//-------------------------------------------------------------------------------------------------

void CMouseTargetEffect::Init() {
  m_pEffect = g_pEffectLIST->Add_EffectWithIDX( s_iMouseTargetEffectIDX, false );
  if ( m_pEffect == nullptr ) {
    g_pCApp->ErrorBOX( "Target effect load failed", "ERROR" );
  }
}

//-------------------------------------------------------------------------------------------------
/// @param
/// @brief 이펙트 클리어..
//-------------------------------------------------------------------------------------------------

void CMouseTargetEffect::Clear() {
  if ( m_pEffect != nullptr ) {
    delete m_pEffect;
    m_pEffect = nullptr;
  }
}

//-------------------------------------------------------------------------------------------------
/// @param
/// @brief 효과 출력 시작
//-------------------------------------------------------------------------------------------------

void CMouseTargetEffect::Start(float x, float y, float z) {
  if ( g_GameDATA.m_bFilmingMode )
    return;

  if ( m_pEffect == nullptr ) {
    Init();
  }

  if ( m_pEffect ) {
    m_pEffect->RemoveFromScene();
    D3DVECTOR vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    m_pEffect->Transform( vec );
    m_pEffect->InsertToScene();
  }
}

//-------------------------------------------------------------------------------------------------
/// param 
/// @brief 매 루프 마다 처리될것이 있으면 처리
//-------------------------------------------------------------------------------------------------

void CMouseTargetEffect::Proc() {
  if ( m_pEffect ) {
    if ( m_pEffect->IsFinish() ) {
      m_pEffect->RemoveFromScene();
    }
  }
}
