#ifndef _CQuestListItem_
#define _CQuestListItem_
#include "winctrl.h"

#include "SinglelineString.h"

/**
* 퀘스트창에서 퀘스트목록 ZListBox의 item Class
*
* @Author	최종진
* @Date		2005/9/15
*/
class CQuestListItem : public CWinCtrl {
public:
           CQuestListItem(int  iQuestIndex);
  virtual  ~CQuestListItem(void);
  void     Draw() override;
  unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;
  void     Update(POINT     ptMouse) override;
  void     SetSelected() override;

protected:
  int               m_iQuestIndex;
  CSinglelineString m_title;
};
#endif
