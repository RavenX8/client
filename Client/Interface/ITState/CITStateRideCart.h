#ifndef _CITStateRideCart_
#define _CITStateRideCart_
#include "CITState.h"

//----------------------------------------------------------------------------------
/// @brief 2인승 카트 보조석에 탔을때의 인터페이스 상태
//----------------------------------------------------------------------------------
class CITStateRideCart : public CITState {
public:
          CITStateRideCart(void );
  virtual ~CITStateRideCart(void);

  void     Enter() override;
  void     Leave() override;
  unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;
  void     Update(POINT     ptMouse) override {}

};
#endif
