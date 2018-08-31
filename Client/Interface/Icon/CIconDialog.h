#ifndef _CIconDialog_
#define _CIconDialog_
#include "cicon.h"
class CDragItem;

/**
* 인터페이스의 다이얼로그가 아이콘화 되었을때 사용되는 Icon
*
* @Warning		CreateIcon에서 만든후 파괴는 알아서;
* @Author		최종진
* @Date			2005/9/6
*/
class CIconDialog : public CIcon {
public:
          CIconDialog();
  virtual ~CIconDialog(void);

  void                Update(POINT     ptMouse) override;
  unsigned            Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;
  void                ExecuteCommand() override;
  CIcon*              Clone() override;
  void                GetToolTip(CInfo& ToolTip, DWORD dwDialogType, DWORD dwType) override;
  void                SetPosition(POINT pt) override;
  int                 GetIndex() override;
  static CIconDialog* CreateIcon(int iDialogType, bool bCreateDragItem);

  void SetDialogType(int iDialogType);
  int  GetDialogType();

  void       SetDragItem(CDragItem* pDragItem);
  CDragItem* GetDragItem();

protected:
  int        m_iDialogType;
  CDragItem* m_pDragItem;
  bool       m_bClicked;
  POINT      m_ptClicked;
};
#endif
