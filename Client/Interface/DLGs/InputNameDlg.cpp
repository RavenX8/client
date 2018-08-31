#include "StdAfx.h"
#include "./InputNameDlg.h"
#include "../Command/UICommand.h"
#include "TEditBox.h"

CInputNameDlg::CInputNameDlg(void) {
  m_command = nullptr;
}

CInputNameDlg::~CInputNameDlg(void) {
  SAFE_DELETE( m_command );
}

void CInputNameDlg::Hide() {
  CTDialog::Hide();
  SAFE_DELETE( m_command );
}

void CInputNameDlg::SetCommand(CTCmdInputName* command) {
  assert( command );
  assert( m_command == NULL );
  m_command = command;
}

unsigned CInputNameDlg::Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) {
  if ( !IsVision() ) return 0;
  if ( unsigned uiProcID = CTDialog::Process( uiMsg, wParam, lParam ) ) {
    switch ( uiMsg ) {
      case WM_LBUTTONUP: {
        switch ( uiProcID ) {
          case IID_BTN_CLOSE: Hide();
            break;
          case IID_BTN_CONFIRM: if ( ExecuteCommand() )
              Hide();
            break;
          default: break;
        }
        break;
      }
      default: break;
    }
    return uiMsg;
  }
  return 0;
}

bool CInputNameDlg::ExecuteCommand() {
  if ( m_command == nullptr ) return false;

  CWinCtrl* pCtrl = Find( IID_EDITBOX );
  assert( pCtrl );
  if ( pCtrl == nullptr ) return false;

  assert( pCtrl->GetControlType() == CTRL_EDITBOX );
  if ( pCtrl->GetControlType() != CTRL_EDITBOX ) return false;

  CTEditBox* pEditBox = (CTEditBox*)pCtrl;

  const char* pszName = pEditBox->get_text();
  if ( pszName && strlen( pszName ) > 0 ) {
    m_command->SetName( pszName );
    m_command->Exec( nullptr );
    SAFE_DELETE( m_command );
    return true;
  }
  return false;
}
