#ifndef _CSkillTreeDlg_
#define _CSkillTreeDlg_
#include "tgamectrl/TDialog.h"
#include "../../gamecommon/IObserver.h"
class CIconSkill;

struct node_skill {
  int         m_level;
  int         m_index;
  int         m_level_limit;
  int         m_offset_x;
  int         m_offset_y;
  CIconSkill* m_icon;
};

struct node_baseskill : public node_skill {
  HNODE                 m_texture;
  std::list<node_skill> m_children;
  void                  clear();
};

struct s_userdata {
  int                       depth;
  std::list<node_baseskill> base_skills;

  void clear() {
    depth = 0;
    std::list<node_baseskill>::iterator iter;
    for ( iter = base_skills.begin(); iter != base_skills.end(); ) {
      iter->clear();
      iter = base_skills.erase( iter );
    }
  }
};

/**
* ��ü ��ų�� ���� ������ Ʈ���������� �����ִ� ���̾�α�
*	- ����Ÿ�� XML�������� �Ǿ� �ִ�.
*	- PARSER�� MSXML�� �ƴ� EXPAT�� ����Ѵ�.
*	- Observable : CSkillSlot
*
*
* @Warning	�븸������ ��� ��ųƮ��â�� �������� ������ ����Ÿ�� ������� ���� �ʴ�.
* @TODO		MSXML�� ��� ȣȯ�� ������ ���� ������ ������ε� ������� ���������� �޸𸮿��� ȭ���� �ε� �Ҽ� ����..
*			�׷��� XML�� ��� ����ȭ�Ͻý����� �����ϰ� �ִ�..�׷��� �׽�Ʈ���ؼ� EXPAT���� �����Ͽ���. ���߿� TGameCtrl������ Parser�� ��ü�ϴ°� �������ϴ�.
* @Author	������
* @Date		2005/9/15
*/
class CSkillTreeDlg : public CTDialog, public IObserver {
public:
           CSkillTreeDlg(void );
  virtual  ~CSkillTreeDlg(void);
  void     Show() override;
  void     Hide() override;
  void     Draw() override;
  void     MoveWindow(POINT pt) override;
  void     Update(POINT     ptMouse) override;
  unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;

  void Update(CObservable* pObservable, CTObject* pObj) override;

  static CIconSkill* MakeIcon(int skillindex, int level, int level_limit);

private:

  void LevelupSkill(int skillslotindex);
  void AddSkill(int     skillslotindex);
  void RemoveSkill(int  skillslotindex);

  enum {
    IID_BTN_ICONIZE = 10,
    IID_BTN_CLOSE,
    IID_TEXT_SOLDIER = 21,
    IID_TEXT_MUSE,
    IID_TEXT_HOWKER,
    IID_TEXT_DEALER
  };

  s_userdata m_userdata;
};

#endif