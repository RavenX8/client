#ifndef _ChatRoomListItem_
#define _ChatRoomListItem_
#include "tgamectrl/WinCtrl.h"

/**
* 커뮤니티 창의 대화방 List에 사용되는 item
*
* @Author		최종진
* @Date			2005/9/15
*/

class CChatRoomListItem : public CWinCtrl {
public:
           CChatRoomListItem(WORD wID, BYTE btRoomType, BYTE btUserCount, const char* pszTitle);
  virtual  ~CChatRoomListItem(void);
  void     Draw() override;
  unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;
  void     SetSelected() override;

protected:
  WORD        m_wID;
  BYTE        m_btRoomType;
  BYTE        m_btUserCount;
  std::string m_strTitle;

};
#endif
