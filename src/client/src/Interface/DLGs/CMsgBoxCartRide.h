#ifndef _CMSGBOXCARTRIDE_
#define _CMSGBOXCARTRIDE_

#include "StdAfx.h"
#include "./CMsgBox.h"

class CMsgBox_CartRide : public CMsgBox {
public:

          CMsgBox_CartRide();
  virtual ~CMsgBox_CartRide() {}

  void Update(POINT ptMouse) override;

  WORD m_wOwner;
  WORD m_wGuest;
};

#endif
