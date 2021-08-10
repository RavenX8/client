#include "StdAfx.h"
#include "./TutorialEventUtility.h"
#include "../System/SystemProcScript.h"
#include "../OBJECT.h"
#include "../Interface/it_mgr.h"

//CTutorialEventUtility _TutorialEventUtility;

CTutorialEventUtility::CTutorialEventUtility(void) {}

CTutorialEventUtility::~CTutorialEventUtility(void) {}

bool CTutorialEventUtility::Init() {
  /// Load notify button table
  if ( m_TblNotifyButtonEvent.Load( "3DDATA\\STB\\EventButton.STB", true, true ) == false )
    return false;

  return true;
}

void CTutorialEventUtility::Release() {
  m_TblNotifyButtonEvent.Free();
}

/// �˸� ��ư �̺�Ʈ�� �����Ѵ�.
bool CTutorialEventUtility::CreateNotifyEventButton(int iEventIdx) {
  if ( iEventIdx <= 0 )
    return false;

  if ( iEventIdx >= m_TblNotifyButtonEvent.m_nDataCnt )
    return false;

  g_itMGR.AddNotifybutton( iEventIdx );

  return true;
}

//-------------------------------------------------------------------------------------------
/// EventButton.STB�� ������ �˸� ��ư �̺�Ʈ�� �����Ѵ�.
//-------------------------------------------------------------------------------------------
bool CTutorialEventUtility::ExecNotifyEvent(int iEventIdx) {
  if ( iEventIdx <= 0 )
    return false;

  if ( iEventIdx >= m_TblNotifyButtonEvent.m_nDataCnt )
    return false;

  CSystemProcScript::GetSingleton().CallLuaFunction( m_TblNotifyButtonEvent.m_ppDESC[iEventIdx],
                                                     ZZ_PARAM_INT, g_pAVATAR->Get_INDEX(), ZZ_PARAM_END );

  return true;
}
