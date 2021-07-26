#ifndef _VFSMANAGER_
#define _VFSMANAGER_

#include "JSingleton.h"
#include <list>
#include "CFileSystem.h"
#include "triggervfs/TriggerVFS.h"

enum {
  VFS_NORMAL = 0,
  VFS_TRIGGER_VFS,
};

/// VFS manager class
class CVFSManager {
private:
  std::list<CFileSystem*> m_VFSList;
  std::list<CFileSystem*> m_VFSUsedList;

  int m_iVFSType;
  VHANDLE m_hVFile;

  CVFSManager();
public:
  ~CVFSManager();

  void SetVFS(VHANDLE pVFile) { m_hVFile = pVFile; };
       VHANDLE GetVFS() { return m_hVFile; };

  bool         InitVFS(int          iVFSType, int iReserveCount = 10); /// ��� �ڿ� �ʱ�ȭ
  CFileSystem* GetNewFileSystem(int iVFSType);
  void         ReleaseAll(); /// ��� �ڿ� ����

  CFileSystem* GetFileSystem();
  void         ReturnToManager(CFileSystem* pFileSystem);

  bool IsExistFile(const char* pFileName);

  CVFSManager(const CVFSManager&) = delete;
  CVFSManager& operator=(const CVFSManager &) = delete;
  CVFSManager(CVFSManager &&) = delete;
  CVFSManager & operator=(CVFSManager &&) = delete;

  static auto& GetSingleton(){
    static CVFSManager inst;
    return inst;
  }
};

#endif //_VFSMANAGER_
