#include "StdAfx.h"
#include "./CCommunity.h"

CCommunity::CCommunity(void) {}

CCommunity::~CCommunity(void) {}

CCommunity&         CCommunity::GetInstance() {
  static CCommunity s_Instance;
  return s_Instance;
}
