#ifndef _CNUMBERINPUTDLG_
#define _CNUMBERINPUTDLG_

#include "../../../TGameCtrl/TDialog.h"
class CTCmdNumberInput;

/**
* 아이템의 이동시나 기타 숫자 입력시 사용되는 다이얼로그
*
* @Warning		CTCmdNumberInput를 상속받은 Command만을를 입력후 실행할 Command로 설정할수 있다.
* @Author		최종진
* @Date			2005/9/12
*/

class CNumberInputDlg : public CTDialog {
public:
          CNumberInputDlg(void );
  virtual ~CNumberInputDlg(void);

  void         Update(POINT ptMouse) override;
  void         Show() override;
  void         Hide() override;
  unsigned int Process(UINT uiMsg, WPARAM wParam, LPARAM lParam) override;

  enum {
    IID_EDITBOX = 2,
    IID_BUTTON_CLOSE = 3,
    IID_BTN_DEL = 4,
    IID_BTN_OK = 5,
    IID_BTN_MAX = 6,
    IID_BTN_0 = 10,
    IID_BTN_1 = 11,
    IID_BTN_2 = 12,
    IID_BTN_3 = 13,
    IID_BTN_4 = 14,
    IID_BTN_5 = 15,
    IID_BTN_6 = 16,
    IID_BTN_7 = 17,
    IID_BTN_8 = 18,
    IID_BTN_9 = 19,
  };

  int64_t GetLastInputNumber() { return m_iLastInputNumber; }

  void SetMaxNumber(int64_t         iMaxNumber) { m_iMaxNumber = iMaxNumber; }
  void SetCommand(CTCmdNumberInput* pCmd, CTObject* pObj);

protected:

  void OnLButtonUp(unsigned iProcID);

  void AppendText(char* szText);
  void DeleteOneChar();

  int64_t GetNumberFromEditBox();

  //void		ChangeButtonState( unsigned int iID, int iState ); 사용안함
  void SetFocusEditBox();
  void SendCommand();
  void ClearCommand();

protected:
  CTCmdNumberInput* m_pCmd;          /// 입력 완료시 실행할 CTCommand
  CTObject*         m_pCommandParam; /// m_pCmd 처리에 필요한 인자를 임시 보관

  int64_t m_iLastInputNumber; /// 마지막 입력된 숫자
  int64_t m_iMaxNumber;       /// 최대 입력가능한 숫자
};
#endif
