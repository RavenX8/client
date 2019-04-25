#include "StdAfx.h"
#include "./ObjectTargetEffect.h"
#include "../CObjCHAR.h"

const int s_ObjectTargetEffect = 1406;

CObjectTargetEffect::CObjectTargetEffect(void) {
  m_pEffect = nullptr;
  m_pParent = nullptr;
}

CObjectTargetEffect::~CObjectTargetEffect(void) {}

void CObjectTargetEffect::Init() {
  m_pEffect = g_pEffectLIST->Add_EffectWithIDX( s_ObjectTargetEffect, false );
  if ( m_pEffect == nullptr ) {
    g_pCApp->ErrorBOX( "Target effect load failed", "ERROR" );
  }
}

void CObjectTargetEffect::Clear() {
  if ( m_pEffect != nullptr ) {
    delete m_pEffect;
    m_pEffect = nullptr;
  }
}

void CObjectTargetEffect::Attach(CObjCHAR* pObjCHAR) {
  if ( m_pEffect == nullptr )
    return;

  if ( pObjCHAR == nullptr || pObjCHAR->Get_HP() <= 0 )
    return;

  m_pParent = pObjCHAR;

  m_pEffect->UnlinkNODE();
  m_pEffect->LinkBONE( m_pParent->GetZMODEL(), 0 );
}

void CObjectTargetEffect::Detach() {
  m_pParent = nullptr;

  m_pEffect->UnlinkNODE();
}
