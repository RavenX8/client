#include "StdAfx.h"
#include "./CSelectChannel.h"
#include "../Command/CTCmdHotExec.h"
#include "../../Network/CNetwork.h"
#include "ExternalUILobby.h"
#include "tgamectrl/TListBox.h"
#include "tgamectrl/TMsgBox.h"

CSelectChannel::CSelectChannel(void) {}

CSelectChannel::~CSelectChannel(void) {}

void CSelectChannel::SetEUIObserver(CExternalUIManager* pObserver) {}

unsigned CSelectChannel::Process(UINT uiMsg, WPARAM wParam, LPARAM lParam) {
  if ( !IsVision() )
    return 0;

  return 0;
}

void CSelectChannel::Show() {
  CTDialog::Show();
}
