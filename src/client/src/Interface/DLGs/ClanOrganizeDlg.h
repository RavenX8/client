#ifndef _CClanOrganizeDlg_
#include "tgamectrl/TDialog.h"

/**
* 클랜 창설시 사용되는 다이얼로그
*	- 클랜마크는 처음에는 정해진 이미지중 2개를 선택하여 만들어지며 나중에 별도의 이미지를 만들어서 등록사용할수 있다
* 
* @Author		최종진
* @Date			2005/9/12
* 
*/
class CClanOrganizeDlg : public CTDialog {
public:
           CClanOrganizeDlg(void );
  virtual  ~CClanOrganizeDlg(void);
  bool     Create(const char*    IDD) override;
  void     Draw() override;
  unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;

private:
  void OnLButtonDown(unsigned uiMsg, WPARAM wParam, LPARAM lParam);
  void OrganizeClan();

  enum {
    IID_BTN_CONFIRM = 10,
    IID_BTN_CLOSE = 11,
    IID_BTN_CANCEL = 12,
    IID_EDIT_TITLE = 20,
    IID_EDIT_SLOGAN = 21,
    IID_TABLE_CLANCENTER = 30,
    IID_TABLE_CLANBACK = 40,
  };

  int m_iSelectedClanBack;   /// 선택한 배경이미지 번호
  int m_iSelectedClanCenter; /// 선택한 전면이미지 번호
};
#endif
