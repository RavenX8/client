#ifndef _LOADING_
#define _LOADING_
//#define __THREADED_LOADING

#include "../Util/JSingleton.h"
#include "../GameProc/LoadingImageManager.h"

/**
* ���� ������ �ε��̹����� �����ִ� Class 
*	- CLoadingImageManager�� stb�� �����Ͽ� �����־���� �̹����� �ٲپ� �ش�.
*
* @Author	������
* @Date		2005/9/5
*/
class CLoading {

  HNODE m_hTexture;

  CLoadingImageManager m_LoadingImageManager;

  /// m_LoadingImageManager �� �ʱ�ȭ �Ǿ��°�?
  bool m_bInitLoadingImageManager;

  CLoading();
public:
  ~CLoading();

  CLoading(const CLoading&) = delete;
  CLoading& operator=(const CLoading &) = delete;
  CLoading(CLoading &&) = delete;
  CLoading & operator=(CLoading &&) = delete;

  static auto& GetSingleton(){
    static CLoading inst;
    return inst;
  }

  bool InitLoadingImageManager();

  bool LoadTexture(int iZoneNo = 9999, int iPlanetNO = 0);
  void Draw();
  void UnloadTexture();
};

#define g_Loading CLoading::GetSingleton()

#endif //_LOADING_
