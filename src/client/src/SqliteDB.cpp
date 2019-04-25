#include "StdAfx.h"
#include "./SqliteDB.h"

/*
#pragma comment(lib, "sqlite.lib")

CSqliteDB::CSqliteDB(void) {
  m_db = nullptr;
}

CSqliteDB::~CSqliteDB(void) {}

CSqliteDB&         CSqliteDB::GetInstance() {
  static CSqliteDB s_Instance;
  return s_Instance;
}

int CSqliteDB::Open(const char* pszName) {
  assert( pszName );
  assert( m_db == NULL );
  if ( pszName == nullptr || m_db != nullptr )
    return 1;

  char* zErrMsg;
  m_db = sqlite_open( pszName, 0, &zErrMsg );
  if ( m_db == nullptr ) {
    assert( m_db );
    sqlite_freemem( zErrMsg );
    return 1;
  }
  return 0;
}

int CSqliteDB::Close() {
  assert( m_db );
  if ( m_db == nullptr )
    return 1;

  sqlite_close( m_db );
  return 0;
}

int CSqliteDB::Exec(const char* sql, sqlite_callback callback) {
  assert( m_db );
  assert( sql );
  if ( m_db == nullptr )
    return 1;
  if ( sql == nullptr )
    return 1;

  char* zErrMsg;
  int   rc = sqlite_exec( m_db, sql, callback, nullptr, &zErrMsg );
  if ( rc != SQLITE_OK ) {
    //		g_itMGR.OpenMsgBox( zErrMsg );
    sqlite_freemem( zErrMsg );
    return 1;
  }
  return 0;
}

int CSqliteDB::AddMemo(const char* pszName, const char* pszSender, const char* pszContent, DWORD time) {
  assert( m_db );
  if ( m_db == nullptr )
    return 1;

  const char* sqlFormat = "insert into memo values ( '%q', '%q' ,'%q', %d )";
  char*       zErrMsg;
  int         rc = sqlite_exec_printf( m_db, sqlFormat, nullptr, nullptr, &zErrMsg, pszName, pszSender, pszContent, time );
  if ( rc != SQLITE_OK ) {
    sqlite_freemem( zErrMsg );
    return 1;
  }
  return 0;
}

int CSqliteDB::LoadMemo(const char* pszName, sqlite_callback callback) {
  assert( m_db );
  if ( m_db == nullptr )
    return 1;

  const char* sqlFormat = "Select ROWID, sender, content, time from memo where name = '%q'";
  char*       zErrMsg;
  int         rc = sqlite_exec_printf( m_db, sqlFormat, callback, nullptr, &zErrMsg, pszName );
  if ( rc != SQLITE_OK ) {
    sqlite_freemem( zErrMsg );
    return 1;
  }
  return 0;
}

int CSqliteDB::DeleteMemo(int iRowid) {
  assert( m_db );
  if ( m_db == nullptr )
    return 1;

  const char* sqlFormat = "delete from memo where ROWID = '%d'";
  char*       zErrMsg;
  int         rc = sqlite_exec_printf( m_db, sqlFormat, nullptr, nullptr, &zErrMsg, iRowid );
  if ( rc != SQLITE_OK ) {
    sqlite_freemem( zErrMsg );
    return 1;
  }
  return 0;
}

*/