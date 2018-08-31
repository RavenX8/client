#ifndef _CIconSkillClan_
#define _CIconSkillClan_
#include "ciconskill.h"

/**
* 클랜스킬표시에 사용되는  Icon
*
* @Author	최종진
* @Date		2005/9/12
*/
class CIconSkillClan : public CIconSkill {
  int m_iSkillSlot;
public:
          CIconSkillClan(void );
          CIconSkillClan(int  iSkillSlot);
  virtual ~CIconSkillClan(void);

  int     GetIndex() override; /// GetSkillSlotFromIcon()
  void    SetSkillSlotToIcon(int iSkillSlot);
  int     GetSkillSlotFromIcon(); /// SetSkillSlotToIcon에서 저장해두었던 스킬슬롯 인덱스를 리턴한다.
  int     GetSkillLevel() override;
  int     GetSkillIndex() override; /// CSkill에서 스킬인덱스(STB라인넘버)를 구해서 리턴한다.
  CSkill* GetSkill();

  //----------------------------------------------------------------------------------------------------
  /// Overrided from CIcon
  //----------------------------------------------------------------------------------------------------
  void        Draw() override;
  void        ExecuteCommand() override;
  CIcon*      Clone() override;
  void        GetToolTip(CInfo& ToolTip, DWORD dwDialogType, DWORD dwType) override;
  const char* GetName() override;
};
#endif
