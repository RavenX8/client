#include "StdAfx.h"
#include "./SystemInfo.h"

class CSystemInfo g_SystemInfo;

CSystemInfo::CSystemInfo(void) {
  m_OSVerInfo.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
}

CSystemInfo::~CSystemInfo(void) {}

void CSystemInfo::CollectingSystemInfo() {
  // Get system language
  GetLocaleInfo( LOCALE_SYSTEM_DEFAULT, LOCALE_SENGLANGUAGE,
                 m_szLanguageBuffer, sizeof(m_szLanguageBuffer) );

//  GetVersionEx( &m_OSVerInfo );
//
//  switch(m_OSVerInfo.dwMajorVersion) {
//  case 4: //windows 98/NT
//    m_iWindowsVersion = WINDOWS_98;
//  case 5: //windows 2000
//    m_iWindowsVersion = WINDOWS_2000;
//  case 6: //windows 8+
//  default:
    m_iWindowsVersion = WINDOWS_2000;
//    break;
//  }

  GetWindowsDirectory( m_szWindowsFolder, sizeof(m_szWindowsFolder) );
}
