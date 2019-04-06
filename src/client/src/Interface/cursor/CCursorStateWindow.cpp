#include "StdAfx.h"
#include "./CCursorStateWindow.h"
#include "CCursor.h"

CCursorStateWindow::CCursorStateWindow(void) {
  m_iState = CCursor::STATE_WINDOW;
}

CCursorStateWindow::~CCursorStateWindow(void) {}

int CCursorStateWindow::SetCursorType(int iType) {
  //ShowCursor( TRUE );
  return 0;
}
