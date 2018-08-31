#ifndef _CIconQuick_
#define _CIconQuick_
#include "cicon.h"

/**
* 퀵바에 사용되는 Icon, 내부적으로 다른 아이콘을 가지고 있다.
* 
* @Warning		항상 CIcon의 virtual method를 구현해주어야 한다.( 멤버로 가지고 있는 CIcon이 동작하도록 )
* @Author		최종진
* @Date			2005/9/6
*/
class CIconQuick : public CIcon {
public:
          CIconQuick(CIcon* pIcon);
  virtual ~CIconQuick(void  );

  void   Draw() override;
  void   Update(POINT ptMouse) override;
  void   ExecuteCommand() override;
  CIcon* Clone() override;
  void   Clear() override;
  void   SetPosition(POINT pt) override;
  void   GetToolTip(CInfo& ToolTip, DWORD dwDialogType, DWORD dwType) override;
  int    GetIndex() override;

  void         SetQuickBarSlotIndex(int iIndex);
  int          GetQuickBarSlotIndex();
  const CIcon* GetIcon();

protected:
  CIcon* m_pIcon;              /// 실제 모든 처리를 할 아이콘에 대한 포인터
  int    m_iQuickBarSlotIndex; /// 퀵바에서의 슬롯 인덱스
};
#endif
