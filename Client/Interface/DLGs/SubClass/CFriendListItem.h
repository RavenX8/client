#ifndef _CFriendListItem_
#define _CFriendListItem_
#include "winctrl.h"

/**
* Ŀ�´�Ƽ â���� ģ�� ����Ʈ�� ���Ǵ� Item Class
*
* @Author		������
* @Date			2005/9/15
*/
class CFriendListItem : public CWinCtrl {
public:
           CFriendListItem(DWORD dwUserTag, BYTE btStatus, const char* pszName);
  virtual  ~CFriendListItem(void );
  void     Draw() override;
  unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;
  void     SetSelected() override;

  void SetStatus(BYTE btStatus);

  const DWORD GetUserTag();
  const char* GetName();
  const BYTE  GetStatus();

protected:
  DWORD       m_dwUserTag;
  BYTE        m_btStatus;
  std::string m_strName;
  int         m_online_image_id;
  int         m_offline_image_id;
};
#endif
