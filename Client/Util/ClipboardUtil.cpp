#include "StdAfx.h"
#include <string>
#include "./ClipboardUtil.h"

CClipboardUtil::CClipboardUtil(void) {}

CClipboardUtil::~CClipboardUtil(void) {}

void      CClipboardUtil::CopyStringtoClibboard(std::string istr) {
  char*   ptr;
  HGLOBAL hmem;
  hmem = GlobalAlloc( GHND, istr.size() + 1 );
  ptr  = (char *)GlobalLock( hmem );
  strcpy( ptr, istr.c_str() );
  GlobalUnlock( hmem );
  if ( OpenClipboard( nullptr ) ) {
    EmptyClipboard();
    SetClipboardData( CF_TEXT, hmem );
    CloseClipboard();
  }
}

std::string   CClipboardUtil::GetStringFromClibboard() {
  HGLOBAL     hmem;
  char*       ptr;
  std::string str;

  if ( IsClipboardFormatAvailable( CF_TEXT ) ) {
    OpenClipboard( nullptr );
    hmem = GetClipboardData( CF_TEXT );
    ptr  = (char *)GlobalLock( hmem );

    str = std::string( ptr );
    GlobalUnlock( hmem );
    CloseClipboard();
  }
  return str;
}
