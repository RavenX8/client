#include "StdAfx.h"
#include "./IObserver.h"

void IObserver::Update(CObservable* pObservable, CTObject* pObj) {
  assert( 0 && "Pure virtual function IObserver::Update" );
  *(int*)nullptr = 10;
}
