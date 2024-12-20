#ifndef _CDialogNpcScriptAnswerItemNew_
#define _CDialogNpcScriptAnswerItemNew_
#include "tgamectrl/WinCtrl.h"
#include "tgamectrl/JStringParser.h"

/**
* 대만버젼의 경우 NPC 대화시 유저가 선택할 Scirpt를  출력할 ZLISTBOX를 위한 Item Class
* 
* @Warning		국내버젼과 대만버젼이 다른 Item Class를 사용한다.
* @Author		최종진
* @Date			2005/9/15
*/

class CDialogNpcScriptAnswerItemNew : public CWinCtrl {
public:
          CDialogNpcScriptAnswerItemNew(int  iIndex, char* pszScript, int iEventID, void (*fpEventHandle)(int iEventID), int iWidth);
  virtual ~CDialogNpcScriptAnswerItemNew(void);

  void     Draw() override;
  unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;
  void     Update(POINT     ptMouse) override;

protected:
  int m_iLineHeight;
  int m_iIndent;
  int m_iIndex;

  CJStringParser m_Script;

  int    m_iEventID;                     //이벤트의 아이디
  void (*m_fpEventHandle)(int iEventID); //예시 선택시 처리 함수 
  bool   m_highlight;
};
#endif
