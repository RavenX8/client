#ifndef _CDialogNpcScriptAnswerItemNew_
#define _CDialogNpcScriptAnswerItemNew_
#include "tgamectrl/WinCtrl.h"
#include "tgamectrl/JStringParser.h"

/**
* �븸������ ��� NPC ��ȭ�� ������ ������ Scirpt��  ����� ZLISTBOX�� ���� Item Class
* 
* @Warning		���������� �븸������ �ٸ� Item Class�� ����Ѵ�.
* @Author		������
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

  int    m_iEventID;                     //�̺�Ʈ�� ���̵�
  void (*m_fpEventHandle)(int iEventID); //���� ���ý� ó�� �Լ� 
  bool   m_highlight;
};
#endif