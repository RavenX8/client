#ifndef _ZListBox_
#define _ZListBox_
#include "winctrl.h"
#include "IScrollModel.h"
#include <deque>

//*------------------------------------------------------------------//
///
//*------------------------------------------------------------------//
class TGAMECTRL_API CZListBox : public CWinCtrl, public IScrollModel {
public:
          CZListBox(void );
  virtual ~CZListBox(void);

  void     Draw() override;
  void     MoveWindow(POINT pt) override;
  void     Update(POINT     ptMouse) override;
  void     Show() override;
  void     Hide() override;
  unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;

  void      Add(CWinCtrl* pCtrl);
  void      Clear();
  void      SetSelected(int iIndex);
  void      DeselectAll();
  int       GetSelectedItemIndex();
  CWinCtrl* GetItem(int            iIndex);
  void      InsertItem(int         iIndex, CWinCtrl* pCtrl);
  bool      DelItem(int            iIndex);
  bool      DelItemByControlID(int iID);
  int       GetSize();

  //*---------------------------------------------------//
  /// implemented from IScrollModel
  int GetValue() override;
  int GetExtent() override;
  int GetMaximum() override;
  int GetMinimum() override;

  void SetValue(int  ) override;
  void SetExtent(int ) override;
  void SetMaximum(int) override;
  void SetMinimum(int) override;

  RECT GetWindowRect() override;
  //*---------------------------------------------------//

protected:

  int m_iValue;
  int m_iExtent;

  std::deque<CWinCtrl*> m_Items;
};
#endif
