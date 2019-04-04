#ifndef _CFileSystemTriggerVFS_
#define _CFileSystemTriggerVFS_

#include "CFileSystem.h"
#include "triggervfs/TriggerVFS.h"

class CFileSystemTriggerVFS : public CFileSystem {
private:
  VFileHandle* m_pFileHandle;
  VHANDLE      m_hVFile;

  std::string    m_strFileName;
  unsigned char* m_pData;

  int m_iSize;
public:
  void SetVFS(VHANDLE hVFile) { m_hVFile = hVFile; };

public:
  CFileSystemTriggerVFS(void );
  ~CFileSystemTriggerVFS(void);

  bool OpenFile(const char* pFileName, int iOpenType = OPEN_READ_BIN) override;
  void CloseFile() override;

  bool           ReadToMemory() override;
  void           ReleaseData() override;
  unsigned char* GetData() override { return m_pData; };;

  int  GetSize() override;
  bool IsExist(const char* pFileName) override;

private:
  virtual void SetVFSHandle(VFileHandle* pFileHandle) { m_pFileHandle = pFileHandle; };

public:
  int  Read(void*        lpBuf, unsigned int nCount) override;
  void Write(const void* lpBuf, unsigned int nCount) override;
  bool Seek(long         lOff, unsigned int  nFrom) override;
  long Tell() override;
  bool IsEOF() override;

  int  ReadStringByNullLength() override;
  int  ReadStringByNull(char*        lpBuf) override;
  void WriteStringByNull(const char* pStr) override;

  int  ReadPascalStringLength() override;
  int  ReadPascalString(char*        lpBuf, int iBufferLength) override;
  void WritePascalString(const char* pStr) override;

  // Specific read method
  int ReadFloat(float*  pValue) override;
  int ReadFloat2(float* lpBuf) override;
  int ReadFloat2(float* x, float* y) override;
  int ReadFloat3(float* lpBuf) override;
  int ReadFloat3(float* x, float* y, float* z) override;
  int ReadFloat4(float* lpBuf) override;
  int ReadFloat4(float* x, float* y, float* z, float* w) override;

  int ReadChar(char*          pValue) override;
  int ReadByte(unsigned char* pValue) override;
  int ReadBool(bool*          pValue) override;

  int ReadInt16(short*   pValue) override;
  int ReadInt32(int*     pValue) override;
  int ReadInt64(int64_t* pValue) override;

  int ReadUInt16(unsigned short*   pValue) override;
  int ReadUInt32(unsigned int*     pValue) override;
  int ReadUInt64(uint64_t* pValue) override;

  // Specific write method
  void WriteFloat(float*  pValue, const char* strValueName                                    = nullptr) override;
  void WriteFloat2(float* lpBuf, const char*  strValueName                                    = nullptr) override;
  void WriteFloat2(float* x, float*           y, const char* strValueName                     = nullptr) override;
  void WriteFloat3(float* lpBuf, const char*  strValueName                                    = nullptr) override;
  void WriteFloat3(float* x, float*           y, float* z, const char* strValueName           = nullptr) override;
  void WriteFloat4(float* lpBuf, const char*  strValueName                                    = nullptr) override;
  void WriteFloat4(float* x, float*           y, float* z, float* w, const char* strValueName = nullptr) override;

  void WriteChar(char*          pValue, const char* strValueName = nullptr) override;
  void WriteByte(unsigned char* pByte, const char*  strValueName = nullptr) override;
  void WriteBool(bool*          pValue, const char* strValueName = nullptr) override;

  void WriteInt16(short*   pValue, const char* strValueName = nullptr) override;
  void WriteInt32(int*     pValue, const char* strValueName = nullptr) override;
  void WriteInt64(int64_t* pValue, const char* strValueName = nullptr) override;

  void WriteUInt16(unsigned short*   pValue, const char* strValueName = nullptr) override;
  void WriteUInt32(unsigned int*     pValue, const char* strValueName = nullptr) override;
  void WriteUInt64(uint64_t* pValue, const char* strValueName = nullptr) override;
};

#endif //_CFileSystemTriggerVFS_
