#include "StdAfx.h"
#include "./CMakeState.h"
#include "../MakeDLG.h"

CMakeState::CMakeState(void) {
  m_pParent = nullptr;
}

CMakeState::~CMakeState(void) {}

void CMakeState::Draw() {
  _ASSERT( 0 && "CMakeState::Draw()" );
  *(int*)nullptr = 10;
};

void CMakeState::Update(POINT ptMouse) {
  _ASSERT( 0 && "CMakeState::Update" );
  *(int*)nullptr = 10;
}

void CMakeState::Show() {
  _ASSERT( 0 && "CMakeState::Show" );
  *(int*)nullptr = 10;
}

void CMakeState::Hide() {
  _ASSERT( 0 && "CMakeState::Hide" );
  *(int*)nullptr = 10;
}

void CMakeState::Init() {
  _ASSERT( 0 && "CMakeState::Init" );
  *(int*)nullptr = 10;
}

void CMakeState::MoveWindow(POINT ptPos) {
  _ASSERT( 0 && "CMakeState::MoveWindow" );
  *(int*)nullptr = 10;
}

unsigned int CMakeState::Process(UINT uiMsg, WPARAM wParam, LPARAM lParam) {
  _ASSERT( 0 && "CMakeState::Process" );
  *(int*)nullptr = 10;
  return 0;
}

bool CMakeState::IsModal() {
  _ASSERT( 0 && "CMakeState::IsModal() is Pure Virtual Function" );
  *(int*)nullptr = 10;
  return false;
}
