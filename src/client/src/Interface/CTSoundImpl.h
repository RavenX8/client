#ifndef _CTSOUNDIMPL_
#define _CTSOUNDIMPL_

#include "tgamectrl/ITSound.h"

/**
* �������̽� ���̺귯��(TGameCtrl)���� �˷��� ITSound�� Implemented Class
*  : TGameCtrl���������� ���带 �÷����Ҷ� �� �������̽��� ���ؼ� ȣ���Ѵ�. 
*
* @Author		������
*
* @Date			2005/9/5
*/

class CTSoundImpl : public ITSound {
public:
  CTSoundImpl(void );
  ~CTSoundImpl(void);

  void Playsound(int i) override;
};

extern CTSoundImpl g_SoundImpl;

#endif