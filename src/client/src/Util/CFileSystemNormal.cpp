#include "StdAfx.h"
#include "CFileSystemNormal.h"
#include <assert.h>

CFileSystemNormal::CFileSystemNormal(void) : m_pFile( nullptr ), m_pData( nullptr ) {
  m_pData = nullptr;
  m_iSize = 0;
};

CFileSystemNormal::~CFileSystemNormal(void) {
  // Destruct
}

bool CFileSystemNormal::OpenFile(const char* fname, int iMode) {
  m_strFileName = std::string( fname );

  switch ( iMode ) {
    case OPEN_CREATE_BIN: //"wb"
      m_pFile = fopen( fname, "wb" );
      break;
    case OPEN_READ_BIN: //"rb"
      m_pFile = fopen( fname, "rb" );
      break;
    case OPEN_APPEND_BIN: //"ab"
      m_pFile = fopen( fname, "ab" );
      break;

    case OPEN_CREATE_TEXT: //"wt"
      m_pFile = fopen( fname, "wt" );
      break;
    case OPEN_READ_TEXT: //"rt"
      m_pFile = fopen( fname, "rt" );
      break;
    case OPEN_APPEND_TEXT: //"at"
      m_pFile = fopen( fname, "at" );
      break;
  }

  if ( m_pFile == nullptr ) {
    m_strFileName = std::string( "" );
    return false;
  }

  if ( m_pData != nullptr ) {
    delete m_pData;
    m_pData = nullptr;
  }

  m_iSize = 0;

  return true;
}

void CFileSystemNormal::CloseFile() {
  if ( m_pFile ) {
    fclose( m_pFile );
    m_pFile = nullptr;
  }

  m_strFileName = std::string( "" );
}

void CFileSystemNormal::ReleaseData() {
  if ( m_pData != nullptr ) {
    delete[] m_pData;
    m_pData = nullptr;
  }
}

////////////////////////////////////////////////////////////
// Overload function
int CFileSystemNormal::Read(void* lpBuf, unsigned int nCount) {
  assert( lpBuf && "Read failed from FileStream, output buffer is null" );
  assert( m_pFile && "Read failed from FileStream, file description is null" );
  if ( m_pFile == nullptr || lpBuf == nullptr ) {
    return 0;
  }

  memset( lpBuf, 0, sizeof( char ) * nCount );
  return fread( lpBuf, 1, nCount, m_pFile );
}

void CFileSystemNormal::Write(const void* lpBuf, unsigned int nCount) {
  assert( lpBuf && "Write failed from FileStream, input buffer is null" );
  assert( m_pFile && "Write failed from FileStream, file description is null" );

  if ( m_pFile != nullptr && lpBuf != nullptr ) {
    memset( const_cast<void*>(lpBuf), 0, sizeof(nCount) );
    fwrite( lpBuf, 1, nCount, m_pFile );
  }
}

bool CFileSystemNormal::Seek(long lOff, unsigned int nFrom) {
  assert( m_pFile && "Seek failed from FileStream, file description is null" );
  if ( m_pFile == nullptr )
    return false;

  int iResult = 1;

  switch ( nFrom ) {
    case FILE_POS_SET: iResult = fseek( m_pFile, lOff, SEEK_SET );
      break;
    case FILE_POS_CUR: iResult = fseek( m_pFile, lOff, SEEK_CUR );
      break;
    case FILE_POS_END: iResult = fseek( m_pFile, lOff, SEEK_END );
      break;
  }

  if ( iResult )
    return false;

  return true;
}

long CFileSystemNormal::Tell() {
  assert( ( m_pFile != NULL ) && "file desc is null" );
  return ftell( m_pFile );
}

bool CFileSystemNormal::IsEOF() {
  assert( ( m_pFile != NULL ) && "file desc is null" );
  return feof( m_pFile );
}

int CFileSystemNormal::ReadStringByNullLength() {
  assert( ( m_pFile != NULL ) && "Read string failed from FileStream, file description is null" );

  if ( m_pFile == nullptr )
    return 0;

  long lFilePtr = this->Tell();

  char cOneChar;
  Read( &cOneChar, sizeof( char ) );

  int j = 0;
  while ( cOneChar != '\0' ) {
    j++;
    Read( &cOneChar, sizeof( char ) );
  }

  this->Seek( lFilePtr, FILE_POS_SET );

  return j;
}

int CFileSystemNormal::ReadStringByNull(char* lpBuf) {
  assert( ( lpBuf != NULL ) && "Read string failed from FileStream, Output buffer is null" );
  assert( ( m_pFile != NULL ) && "Read string failed from FileStream, file description is null" );

  if ( lpBuf == nullptr || m_pFile == nullptr )
    return 0;

  char cOneChar;
  Read( &cOneChar, sizeof( char ) );

  int j = 0;
  while ( cOneChar != '\0' ) {
    lpBuf[j++] = cOneChar;
    Read( &cOneChar, sizeof( char ) );
  }
  lpBuf[j] = '\0';

  return j;
}

