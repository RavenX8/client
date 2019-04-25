#include "StdAfx.h"
#include "./CTEvent.h"

CTEvent::CTEvent(void) {
  m_iID = 0;

}

CTEvent::~CTEvent(void) {}

int CTEvent::GetID() {
  return m_iID;
}

void CTEvent::SetID(int iID) {
  m_iID = iID;
}
