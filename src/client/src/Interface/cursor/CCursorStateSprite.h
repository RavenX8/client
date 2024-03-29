#ifndef _CCursorStateSprite_
#define _CCursorStateSprite_

#include "ccursorstate.h"

/**
* 매프레임 이미지를 Draw하는 State Class( 기본 커서만 있음 )
* 
*
* @Author		최종진
*
* @Date			2005/9/6
*/

class CCursorStateSprite : public CCursorState {
public:
          CCursorStateSprite(void );
  virtual ~CCursorStateSprite(void);
  void    Enter() override;
  void    Leave() override;
  void    Draw(POINT        ptMouse) override;
  int     SetCursorType(int iType) override;

private:
  HNODE m_hTexture;
};
#endif
