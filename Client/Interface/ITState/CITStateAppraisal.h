#ifndef _CITStateAppraisal_
#define _CITStateAppraisal_
#include "citstate.h"

/**
* 아이템감정을 시작해서 유저의 입력(마우스클릭)을 대기 하는 인터페이스(IT_MGR) 상태
*
* @Author		최종진
*
* @Date			2005/9/6
*
*/
class CITStateAppraisal : public CITState {
public:
          CITStateAppraisal(void );
  virtual ~CITStateAppraisal(void);

  void     Enter() override;
  void     Leave() override;
  unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;
  void     Update(POINT     ptMouse) override {}
};
#endif
