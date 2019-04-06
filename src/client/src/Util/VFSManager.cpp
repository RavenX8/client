#include "StdAfx.h"
#include "VFSManager.h"
#include "CFileSystemNormal.h"
#include "CFileSystemTriggerVFS.h"

#include <algorithm>

CVFSManager __SingletonVFSManager;

CVFSManager::CVFSManager() {
  // Construct
  m_hVFile   = nullptr;
  m_iVFSType = VFS_NORMAL;
}

CVFSManager::~CVFSManager() {
  ReleaseAll();
}

/// Get current file system
CFileSystem*   CVFSManager::GetNewFileSystem(int iVFSType) {
  CFileSystem* pFileSystem = nullptr;

  switch ( iVFSType ) {
    case VFS_NORMAL: {
      pFileSystem = (CFileSystem*)new CFileSystemNormal();
    }
    break;
    case VFS_TRIGGER_VFS: {
      if ( m_hVFile == nullptr ) {
        pFileSystem = (CFileSystem*)new CFileSystemNormal();
        break;
      }

      CFileSystemTriggerVFS* pFsVFS = new CFileSystemTriggerVFS();
      pFsVFS->SetVFS( m_hVFile );
      pFileSystem = (CFileSystem*)pFsVFS;

    }
    break;
  }

  return pFileSystem;
}

bool CVFSManager::InitVFS(int iVFSType, int iReserveCount /*=10*/) {
  ReleaseAll();

  m_iVFSType = iVFSType;

  if ( iVFSType == VFS_TRIGGER_VFS && m_hVFile == nullptr ) {
    if ( g_pCApp )
      g_pCApp->ErrorBOX( "먼저 VFS를 설정하시오", "ERROR" );
    return false;
  }

  CFileSystem* pFileSystem = nullptr;
  for ( int    i           = 0; i < iReserveCount; i++ ) {
    pFileSystem            = GetNewFileSystem( iVFSType );

    if ( pFileSystem == nullptr ) {
      ReleaseAll();
      return false;
    }

    m_VFSList.push_back( pFileSystem );
  }

  return true;
}

///util functio for for_each
void ReleaseSingleVFS(CFileSystem* pFileSystem) {
  if ( pFileSystem != nullptr ) {
    delete pFileSystem;
    pFileSystem = nullptr;
  }
}

void CVFSManager::ReleaseAll() {
  std::for_each( m_VFSList.begin(), m_VFSList.end(), ReleaseSingleVFS );
  std::for_each( m_VFSUsedList.begin(), m_VFSUsedList.end(), ReleaseSingleVFS );

  m_VFSList.clear();
  m_VFSUsedList.clear();
}

CFileSystem*   CVFSManager::GetFileSystem() {
  CFileSystem* pFileSystem = nullptr;

  if ( m_VFSList.empty() ) {
    pFileSystem = GetNewFileSystem( m_iVFSType );
    m_VFSList.push_back( pFileSystem );
  }

  pFileSystem = m_VFSList.back();
  m_VFSList.pop_back();

  m_VFSUsedList.push_back( pFileSystem );

  return pFileSystem;
}

void CVFSManager::ReturnToManager(CFileSystem* pFileSystem) {
  m_VFSList.push_back( pFileSystem );

  m_VFSUsedList.remove( pFileSystem );
}

bool CVFSManager::IsExistFile(const char* pFileName) {
  if ( pFileName == nullptr )
    return false;

  CFileSystem* pFileSystem = GetFileSystem();
  if ( pFileSystem == nullptr )
    return false;

  return pFileSystem->IsExist( pFileName );
}
