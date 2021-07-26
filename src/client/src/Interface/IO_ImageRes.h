#ifndef _IOIMAGERES_
#define _IOIMAGERES_

#include "../Util/JSingleton.h"

#include <vector>
#include <map>

/// ��Ʈ���� ���ҽ������� ��ο� 
struct stTexture {
  char  m_szName[MAX_PATH]; //�̸� 
  DWORD m_dwColorKey;
  HNODE m_Texture;
  short m_nTXID;
};

struct stSprite {
  short m_nTextureID;
  RECT  m_Rect;
  DWORD m_dwColor;

  char m_szID[32];

};

typedef std::vector<stTexture>           TextureVEC;
typedef std::vector<stTexture>::iterator TextureVEC_itor;

typedef std::vector<stSprite>           SpriteVEC;
typedef std::vector<stSprite>::iterator SpriteVEC_itor;

class CFileSystem;

////////////////////////////////////////////////////////////////////////////////////////
/// Image resource for UI
////////////////////////////////////////////////////////////////////////////////////////
class CImageRes {
private:
  short m_nSpriteCNT;

  TextureVEC m_TextureVec;
  SpriteVEC  m_SpriteVec;

public:
  CImageRes();
  ~CImageRes();

  /// Load TSI file
  bool LoadRES(CFileSystem* pFileSystem, const char* strResPath = "3DData\\Control");
  void ReleaseRES();

  stTexture* GetTexture(int iSprIndex);
  stSprite*  GetSprite(int  iSprIndex);
  short      GetSpriteCount() { return m_nSpriteCNT; }

};

/// ���ӳ��ο��� ���Ǵ� �̹��� ���ҽ���..
enum {
  IMAGE_RES_UI = 0,
  IMAGE_RES_ITEM = 1,
  IMAGE_RES_NPCFACE = 2,
  ///������ .EXUI�� XML������ MODULE INDEX������ �״�� ��.
  IMAGE_RES_EXUI = 3,
  IMAGE_RES_SKILL_ICON = 4,
  IMAGE_RES_STATE_ICON = 5,
  IMAGE_RES_SOCKETJAM_ICON = 6,
  IMAGE_RES_CLANBACK = 7,
  IMAGE_RES_CLANCENTER = 8,
  IMAGE_RES_TARGETMARK = 9,
  IMAGE_RES_WARNING = 10,
  IMAGE_RES_NEWUI = 11,
  IMAGE_RES_MAX,
};

////////////////////////////////////////////////////////////////////////////////////////
/// Image res manager for UI
////////////////////////////////////////////////////////////////////////////////////////
typedef std::map<int, std::string>  LIST_INDEX_STRING;
typedef LIST_INDEX_STRING::iterator LIST_INDEX_STRING_ITER;

class CImageResManager {
private:
  CImageRes m_ImageRes[ IMAGE_RES_MAX ];

  void ReleaseResource(short i);
  ///list_npcface�� ������ ����
  LIST_INDEX_STRING m_NpcFaceFiles;
  CImageResManager();
public:
  ~CImageResManager();

  CImageResManager(const CImageResManager&) = delete;
  CImageResManager& operator=(const CImageResManager &) = delete;
  CImageResManager(CImageResManager &&) = delete;
  CImageResManager & operator=(CImageResManager &&) = delete;

  static auto& GetSingleton(){
    static CImageResManager inst;
    return inst;
  }

  bool LoadImageResources();
  void ReleaseResources();

  CImageRes* GetImageRes(int iType);

  /// NPC Face Image ����
  bool  Add_NpcFaceFileInfo(int  index, const char* filename);
  HNODE Load_NpcFace(int         index);
  HNODE Load_NpcFace(const char* szName_);
};

#endif //_IOIMAGERES_
