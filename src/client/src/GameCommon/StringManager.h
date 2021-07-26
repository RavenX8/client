#ifndef _STRING_MANAGER_
#define _STRING_MANAGER_

#include <string>
#include <vector>

#include "../Util/JSingleton.h"
#include <functional>
#include "../Util/JResourceContainer.h"

template <typename T>
class LessThen_TableEntity {
public:
  bool operator ()(T& lhs, const std::string& rhs) {
    return lhs < rhs;
  }
};

//-----------------------------------------------------------------------------------------------
/// ���� ��� Ÿ��
//-----------------------------------------------------------------------------------------------
enum LANGUAGE_TYPE {
  LANGUAGE_KOR = 0,
  LANGUAGE_USA = 1,
  LANGUAGE_JPN = 2,
  LANGUAGE_CHA_TRADITIONAL = 3,
  LANGUAGE_CHA_SIMPLE = 4,
};

//-----------------------------------------------------------------------------------------------
/// ���̺� Ÿ��
//-----------------------------------------------------------------------------------------------
enum STRING_TABLE_TYPE {
  ITEM_TYPE_ST = 0,
  NORMAL_TYPE_ST = 1,
  QUEST_TYPE_ST = 2
};

//-----------------------------------------------------------------------------------------------
/// ������ Ÿ�� ��Ƽ�� ���̺��
//-----------------------------------------------------------------------------------------------
enum ITEM_TYPE_TABLES {
  LIST_ARMS_S_TB = 0,
  LIST_BACK_S_TB,
  LIST_BODY_S_TB,

  LIST_CAP_S_TB,
  LIST_FACEITEM_S_TB,
  LIST_FOOT_S_TB,

  LIST_JEMITEM_S_TB,
  LIST_JEWEL_S_TB,
  LIST_NATURAL_S_TB,

  LIST_PAT_S_TB,
  LIST_QUESTITEM_S_TB,
  LIST_SKILL_S_TB,

  LIST_SUBWPN_S_TB,
  LIST_USEITEM_S_TB,
  LIST_WEAPON_S_TB,

  LIST_ZONE_S_TB,
  HELP_S_TB,

  MAX_ITEM_TYPE_TB,
};

//-----------------------------------------------------------------------------------------------
/// �븻 Ÿ�� ��Ƽ�� ���̺��
//-----------------------------------------------------------------------------------------------
enum NORMAL_TYPE_TABLES {
  STR_ITEMPREFIX = 0,
  LIST_NPC_S_TB,
  LIST_STRING_S_TB,
  STR_ABILITY,
  STR_JOBNAME,
  STR_ITEMTYPE,
  STR_SKILLTARGET,
  STR_SKILLTYPE,
  LIST_SELL_S,
  LIST_CLASS_S,
  LIST_UNION_S,
  STR_CLAN,
  STR_PLANET,
  STR_NOTIFY_MESSAGE,
  MAX_NORMAL_TYPE_TB
};

//-----------------------------------------------------------------------------------------------
/// ����Ʈ Ÿ�� ��Ƽ�� ���̺��
//-----------------------------------------------------------------------------------------------
enum QUEST_TYPE_TABLES {
  LIST_QUEST_S_TB = 0,
  LIST_STATUS_S,
  MAX_QUEST_TYPE_TB
};

struct stTypeTable {
  std::string strKey;
  int         iIndex;
};

//-----------------------------------------------------------------------------------------------
/// ������ Ÿ�� ��Ƽ�� ���̺�
//-----------------------------------------------------------------------------------------------
struct stItemTypeTable : public stTypeTable {
  std::string strName;
  std::string strDesc;
};

//-----------------------------------------------------------------------------------------------
/// �Ϲ� ��Ʈ�� ���̺�
//-----------------------------------------------------------------------------------------------
struct stNormalTypeTable : public stTypeTable {
  std::string strString;

  bool operator <(std::string& rhs) {
    return this->strKey < rhs;
  }
};

//-----------------------------------------------------------------------------------------------
/// ����Ʈ ��Ʈ�� ���̺�
//-----------------------------------------------------------------------------------------------
struct stQuestTypeTable : public stTypeTable {
  std::string strName;
  std::string strStartMsg;
  std::string strEndMsg;
  std::string strDesc;

  bool operator <(std::string& rhs) {
    return this->strKey < rhs;
  }
};

typedef JED::CResourceContainer<stItemTypeTable>   ItemTypeMap;
typedef JED::CResourceContainer<stNormalTypeTable> NormalTypeMap;
typedef JED::CResourceContainer<stQuestTypeTable>  QuestTypeMap;

//typedef CResourceContainer< stItemTypeTable >::iterator		ItemTypeMapItor;
//typedef CResourceContainer< stNormalTypeTable >::iterator	NormalTypeMapItor;
//typedef CResourceContainer< stQuestTypeTable >::iterator		QuestTypeMapItor;

//-----------------------------------------------------------------------------------------------
/// ���ӳ� ���� ��Ʈ������ �ܺ� ��Ʈ���� ���� ����..
//-----------------------------------------------------------------------------------------------
class CStringManager {
private:
  LANGUAGE_TYPE m_iLanguageType;

