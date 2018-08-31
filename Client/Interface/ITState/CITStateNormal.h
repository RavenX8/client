#ifndef _CITStateNormal_
#define _CITStateNormal_
#include "citstate.h"

/**
* Normal Interface(IT_MGR) State
*
* @Author		√÷¡æ¡¯
*
* @Date			2005/9/6
*/
class CITStateNormal : public CITState {
public:
          CITStateNormal(void );
  virtual ~CITStateNormal(void);

  void     Enter() override;
  void     Leave() override {}
  unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;
  void     Update(POINT     ptMouse) override {}
private:
  bool ProcessHotKeyNormalInput(unsigned uiMsg, WPARAM wParam, LPARAM lParam);
  bool ProcessHotKeyAutoEnter(unsigned   uiMsg, WPARAM wParam, LPARAM lParam);
};
#endif
