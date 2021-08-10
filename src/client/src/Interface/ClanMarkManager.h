#ifndef _CLAN_MARK_MANAGER_
#define _CLAN_MARK_MANAGER_

#include "CClanMarkUserDefined.h"
#include "../Util/JSingleton.h"

#include <list>
#include <string>

//----------------------------------------------------------------------------------
/// class for texture pool.
///
///
//----------------------------------------------------------------------------------

class CClanMarkManager {
  CClanMarkManager(void );
public:
  ~CClanMarkManager(void);

  CClanMarkManager(const CClanMarkManager&) = delete;
  CClanMarkManager& operator=(const CClanMarkManager &) = delete;
  CClanMarkManager(CClanMarkManager &&) = delete;
  CClanMarkManager & operator=(CClanMarkManager &&) = delete;

  static auto& GetSingleton(){
    static CClanMarkManager inst;
    return inst;
  }

  CClanMarkUserDefined* GetClanMark(const char* pstrName, WORD crc16, int iClanID);

  //------------------------------------------------------------------
  /// ������ ���� ������Ʈ( ������ �÷���, �ؽ��� �ε� )
  //------------------------------------------------------------------
  void UpdatePool();
  //------------------------------------------------------------------
  /// Clear m_TextureNodePool
  //------------------------------------------------------------------
  void Free();
  void ReloadTexture(const char* FileName, WORD wCRC16);

private:
  //------------------------------------------------------------------
  /// Search texture form pool.
  //------------------------------------------------------------------
  CClanMarkUserDefined* SearchTexture(const char* pstrName);

  //------------------------------------------------------------------
  /// ���ο� UserDefinedClanMark ��ü�� �����ϰ� ����..
  //------------------------------------------------------------------
  CClanMarkUserDefined* GetUserdefinedClanMark(const char* pstrName, WORD crc16, int iClanID);

  //------------------------------------------------------------------
  /// Load new texture form HDD.
  //------------------------------------------------------------------
  HNODE LoadNewTexture(const char* pstrName, WORD crc16);

  //------------------------------------------------------------------
  /// �ؽ��� �ε� �÷��װ� ���� ���鿡 ���� �ؽ��� �ε�..
  //------------------------------------------------------------------
  void LoadRealTexture(CClanMarkUserDefined*);

private:

  std::list<CClanMarkUserDefined*> m_TextureNodePool;

};

#endif //_CLAN_MARK_MANAGER_