void CFileSystemNormal::WriteStringByNull(const char* pStr) {
  assert( ( pStr != NULL ) && "Write string to FileStream failed , Input string is null" );
  assert( m_pFile && "Write string to FileStream failed, file description is null" );

  if ( pStr == nullptr || m_pFile == nullptr )
    return;

  int j = 0;
  while ( pStr[j] != '\0' ) {
    Write( &(pStr[j]), sizeof( char ) );
    j++;
  }
  Write( &(pStr[j]), sizeof( char ) );
}

int CFileSystemNormal::ReadPascalStringLength() {
  assert( m_pFile && "Write string to FileStream failed, file description is null" );

  if ( m_pFile == nullptr )
    return 0;

  long lFilePtr = this->Tell();

  int  iLength;
  BYTE btLength;
  ReadByte( &btLength );

  /// 한바이트를 사용하는가? 두바이트를 사용하는가?

  /// 두바이트를 사용한다.
  if ( btLength & 0x80 ) {
    BYTE btSecondByte = 0;
    ReadByte( &btSecondByte );

    int iSecondLength = btSecondByte;
    int iFirstLength  = btLength;

    iLength = (iSecondLength << 7) | (iFirstLength - 0x80);
  } else {
    iLength = btLength;
  }

  this->Seek( lFilePtr, FILE_POS_SET );

  return iLength;
}

int CFileSystemNormal::ReadPascalString(char* lpBuf, int iBufferLength) {
  assert( ( lpBuf != NULL ) && "Write string to FileStream failed , Input string is null" );
  assert( m_pFile && "Write string to FileStream failed, file description is null" );

  if ( lpBuf == nullptr || iBufferLength == 0 || m_pFile == nullptr )
    return 0;

  int  iLength;
  BYTE btLength;
  ReadByte( &btLength );

  /// 한바이트를 사용하는가? 두바이트를 사용하는가?

  /// 두바이트를 사용한다.
  if ( btLength & 0x80 ) {
    BYTE btSecondByte = 0;
    ReadByte( &btSecondByte );

    int iSecondLength = btSecondByte;
    int iFirstLength  = btLength;

    iLength = (iSecondLength << 7) | (iFirstLength - 0x80);
  } else {
    iLength = btLength;
  }

  if ( iLength > iBufferLength ) {
    assert( 0 && "Input buffer is not enough to fill data that readed from file( Pascal string )" );
    return 0;
  }

  Read( lpBuf, iLength );
  lpBuf[iLength] = '\0';

  return iLength;
}

void CFileSystemNormal::WritePascalString(const char* pStr) {
  assert( ( pStr != NULL ) && "Write string to FileStream failed , Input string is null" );
  assert( m_pFile && "Write string to FileStream failed, file description is null" );

  if ( pStr == nullptr || m_pFile == nullptr )
    return;

  short nLength = (short)strlen( pStr );

  /// 두바이트로 길이를 표현한다.
  if ( nLength > 0x7F ) {
    BYTE btFirstLength = (nLength & 0x1111111) | 0x10000000;
    WriteByte( &btFirstLength, "First length" );

    BYTE btSecondLength = nLength >> 7;
    WriteByte( &btSecondLength, "Second Length" );
  } else {
    BYTE btSingleByte = (BYTE)nLength;
    WriteByte( &btSingleByte, "Single length" );
  }

  Write( pStr, nLength );
}

////////////////////////////////////////////////////////////

bool CFileSystemNormal::ReadToMemory() {
  if ( m_pFile == nullptr ) {
    m_strFileName = std::string( "" );
    return false;
  }

  ReleaseData();

  m_iSize = GetSize();
  if ( m_iSize == 0 ) {
    CloseFile();
    return false;
  }

  m_pData = new unsigned char[ m_iSize + 1 ]; // so 0 size file will be saved
  memset( m_pData, 0, m_iSize + 1 );
  Read( m_pData, m_iSize );

  return true;
}

int CFileSystemNormal::GetSize() {
  if ( m_pFile == nullptr || m_strFileName.empty() )
    return 0;

  if ( m_iSize )
    return m_iSize;

  FILE* fp = fopen( m_strFileName.c_str(), "rb" );

  if ( fp == nullptr )
    return 0;

  long lStart = ftell( fp );
  fseek( fp, 1, SEEK_END );
  long lLast = ftell( fp );

  return (lLast - lStart);
}

bool CFileSystemNormal::IsExist(const char* pFileName) {
  return CUtil::Is_FileExist( (char*)pFileName );
}

///////////////////////////////////////////////////////////////////////

// Specific read method
int CFileSystemNormal::ReadFloat(float* pValue) {
  return Read( pValue, sizeof( float ) );
}

int CFileSystemNormal::ReadFloat2(float* lpBuf) {
  return Read( lpBuf, sizeof( float ) * 2 );
}

