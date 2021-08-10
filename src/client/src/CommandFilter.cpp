#include "StdAfx.h"
#include "./CommandFilter.h"
#include "ObjectCommand/ObjCommand.h"
#include "OBJECT.h"
#include "GameCommon/Skill.h"

//CCommandFilter _CommandFilter;

CCommandFilter::CCommandFilter(void) {
  m_pPrevCommand = nullptr;

  m_ObjCommandPool[OBJECT_COMMAND_STOP]       = new CObjStopCommand();
  m_ObjCommandPool[OBJECT_COMMAND_ATTACK]     = new CObjAttackCommand();
  m_ObjCommandPool[OBJECT_COMMAND_SIT]        = new CObjSitCommand();
  m_ObjCommandPool[OBJECT_COMMAND_STAND]      = new CObjStandCommand();
  m_ObjCommandPool[OBJECT_COMMAND_MOVE]       = new CObjMoveCommand();
  m_ObjCommandPool[OBJECT_COMMAND_DIE]        = new CObjDieCommand();
  m_ObjCommandPool[OBJECT_COMMAND_TOGGLE]     = new CObjToggleCommand();
  m_ObjCommandPool[OBJECT_COMMAND_Skill2SELF] = new CObjSkill2SelfCommand();
  m_ObjCommandPool[OBJECT_COMMAND_Skill2OBJ]  = new CObjSkill2ObjCommand();
  m_ObjCommandPool[OBJECT_COMMAND_Skill2POS]  = new CObjSkill2PosCommand();
}

            CCommandFilter::~CCommandFilter(void) {
  for ( int i = 0; i < OBJECT_COMMAND_MAX; i++ )
  SAFE_DELETE(m_ObjCommandPool[i])
}

//----------------------------------------------------------------------------------------------------
/// @brief for object pooling..
//----------------------------------------------------------------------------------------------------
CObjCommand* CCommandFilter::GetCommandObject(int iObjectType) {
  return m_ObjCommandPool[iObjectType];
}

//----------------------------------------------------------------------------------------------------
/// @brief ���� �������� ���̶�� �������� �ʴ´�.
//----------------------------------------------------------------------------------------------------
bool CCommandFilter::CanSendAttackCommand() {
  return false;
}

//----------------------------------------------------------------------------------------------------
/// @brief ���� �����غ����� ��ų�̶��
//----------------------------------------------------------------------------------------------------
bool CCommandFilter::CanSendSelfSkillCommand(int iSkillSlot) {
  if ( m_pPrevCommand == nullptr )
    return true;

  if ( m_pPrevCommand->GetType() == OBJECT_COMMAND_Skill2SELF ) {
    /// get skill.
    CSkillSlot* pSkillSlot = g_pAVATAR->GetSkillSlot();
    CSkill*     pSkill     = pSkillSlot->GetSkill( iSkillSlot );
    int         iSkillIDX  = pSkill->GetSkillIndex();

    if ( ((CObjSkill2SelfCommand*)m_pPrevCommand)->GetSkillIndex() == iSkillIDX )
      return false;
  }

  return true;
}

//----------------------------------------------------------------------------------------------------
/// @brief ���� �����غ����� ��ų�̶��( Ÿ�ٵ� ���� ) ��� ����
//----------------------------------------------------------------------------------------------------
bool CCommandFilter::CanSendTargetSkillCommand(int iClientTarget, int iSkillSlot) {
  if ( m_pPrevCommand == nullptr )
    return true;

  if ( m_pPrevCommand->GetType() == OBJECT_COMMAND_Skill2OBJ ) {
    int iServerTarget = g_pObjMGR->Get_ServerObjectIndex( iClientTarget );
    if ( ((CObjSkill2ObjCommand*)m_pPrevCommand)->GetServerTarget() == iServerTarget ) {
      /// get skill.
      CSkillSlot* pSkillSlot = g_pAVATAR->GetSkillSlot();
      CSkill*     pSkill     = pSkillSlot->GetSkill( iSkillSlot );
      int         iSkillIDX  = pSkill->GetSkillIndex();

      if ( ((CObjSkill2ObjCommand*)m_pPrevCommand)->GetSkillIndex() == iSkillIDX )
        return false;
    }
  }

  return true;
}

//----------------------------------------------------------------------------------------------------
/// @brief ���� �����غ����� ��ų�̶��( ��ġ�� ���� ) ��� ����
//----------------------------------------------------------------------------------------------------
bool CCommandFilter::CanSendPositionSkillCommand(D3DVECTOR& PosTO, int iSkillSlot) {
  if ( m_pPrevCommand == nullptr )
    return true;

  if ( m_pPrevCommand->GetType() == OBJECT_COMMAND_Skill2POS ) {
    if ( (((CObjSkill2PosCommand*)m_pPrevCommand)->GetTargetPos().x - PosTO.x) < 1.0f &&
         (((CObjSkill2PosCommand*)m_pPrevCommand)->GetTargetPos().y - PosTO.y) < 1.0f ) {
      /// get skill.
      CSkillSlot* pSkillSlot = g_pAVATAR->GetSkillSlot();
      CSkill*     pSkill     = pSkillSlot->GetSkill( iSkillSlot );
      int         iSkillIDX  = pSkill->GetSkillIndex();

      if ( ((CObjSkill2PosCommand*)m_pPrevCommand)->GetSkillIndex() == iSkillIDX )
        return false;
    }
  }

  return true;
}
