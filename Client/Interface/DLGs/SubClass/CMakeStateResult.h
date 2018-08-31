#ifndef _CMAKESTATERESULT_
#define _CMAKESTATERESULT_
#include "cmakestate.h"
#include <vector>
class CGuage;

/**
* 제조인터페이스 창의 State Class : 서버로 부터 받은 제조결과를 출력하는 상태
*
* @Author		최종진
* @Date			2005/9/15
*/
class CMakeStateResult : public CMakeState {
public:

               CMakeStateResult(CMakeDLG* pParent);
  virtual      ~CMakeStateResult(void     );
  void         Draw() override;
  void         Update(POINT ptMouse) override;
  void         Show() override;
  void         Hide() override;
  void         Init() override;
  unsigned int Process(UINT     uiMsg, WPARAM wParam, LPARAM lParam) override;
  void         MoveWindow(POINT ptPos) override;
  bool         IsModal() override;

protected:
  std::vector<CGuage*> m_listGuage;
  int                  m_iRedGuageImageID;
  int                  m_iGreenGuageImageID;
  int                  m_dwPrevTime;
  bool                 m_bWaitState; ///메세지 박스가 눌릴때까지 기다린다.

};
#endif
