#ifndef _TARGETOBJECTDLG_
#define _TARGETOBJECTDLG_

#include "tgamectrl/TDialog.h"

/**
* 예전 클베때 타겟에 대한 정보를 보여주는 다이얼로그
*
* @Warning		현재( 2005/9/15)사용하지 않고 있다.
* @Author		최종진
* @Date			2005/9/15
*/
class CTargetObjectDLG : public CTDialog {

public:
          CTargetObjectDLG();
  virtual ~CTargetObjectDLG();

  // *---------------------------------------------------* //
  // Overrided function
  // *---------------------------------------------------* //
  void         Draw() override;
  unsigned int Process(UINT uiMsg, WPARAM wParam, LPARAM lParam) override;
  // *---------------------------------------------------* //

};

#endif //_TARGETOBJECTDLG_
