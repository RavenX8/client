#ifndef _CDialogNpcScriptAnswerItem_
#define _CDialogNpcScriptAnswerItem_

#include "tgamectrl/JStringParser.h"

#include "tgamectrl/WinCtrl.h"
#include "tgamectrl/TSplitString.h"

/**
* NPC ��ȭ�� ������ ������ Scirpt��  ����� ZLISTBOX�� ���� Item Class
* 
* @Warning		���������� �븸������ �ٸ� Item Class�� ����Ѵ�.
* @Author		������
* @Date			2005/9/15
*/
class CDialogNpcScriptAnswerItem : public CWinCtrl {
public:
           CDialogNpcScriptAnswerItem(int  iIndex, char* pszScript, int iEventID, void (*fpEventHandle)(int iEventID), int iImageID, int iLineHeight, int iWidth);
  virtual  ~CDialogNpcScriptAnswerItem(void);
  void     Draw() override;
  unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;
  void     Update(POINT     ptMouse) override;

protected:
  int m_iBorderWidth;
  int m_iLineHeight;
  int m_iIndent;
  int m_iIndex;
  int m_iModuleID;
  int m_iImageID;

  CJStringParser m_Script;

  int    m_iEventID;                     //�̺�Ʈ�� ���̵�
  void (*m_fpEventHandle)(int iEventID); //���� ���ý� ó�� �Լ� 
  bool   m_highlight;

};
#endif