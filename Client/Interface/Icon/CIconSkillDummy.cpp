#include "StdAfx.h"
#include "./CIconSkillDummy.h"
#include "../IO_ImageRes.h"
#include "../../Common/IO_Skill.h"
CIconSkillDummy::CIconSkillDummy(void) {}

CIconSkillDummy::CIconSkillDummy(int skillindex) {

  m_skillindex      = skillindex;
  m_iModuleID       = IMAGE_RES_SKILL_ICON;
  m_iIconGraphicID  = SKILL_ICON_NO( skillindex);
  m_dwIconDrawColor = D3DCOLOR_ARGB( 255, 150, 150, 150 );
}

CIconSkillDummy::~CIconSkillDummy(void) {}

void CIconSkillDummy::Draw() {
  CIcon::Draw();
}

int CIconSkillDummy::GetSkillIndex() {
  return m_skillindex;
}

int CIconSkillDummy::GetSkillLevel() {
  return SKILL_LEVEL( GetSkillIndex() );
}
