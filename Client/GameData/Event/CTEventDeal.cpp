#include "StdAfx.h"
#include "CTEventDeal.h"
#include "../../GameCommon/Item.h"

CTEventDeal::CTEventDeal(void) {
  m_strString = "CTEventDeal";
  m_pItem     = nullptr;
}

CTEventDeal::~CTEventDeal(void) {
  m_strString.clear();
}

void CTEventDeal::SetItem(CItemFragment* pItem) {
  m_pItem = pItem;
}

CItemFragment* CTEventDeal::GetItem() {
  return m_pItem;
}
