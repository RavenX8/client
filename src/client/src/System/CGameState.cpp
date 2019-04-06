#include "StdAfx.h"
#include "./CGameState.h"
#include "../CApplication.h"

CGameState::CGameState(void) {
  m_iStateID = 0;
}

CGameState::~CGameState(void) {}

void CGameState::ServerDisconnected() {
  g_pCApp->SetExitGame();
}

int CGameState::ProcWndMsgInstant(unsigned uiMsg, WPARAM wParam, LPARAM lParam) {
  return 0;
}

int CGameState::Update(bool bLostFocus) {
  _ASSERT( 0 && "CGameState::Update" );
  *(int*)nullptr = 10;
  return 0;
}

int CGameState::Enter(int iPrevStateID) {
  _ASSERT( 0 && "CGameState::Enter" );
  *(int*)nullptr = 10;
  return 0;
}

int CGameState::Leave(int iNextStateID) {
  _ASSERT( 0 && "CGameState::Leave" );
  *(int*)nullptr = 10;
  return 0;
}

int CGameState::ProcMouseInput(UINT uiMsg, WPARAM wParam, LPARAM lParam) {
  _ASSERT( 0 && "CGameState::ProcMouseInput" );
  *(int*)nullptr = 10;
  return 0;
}

int CGameState::ProcKeyboardInput(UINT uiMsg, WPARAM wParam, LPARAM lParam) {
  _ASSERT( 0 && "CGameState::ProcKeyboardInput" );
  *(int*)nullptr = 10;
  return 0;
}
