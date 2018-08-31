#ifndef _CITStateWaitDisconnect_
#define _CITStateWaitDisconnect_
#include "citstate.h"

/**
* 서버에게 로그아웃을 알리고 Disconnect 될때까지 대기하는 Interface State Class
*
* @Author		최종진
* @Date			2005/9/6
*/
class CITStateWaitDisconnect : public CITState {
public:
          CITStateWaitDisconnect(void );
  virtual ~CITStateWaitDisconnect(void);

  void     Enter() override;
  void     Leave() override;
  unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;
  void     Update(POINT     ptMouse) override;
protected:
  DWORD m_dwEnterTime;
  DWORD m_dwPrevUpdateTime;
  int   m_iWaitDisconnectedTime;
};
#endif
