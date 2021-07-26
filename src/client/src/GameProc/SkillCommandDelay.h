#ifndef _SKILL_COMMAND_DELAY_
#define _SKILL_COMMAND_DELAY_

#include "../Util/JSingleton.h"

///
/// class CSkillCommandDelay 
/// 
/// Managing delay of casting time
///

class CSkillCommandDelay {
private:
  const DWORD m_dwCastingTimeDelay;

  DWORD m_dwCastingStartTime;
  bool  m_bCanCastSkill;
  int   m_iProgressRatio;

  HNODE m_hCoverTexture;
  int   m_iAnimationFrame;

  CSkillCommandDelay(void );
public:
  ~CSkillCommandDelay(void);

  CSkillCommandDelay(const CSkillCommandDelay&) = delete;
  CSkillCommandDelay& operator=(const CSkillCommandDelay &) = delete;
  CSkillCommandDelay(CSkillCommandDelay &&) = delete;
  CSkillCommandDelay & operator=(CSkillCommandDelay &&) = delete;

  static auto& GetSingleton(){
    static CSkillCommandDelay inst;
    return inst;
  }

  bool Init();
  void Release();

  void StartSkill();
  bool CanCastSkill();

  void Proc();
  void Draw(int x, int y);

  int GetSkillCommandDelayProgressRatio() { return m_iProgressRatio; }
};

#endif //_SKILL_COMMAND_DELAY_
