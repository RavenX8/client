#ifndef _CClanSkillListItem_
#define _CClanSkillListItem_

#include "tgamectrl/WinCtrl.h"
#include "CSlot.h"
class CIconSkillClan;
class CDragItem;

/**
* 클랜창에서 클랜 스킬 리스트의 item class
*
* @Author		최종진
* @Date			2005/9/15
*/

class CClanSkillListItem : public CWinCtrl {
public:
          CClanSkillListItem(void );
  virtual ~CClanSkillListItem(void);

  void     Draw() override;
  void     MoveWindow(POINT pt) override;
  void     Update(POINT     ptMouse) override;
  unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;
  void     Show() override;
  void     Hide() override;

  void            SetIcon(CIconSkillClan* pIcon);
  CIconSkillClan* GetIcon();

protected:
  CSlot m_Slot;
  //CTButton*	m_pButton;
  CDragItem* m_pDragItem;
};
#endif