int   CFileSystemNormal::ReadFloat2(float* x, float* y) {
  int iSize = 0;
  iSize += Read( x, sizeof( float ) );
  iSize += Read( y, sizeof( float ) );

  return iSize;
}

int CFileSystemNormal::ReadFloat3(float* lpBuf) {
  return Read( lpBuf, sizeof( float ) * 3 );
}

int   CFileSystemNormal::ReadFloat3(float* x, float* y, float* z) {
  int iSize = 0;
  iSize += Read( x, sizeof( float ) );
  iSize += Read( y, sizeof( float ) );
  iSize += Read( z, sizeof( float ) );

  return iSize;
}

int CFileSystemNormal::ReadFloat4(float* lpBuf) {
  return Read( lpBuf, sizeof( float ) * 4 );
}

int   CFileSystemNormal::ReadFloat4(float* x, float* y, float* z, float* w) {
  int iSize = 0;
  iSize += Read( x, sizeof( float ) );
  iSize += Read( y, sizeof( float ) );
  iSize += Read( z, sizeof( float ) );
  iSize += Read( w, sizeof( float ) );

  return iSize;
}

int CFileSystemNormal::ReadChar(char* pValue) {
  return Read( pValue, sizeof( char ) );
}

int CFileSystemNormal::ReadByte(unsigned char* pValue) {
  return Read( pValue, sizeof( unsigned char ) );
}

int CFileSystemNormal::ReadBool(bool* pValue) {
  return Read( pValue, sizeof( bool ) );
}

int CFileSystemNormal::ReadInt16(short* pValue) {
  return Read( pValue, sizeof( short ) );
}

int CFileSystemNormal::ReadInt32(int* pValue) {
  return Read( pValue, sizeof( int ) );
}

int CFileSystemNormal::ReadInt64(int64_t* pValue) {
  return Read( pValue, sizeof( int64_t ) );
}

int CFileSystemNormal::ReadUInt16(unsigned short* pValue) {
  return Read( pValue, sizeof( unsigned short ) );
}

int CFileSystemNormal::ReadUInt32(unsigned int* pValue) {
  return Read( pValue, sizeof( unsigned int ) );
}

int CFileSystemNormal::ReadUInt64(uint64_t* pValue) {
  return Read( pValue, sizeof(uint64_t) );
}

// Specific write method
void CFileSystemNormal::WriteFloat(float* pValue, const char* strValueName) {
  Write( pValue, sizeof( float ) );
}

void CFileSystemNormal::WriteFloat2(float* lpBuf, const char* strValueName) {
  Write( lpBuf, sizeof( float ) * 2 );
}

void CFileSystemNormal::WriteFloat2(float* x, float* y, const char* strValueName) {
  Write( x, sizeof( float ) );
  Write( y, sizeof( float ) );
}

void CFileSystemNormal::WriteFloat3(float* lpBuf, const char* strValueName) {
  Write( lpBuf, sizeof( float ) * 3 );
}

void CFileSystemNormal::WriteFloat3(float* x, float* y, float* z, const char* strValueName) {
  Write( x, sizeof( float ) );
  Write( y, sizeof( float ) );
  Write( z, sizeof( float ) );
}

void CFileSystemNormal::WriteFloat4(float* lpBuf, const char* strValueName) {
  Write( lpBuf, sizeof( float ) * 4 );
}

void CFileSystemNormal::WriteFloat4(float* x, float* y, float* z, float* w, const char* strValueName) {
  Write( x, sizeof( float ) );
  Write( y, sizeof( float ) );
  Write( z, sizeof( float ) );
  Write( w, sizeof( float ) );
}

void CFileSystemNormal::WriteChar(char* pValue, const char* strValueName) {
  Write( pValue, sizeof( char ) );
}

void CFileSystemNormal::WriteByte(unsigned char* pValue, const char* strValueName) {
  Write( pValue, sizeof( unsigned char ) );
}

void CFileSystemNormal::WriteBool(bool* pValue, const char* strValueName) {
  Write( pValue, sizeof( bool ) );
}

void CFileSystemNormal::WriteInt16(short* pValue, const char* strValueName) {
  Write( pValue, sizeof( short ) );
}

void CFileSystemNormal::WriteInt32(int* pValue, const char* strValueName) {
  Write( pValue, sizeof( int ) );
}

void CFileSystemNormal::WriteInt64(int64_t* pValue, const char* strValueName) {
  Write( pValue, sizeof( int64_t ) );
}

void CFileSystemNormal::WriteUInt16(unsigned short* pValue, const char* strValueName) {
  Write( pValue, sizeof( unsigned short ) );
}

void CFileSystemNormal::WriteUInt32(unsigned int* pValue, const char* strValueName) {
  Write( pValue, sizeof( unsigned int ) );
}

void CFileSystemNormal::WriteUInt64(uint64_t* pValue, const char* strValueName) {
  Write( pValue, sizeof(uint64_t) );
}