  ItemTypeMap   m_ItemTypeTables[ MAX_ITEM_TYPE_TB ];
  NormalTypeMap m_NormalTypeTables[ MAX_NORMAL_TYPE_TB ];
  QuestTypeMap  m_QuestTypeTables[ MAX_QUEST_TYPE_TB ];

  char m_TempBuf[1024];

  std::string m_strNull;

  CStringManager(void );
public:
  ~CStringManager(void);

  CStringManager(const CStringManager&) = delete;
  CStringManager& operator=(const CStringManager &) = delete;
  CStringManager(CStringManager &&) = delete;
  CStringManager & operator=(CStringManager &&) = delete;

  static auto& GetSingleton(){
    static CStringManager inst;
    return inst;
  }

  //-----------------------------------------------------------------------------------------------
  /// ĳ���� ������ ���ӳ� ��� ���̵� ���Ѵ�.
  //-----------------------------------------------------------------------------------------------
  LANGUAGE_TYPE GetLanguageIDInGame(int iCharSet);

  //-----------------------------------------------------------------------------------------------
  /// ĳ���� ������ �̸� ������ ��Ʈ�� ����.
  //-----------------------------------------------------------------------------------------------
  char* GetFontNameByCharSet(int iCharSet);

  //-----------------------------------------------------------------------------------------------
  /// ��Ʈ�����̺� �ʱ�ȭ( �޸� Ǯ�� ���� �۾� )
  //-----------------------------------------------------------------------------------------------
  void InitStringTables();
  void ClearStringTables();

  //-----------------------------------------------------------------------------------------------
  /// ��� ���̺� �ε� �� Ŭ����( ���� �ε��� ��Ʈ���� ���� �۾� )
  //-----------------------------------------------------------------------------------------------
  bool LoadTables(int iCharSet);
  void ClearTables();
  void ClearTypeTable();

  //-----------------------------------------------------------------------------------------------
  /// Ư�� Ÿ�� ���̺� �ε�
  //-----------------------------------------------------------------------------------------------
  bool LoadTypeTable(LANGUAGE_TYPE languageType, const char* pszFileName, int iStringType);

  //-----------------------------------------------------------------------------------------------
  /// String access methodes
  //-----------------------------------------------------------------------------------------------
  //const char*	GetItemString( int iType, int iItemNO );

  stItemTypeTable* GetItemStringData(int iType, int iItemNo);
  const char*      GetItemName(int       iType, int iItemNo);
  const char*      GetItemDesc(int       iType, int iItemNo);

  int GetItemNOFromName(int iType, const char* cName);

  stItemTypeTable* GetSkillStringData(int iSkillNo);
  const char*      GetSkillName(int       iSkillNo);
  const char*      GetSkillDesc(int       iSkillNo);

  stQuestTypeTable* GetQuestStringData(int iQuestNo);
  const char*       GetQuestName(int       iQuestNo);
  const char*       GetQuestStartMsg(int   iQuestNo);
  const char*       GetQuestEndMsg(int     iQuestNo);
  const char*       GetQuestDesc(int       iQuestNo);

  stQuestTypeTable* GetStatusStringData(int iStatusNo);
  const char*       GetStatusName(int       iStatusNo);
  const char*       GetStatusStartMsg(int   iStatusNo);
  const char*       GetStatusEndMsg(int     iStatusNo);
  //-----------------------------------------------------------------------------------------------
  /// NormalType Table String access methodes
  //-----------------------------------------------------------------------------------------------
  char*       GetString(int            iIndex);
  char*       GetJobName(int           iIndex);
  char*       GetSkillType(int         iIndex);
  char*       GetSkillTarget(int       iIndex);
  char*       GetItemType(int          iIndex);
  char*       GetAbility(int           iIndex);
  const char* GetStoreTabName(int      iIndex);
  const char* GetClassName(int         iIndex);
  const char*        GetUnionName(int  iIndex);
  const char*        GetClanClass(int  iClassNo);
  const char*        GetPlanetName(int iPlanetNo);
  const std::string& GetItemPrefix(int iIndex);
  //-----------------------------------------------------------------------------------------------
  /// Npc String
  //-----------------------------------------------------------------------------------------------
  const char* GetNpcName(int iIndex);
  const char* GetNpcDesc(int iIndex);

  //-----------------------------------------------------------------------------------------------
  /// Zone String
  //-----------------------------------------------------------------------------------------------
  stItemTypeTable* GetZoneStringData(int iZoneNO);
  const char*      GetZoneName(int       iZoneNO);
  const char*      GetZoneDesc(int       iZoneNO);

  //-----------------------------------------------------------------------------------------------
  /// Tip String
  //-----------------------------------------------------------------------------------------------
  const char* GetTipHeader(int  iIndex);
  const char* GetTipContent(int iIndex);

  //-----------------------------------------------------------------------------------------------
  /// Notify Message String
  //-----------------------------------------------------------------------------------------------
  const char* GetNotifyMessageString(int iIndex);

private:
  char* GetNormalTypeTableString(int iType, std::string& strKey);

};

#endif //_STRING_MANAGER_
