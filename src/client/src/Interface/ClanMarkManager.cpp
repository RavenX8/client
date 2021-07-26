#include "StdAfx.h"
#include "./ClanMarkManager.h"
#include "../Game.h"
#include "ClanMarkTransfer.h"
#include "Util/classCRC.h"

//CClanMarkManager _ClanMarkManager;

CClanMarkManager::CClanMarkManager(void) {}

CClanMarkManager::~CClanMarkManager(void) {}

//------------------------------------------------------------------
/// return clan mark. If there is not HDD, request to server...
//------------------------------------------------------------------
CClanMarkUserDefined*   CClanMarkManager::GetClanMark(const char* pstrName, WORD crc16, int iClanID) {
  CClanMarkUserDefined* pMark = SearchTexture( pstrName );

  if ( pMark == nullptr ) {
    pMark = GetUserdefinedClanMark( pstrName, crc16, iClanID );
  }

  return pMark;
}

CClanMarkUserDefined*   CClanMarkManager::GetUserdefinedClanMark(const char* pstrName, WORD crc16, int iClanID) {
  CClanMarkUserDefined* ClanMark = new CClanMarkUserDefined;
  ClanMark->SetName( pstrName );
  ClanMark->SetNode( NULL );
  ClanMark->SetFileCRC( crc16 );
  ClanMark->SetClanID( iClanID );

  m_TextureNodePool.push_back( ClanMark );

  return ClanMark;
}

void                    CClanMarkManager::ReloadTexture(const char* FileName, WORD wCRC16) {
  CClanMarkUserDefined* pMark = SearchTexture( FileName );

  if ( pMark != nullptr ) {
    pMark->Free();
    HNODE hNode = LoadNewTexture( FileName, wCRC16 );
    if ( hNode != 0 ) {
      pMark->SetName( FileName );
      pMark->SetNode( hNode );
    }
  }
}

//------------------------------------------------------------------
/// Ư���� �ð����� ������ ���� ������ ����
//------------------------------------------------------------------
const int GC_BOUNDARY_TIME = 100000;

void    CClanMarkManager::UpdatePool() {
  DWORD dwCurrentTime = g_GameDATA.GetGameTime();

  CClanMarkUserDefined*                      pUserDefinedMark;
  std::list<CClanMarkUserDefined*>::iterator iter;
  for ( iter         = m_TextureNodePool.begin(); iter != m_TextureNodePool.end(); ) {
    pUserDefinedMark = *iter;

    DWORD dwLastUsedTime = pUserDefinedMark->GetLastUsedTime();

    /// ���ӿ��� ������ ���� ��� Ÿ���� �ʰ��� �͵鿡 ���ؼ���..
    if ( pUserDefinedMark->GetRefCount() <= 0 && dwCurrentTime - dwLastUsedTime > GC_BOUNDARY_TIME ) {
      iter = m_TextureNodePool.erase( iter );
      delete pUserDefinedMark;
      continue;
    }

    /// �ε��ؾ��� �ؽ��İ� �ִٸ� �ε�...
    if ( pUserDefinedMark->IsLoaded() == false ) {
      LoadRealTexture( pUserDefinedMark );
    }

    ++iter;
  }
}

//------------------------------------------------------------------
/// �ؽ��� �ε� �÷��װ� ���� ���鿡 ���� �ؽ��� �ε�..
//------------------------------------------------------------------
void    CClanMarkManager::LoadRealTexture(CClanMarkUserDefined* pUserDefinedMark) {
  HNODE hNode = LoadNewTexture( pUserDefinedMark->GetName().c_str(), pUserDefinedMark->GetFileCRC() );
  if ( hNode == 0 ) {
    /// ������ ��û..			
    CClanMarkTransfer::GetSingleton().RequestMarkFromServer( pUserDefinedMark->GetClanID() );
  }

  pUserDefinedMark->SetLoadFlag( true );
  pUserDefinedMark->SetNode( hNode );
}

//------------------------------------------------------------------
/// Search texture form pool.
//------------------------------------------------------------------
CClanMarkUserDefined* CClanMarkManager::SearchTexture(const char* pstrName) {
  assert( pstrName );
  std::list<CClanMarkUserDefined*>::iterator iter;
  for ( iter = m_TextureNodePool.begin(); iter != m_TextureNodePool.end(); ++iter ) {
    if ( _strcmpi( (*iter)->GetName().c_str(), pstrName ) == 0 )
      return *iter;
  }
  return nullptr;
}

//--------------------------------------------------------------------
/// Load new texture form HDD.
/// �̸����� �ش� ȭ���� �ִ����� ã�� ���ϴ� ���˿� �´����� üũ�Ѵ�.
//--------------------------------------------------------------------
HNODE CClanMarkManager::LoadNewTexture(const char* pstrName, WORD crc16) {
  assert( pstrName );
  HNODE hNode = NULL;

  /// File exist check
  HANDLE hClanMarkFile = CreateFile( pstrName, GENERIC_READ,FILE_SHARE_READ, nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_READONLY, nullptr );
  if ( hClanMarkFile == INVALID_HANDLE_VALUE ) {
    DWORD dwErr = GetLastError();
    _RPTF2( _CRT_WARN,"CreateFile Error(OPEN_EXISTING):%s %d", pstrName, dwErr );
    return NULL;
  }

  /// Get File Size;
  DWORD FileSizeHigh, FileSizeLow;
  FileSizeLow = GetFileSize( hClanMarkFile, &FileSizeHigh );
  if ( FileSizeLow == INVALID_FILE_SIZE ) {
    DWORD dwErr = GetLastError();
    if ( FileSizeHigh == 0 ) {
      _RPTF1( _CRT_WARN,"GetFileSize Error(%d)", dwErr );
    } else {
      if ( dwErr != NO_ERROR ) {
        _RPTF0( _CRT_WARN,"GetFileSize Error" );
      }
    }
    CloseHandle( hClanMarkFile );
    return NULL;
  }

  /// File Load
  BYTE* buffer = new BYTE[ FileSizeLow ];
  DWORD NumberOfBytesRead;
  if ( FALSE == ReadFile( hClanMarkFile, buffer, FileSizeLow, &NumberOfBytesRead, nullptr ) ) {
    delete []buffer;
    CloseHandle( hClanMarkFile );
    return NULL;
  }
  assert( FileSizeLow == NumberOfBytesRead );

  WORD wCrc16 = classCRC::DataCRC16( buffer, FileSizeLow );

  delete []buffer;
  CloseHandle( hClanMarkFile );

  /// CRC16 Check
  if ( wCrc16 != crc16 )
    return NULL;

  //HNODE h = findNode ( pstrName );
  hNode = loadTexture( pstrName, pstrName, 1, 0 );
  return hNode;
}

void                                         CClanMarkManager::Free() {
  CClanMarkUserDefined*                      userDefinedMark;
  std::list<CClanMarkUserDefined*>::iterator iter;
  for ( iter        = m_TextureNodePool.begin(); iter != m_TextureNodePool.end(); ) {
    userDefinedMark = *iter;
    iter            = m_TextureNodePool.erase( iter );
    delete userDefinedMark;
  }
}
