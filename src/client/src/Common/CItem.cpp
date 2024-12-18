#include "StdAfx.h"
#include "CItem.h"
#include "io_stb.h"

#ifndef	__SERVER
#include "CInventory.h"
#include "OBJECT.h"

#include "../Util/LogWnd.h"
#endif

enum {
  DROP_TIEM_GOLD = 0,
  DROP_TIEM_SILVER = 207,
  DROP_TIEM_BLOND = 208,
};

//-------------------------------------------------------------------------------------------------
// ���������� ����ü�̸��� �ݵ�� tagITEM...
short tagITEM::Subtract(tagITEM& sITEM) {
  if ( this->GetHEADER() != sITEM.GetHEADER() ) {
    sITEM.Clear();
    return 0;
  }

  short nWeight;
  if ( this->IsEnableDupCNT() ) {
    if ( this->GetQuantity() > sITEM.GetQuantity() ) {
      this->m_uiQuantity -= sITEM.GetQuantity();

      return (ITEM_WEIGHT( sITEM.m_cType, sITEM.m_nItemNo ) * sITEM.GetQuantity());
    }

    // ���� ���� ����.
    nWeight            = ITEM_WEIGHT( sITEM.m_cType, sITEM.m_nItemNo ) * this->GetQuantity();
    sITEM.m_uiQuantity = this->GetQuantity();
  } else
    nWeight = ITEM_WEIGHT( sITEM.m_cType, sITEM.m_nItemNo );

  // ��� �������� ������ ����..
  this->Clear();

  return nWeight;
}

//-------------------------------------------------------------------------------------------------
// ���������� ����ü�̸��� �ݵ�� tagITEM...
void tagITEM::SubtractOnly(tagITEM& sITEM) {
  if ( this->GetHEADER() != sITEM.GetHEADER() )
    return;

  if ( this->IsEnableDupCNT() ) {
    if ( this->GetQuantity() > sITEM.GetQuantity() ) {
      this->m_uiQuantity -= sITEM.GetQuantity();
      return;
    }
  }

  // ��� �������� ������, ���� 0���� ������ ����..
  this->Clear();
}

tagBaseITEM& tagBaseITEM::operator=(const RoseCommon::Packet::SrvSetMoneyAndItem::Item& item)
{
  this->m_wHeader = item.get_header().get_header();
  this->m_nGEM_OP = item.get_data().get_gem_opt();
  this->m_cDurability = item.get_data().get_durability();
  this->m_nLife = item.get_data().get_life();
  this->m_bHasSocket = item.get_data().get_hasSocket();
  this->m_bIsAppraisal = item.get_data().get_isAppraised();
  this->m_cGrade = item.get_data().get_refine();

  return *this;
}

tagBaseITEM& tagBaseITEM::operator=(const RoseCommon::Packet::SrvSetItem::Item& item)
{
  this->m_wHeader = item.get_header().get_header();
  this->m_nGEM_OP = item.get_data().get_gem_opt();
  this->m_cDurability = item.get_data().get_durability();
  this->m_nLife = item.get_data().get_life();
  this->m_bHasSocket = item.get_data().get_hasSocket();
  this->m_bIsAppraisal = item.get_data().get_isAppraised();
  this->m_cGrade = item.get_data().get_refine();

  return *this;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void tagBaseITEM::Init(int iItem, unsigned int uiQuantity) {
  this->Clear();

#ifndef	__SERVER
  if ( iItem == 0 ) {
    //--------------------------------------------------------------------------------
    LOGERR( "tagITEM::Init( 0 ) : ITEM Type is 0" );
    //--------------------------------------------------------------------------------
    return;
  }
#endif

  if ( 1001 > iItem )
    return;
  this->m_cType   = (char)(iItem / 1000);
  this->m_nItemNo = iItem % 1000;
  if ( this->IsEnableDupCNT() ) {
    if ( uiQuantity > MAX_DUP_ITEM_QUANTITY )
      uiQuantity       = 1;
    this->m_uiQuantity = uiQuantity;
  } else {
    this->m_nLife       = MAX_ITEM_LIFE; // �⺻ ������ 1000���� ..
    this->m_cDurability = ITEM_DURABITY( this->m_cType, this->m_nItemNo );
  }
}

//-------------------------------------------------------------------------------------------------
bool tagBaseITEM::IsValidITEM() {
  if ( 0 == this->GetTYPE() || 0 == this->GetItemNO() )
    return false;

  if ( this->GetTYPE() > ITEM_TYPE_RIDE_PART )
    return false;
  if ( nullptr == g_pTblSTBs[this->GetTYPE()] )
    return false;

  if ( this->GetItemNO() >= g_pTblSTBs[this->GetTYPE()]->m_nDataCnt )
    return false;

  return true;
}

//-------------------------------------------------------------------------------------------------
bool tagBaseITEM::IsEnableKEEPING() {
  if ( this->IsEmpty() )
    return false;

  // 0�ΰ͸� ���࿡ ���� ������... 
  // �������� �����Ǵ� â���� ������ �ٸ� �ɸ����� �������� ���� �ȵǵ���...
  return (0 == ITEM_USE_RESTRICTION( this->m_cType, this->m_nItemNo ) ||
          ITEM_ENABLE_KEEPING & ITEM_USE_RESTRICTION( this->m_cType, this->m_nItemNo ));
}

//-------------------------------------------------------------------------------------------------
bool tagBaseITEM::IsEnableDROP() {
  if ( this->IsEmpty() )
    return false;
  if ( this->m_cType > ITEM_TYPE_RIDE_PART )
    return false;

  if ( ITEM_DONT_DROP_EXCHANGE & ITEM_USE_RESTRICTION( this->m_cType, this->m_nItemNo ) ) {
    // ��� �Ұ� ������.
    return false;
  }

  return true;
}

//-------------------------------------------------------------------------------------------------
bool tagBaseITEM::IsEnableSELL() {
  if ( this->IsEmpty() )
    return false;
  if ( this->m_cType > ITEM_TYPE_RIDE_PART )
    return false;

  if ( ITEM_DONT_SELL & ITEM_USE_RESTRICTION( this->m_cType, this->m_nItemNo ) ) {
    // �Ǹ� �Ұ� ������.
    return false;
  }

  return true;
}

//-------------------------------------------------------------------------------------------------
bool tagBaseITEM::IsTwoHands() {
  if ( m_cType == ITEM_TYPE_WEAPON ) {
    short nWeaponTYPE = WEAPON_TYPE(m_nItemNo);
    if ( nWeaponTYPE >= 221 && nWeaponTYPE <= 255 ) {
      // ��հ� : 221 ~
      // ���Ÿ� : 231 ~
      // �������� : 241 ~
      // īƮ���迭 : 251 ~
      if ( nWeaponTYPE != 242 ) {
        return true;
      } // else ( 242 �Ѽ� ���� ���� )
    }
  }

  return false;
}

//-------------------------------------------------------------------------------------------------
t_eSHOT tagBaseITEM::GetShotTYPE() {
  /// �Ǽ� �϶��� ȣ��ȴ�.
  if ( ITEM_TYPE_WEAPON == m_cType ) {
    switch ( WEAPON_TYPE( m_nItemNo ) ) {

      case WEAPON_ITEM_USE_ARROW:
      case WEAPON_ITEM_USE_ARROW2: return SHOT_TYPE_ARROW;
      case WEAPON_ITEM_USE_BULLET:
      case WEAPON_ITEM_USE_BULLET2: return SHOT_TYPE_BULLET;
      case WEAPON_ITEM_USE_THROW: return SHOT_TYPE_THROW;
    }
  }

  return MAX_SHOT_TYPE;
}

//-------------------------------------------------------------------------------------------------
const t_EquipINDEX s_ItemType2EquipPos[] =
{
  MAX_EQUIP_IDX,

  EQUIP_IDX_FACE_ITEM, // ITEM_TYPE_FACE_ITEM = 1,		// 1	LIST_FACEITEM.stb	�� ���	
  EQUIP_IDX_HELMET,    // ITEM_TYPE_HELMET,			// 2	LIST_CAP.stb
  EQUIP_IDX_ARMOR,     // ITEM_TYPE_ARMOR,				// 3	LIST_BODY.stb
  EQUIP_IDX_GAUNTLET,  // ITEM_TYPE_GAUNTLET,			// 4	LIST_ARMS.stb
  EQUIP_IDX_BOOTS,     // ITEM_TYPE_BOOTS,				// 5	LIST_FOOT.stb
  EQUIP_IDX_KNAPSACK,  // ITEM_TYPE_KNAPSACK,			// 6	LIST_BACK.stb
  EQUIP_IDX_NECKLACE,  // ITEM_TYPE_JEWEL,				// 7	LIST_JEWEL.stb		��ű� : ����� ����

  EQUIP_IDX_WEAPON_R, // ITEM_TYPE_WEAPON,			// 8	LIST_WEAPON.stb		����
  EQUIP_IDX_WEAPON_L, // ITEM_TYPE_SUBWPN,			// 9	LIST_SUBWPN.stb
};

t_EquipINDEX tagBaseITEM::GetEquipPOS() {
  if ( m_cType >= ITEM_TYPE_FACE_ITEM && m_cType <= ITEM_TYPE_SUBWPN ) {
    t_EquipINDEX eIDX = s_ItemType2EquipPos[m_cType];

    if ( eIDX == EQUIP_IDX_NECKLACE ) {
      switch ( ITEM_TYPE(m_cType, m_nItemNo) ) {
        case JEWEL_ITEM_RING: return EQUIP_IDX_RING;
        case JEWEL_ITEM_NECKLACE: return EQUIP_IDX_NECKLACE;
        case JEWEL_ITEM_EARRING: return EQUIP_IDX_EARRING;
      }
      return MAX_EQUIP_IDX;
    }
    return eIDX;
  }

  return MAX_EQUIP_IDX;
}

//-------------------------------------------------------------------------------------------------
#ifndef	__SERVER
/// 2004 / 6 /17 : Client������ ���� ���� �������ϰ�쿡�� �׻� 0 or 1�� �����ϵ��� ����
unsigned int tagBaseITEM::GetQuantity() {
  if ( IsEnableDupCNT() )
    return m_uiQuantity;

  if ( IsEmpty() )
    return 0;

  return 1;
}
#endif

//-------------------------------------------------------------------------------------------------
#ifndef	__SERVER
unsigned short tagBaseITEM::GetModelINDEX() {
  switch ( m_cType ) {
      /*
      case ITEM_TYPE_FACE		:	return	FACEITEM_ICON_NO	( m_nItemNo );
      case ITEM_TYPE_HELMET	:	return	HELMET_ICON_NO		( m_nItemNo );
      case ITEM_TYPE_ARMOR	:	return	ARMOR_ICON_NO		( m_nItemNo );
      case ITEM_TYPE_GAUNTLET	:	return	GAUNTLET_ICON_NO	( m_nItemNo );
      case ITEM_TYPE_BOOTS	:	return	BOOTS_ICON_NO		( m_nItemNo );
      case ITEM_TYPE_KNAPSACK	:	return	BACKITEM_ICON_NO	( m_nItemNo );
      case ITEM_TYPE_JEWEL	:	return	JEWEL_ICON_NO		( m_nItemNo );
  
      case ITEM_TYPE_WEAPON	:	return	WEAPON_ICON_NO		( m_nItemNo );
      case ITEM_TYPE_SUBWPN	:	return	SUBWPN_ICON_NO		( m_nItemNo );
  
      case ITEM_TYPE_USE		:	return	USEITEM_ICON_NO		( m_nItemNo );
  
      case ITEM_TYPE_JEM		:	return	GEMITEM_ICON_NO		( m_nItemNo );
      case ITEM_TYPE_NATURAL	:	return	NATURAL_ICON_NO		( m_nItemNo );
      */
    case ITEM_TYPE_QUEST: return 0;
    case ITEM_TYPE_MONEY: return DROP_TIEM_GOLD;
  }

  if ( m_cType == 0 )
    return 0;

  return ITEM_FIELD_MODEL( m_cType, m_nItemNo ); // ���°��� ��ǥ �ڽ��� ��������...
}
#endif

//-------------------------------------------------------------------------------------------------
#ifndef	__SERVER
char* tagBaseITEM::GettingMESSAGE() {
  if ( m_cType == ITEM_TYPE_MONEY ) {
    return CStr::Printf( F_STR_GETTING_MONEY, m_uiMoney );
  }
  if ( IsEnableDupCNT() ) {
    return CStr::Printf( F_STR_GETTING_ITEMS, ITEM_NAME( m_cType, m_nItemNo ), GetQuantity() );
  }
  // ���..
  return CStr::Printf( F_STR_GETTING_ITEM, ITEM_NAME( m_cType, m_nItemNo ) );

  return nullptr;
}

char* tagBaseITEM::GettingMESSAGE_Party(const char* paryName_) {
  if ( m_cType == ITEM_TYPE_MONEY ) {
    return CStr::Printf( STR_GETTING_MONEY_PARTY, paryName_, m_uiMoney );
  }
  if ( IsEnableDupCNT() ) {
    return CStr::Printf( STR_GETTING_ITEMS_PARTY, paryName_, ITEM_NAME( m_cType, m_nItemNo ), GetQuantity() );
  }
  // ���..
  return CStr::Printf( STR_GETTING_ITEM_PARTY, paryName_, ITEM_NAME( m_cType, m_nItemNo ) );
  return nullptr;
}

#endif
#ifndef	__SERVER
char*         tagBaseITEM::GettingMESSAGE(int iInventoryListNo) {
  CInventory* pInventory = nullptr;
  if ( m_cType == ITEM_TYPE_MONEY ) {
    return CStr::Printf( F_STR_GETTING_MONEY, m_uiMoney );
  }
  if ( IsEnableDupCNT() ) {
    pInventory    = g_pAVATAR->GetInventory();
    tagITEM sItem = pInventory->LST_GetITEM( iInventoryListNo );

    if ( sItem.GetTYPE() == 0 )
      sItem.m_uiQuantity = 0;

    return CStr::Printf( F_STR_GETTING_ITEMS, ITEM_NAME( m_cType, m_nItemNo ), m_uiQuantity - sItem.GetQuantity() );
  }
  // ���..
  return CStr::Printf( F_STR_GETTING_ITEM, ITEM_NAME( m_cType, m_nItemNo ) );

  return nullptr;
}
#endif

#ifndef	__SERVER
char* tagBaseITEM::GettingQuestMESSAGE() {
  if ( m_cType == ITEM_TYPE_MONEY )
    return CStr::Printf( F_STR_QUEST_GETTING_MONEY, m_uiMoney );
  if ( IsEnableDupCNT() )
    return CStr::Printf( F_STR_QUEST_GETTING_ITEMS, ITEM_NAME( m_cType, m_nItemNo ), m_uiQuantity );
  return CStr::Printf( F_STR_QUEST_GETTING_ITEM, ITEM_NAME( m_cType, m_nItemNo ) );
  return nullptr;
}
#endif

#ifndef	__SERVER
char* tagBaseITEM::SubtractQuestMESSAGE() {
  if ( m_cType == ITEM_TYPE_MONEY )
    return CStr::Printf( F_STR_QUEST_SUBTRACT_MONEY, m_uiMoney );
  if ( IsEnableDupCNT() )
    return CStr::Printf( F_STR_QUEST_SUBTRACT_ITEMS, ITEM_NAME( m_cType, m_nItemNo ), m_uiQuantity );
  return CStr::Printf( F_STR_QUEST_SUBTRACT_ITEM, ITEM_NAME( m_cType, m_nItemNo ) );

  return nullptr;
}
#endif

//-------------------------------------------------------------------------------------------------
///�Ҹ�ź�������� ShotType�� ���
#ifndef	__SERVER
t_eSHOT tagBaseITEM::GetBulletType() {
  if ( IsEmpty() || m_cType != ITEM_TYPE_NATURAL )
    return MAX_SHOT_TYPE;

  return GetNaturalBulletType( m_nItemNo );
}
#endif

#ifndef	__SERVER
t_eSHOT tagBaseITEM::GetNaturalBulletType(int iItemNo) {

  t_eSHOT retType = MAX_SHOT_TYPE;

  if ( iItemNo == 0 )
    return retType;

  switch ( ITEM_TYPE( ITEM_TYPE_NATURAL, iItemNo ) ) {
    case 431: ///ȭ��
      retType = SHOT_TYPE_ARROW;
      break;
    case 432: ///�Ѿ�
      retType = SHOT_TYPE_BULLET;
      break;
    case 433: ///��ô
    case 421:
    case 422:
    case 423: retType = SHOT_TYPE_THROW;
      break;
    default: break;
  }
  return retType;
}
#endif
#ifndef	__SERVER
int tagBaseITEM::GetHitRate() {
  if ( m_cType != ITEM_TYPE_WEAPON )
    return 0;

  return (int)(((float)ITEM_QUALITY( m_cType, m_nItemNo ) * 0.6) + ((float)m_cDurability * 0.8) - 15.0f);
}

int   tagBaseITEM::GetAvoidRate() {
  int iRet = 0;
  switch ( m_cType ) {
    case ITEM_TYPE_HELMET:
    case ITEM_TYPE_ARMOR:
    case ITEM_TYPE_GAUNTLET:
    case ITEM_TYPE_BOOTS:
    case ITEM_TYPE_KNAPSACK:
    case ITEM_TYPE_SUBWPN: iRet = (int)((float)m_cDurability * 0.3f);
      break;
    default: iRet = 0;
  }
  return iRet;
}
#endif
#ifndef	__SERVER
bool tagBaseITEM::IsEqual(int iType, int iItemNo) {
  if ( m_cType == iType && m_nItemNo == iItemNo )
    return true;

  return false;
}
#endif

#ifndef	__SERVER
bool tagBaseITEM::IsEnableExchange() {
  if ( this->IsEmpty() )
    return false;

  if ( ITEM_DONT_DROP_EXCHANGE & ITEM_USE_RESTRICTION( this->m_cType, this->m_nItemNo ) ) {
    // ��� ,��ȯ �Ұ� ������.
    return false;
  }

  return true;
}
#endif

#ifndef	__SERVER
bool tagBaseITEM::IsEnableSeparate() {
  if ( IsEmpty() )
    return false;

  if ( GetTYPE() == ITEM_TYPE_GEM ) ///������ �и��� �Ұ����ϴ�
    return false;

  if ( HasSocket() && GetGemNO() > 300 ) ///��ֵ� ���
    return true;

  if ( ITEM_PRODUCT_IDX( GetTYPE(), GetItemNO() ) )
    return true;

  return false;
}
#endif

#ifndef	__SERVER
bool tagBaseITEM::IsEnableUpgrade() {
  if ( IsEmpty() )
    return false;

  int iClass = ITEM_TYPE( GetTYPE(), GetItemNO() );

  if ( GetGrade() < 9 ) {
    if ( iClass >= 211 && iClass <= 280 ) ///���� - ���ݷ�, ���߷»��
      return true;

    if ( iClass >= 121 && iClass <= 153 ) ///�� - ����, �׸���, ȸ�Ƿ»��
      return true;

    if ( iClass == 161 || iClass == 163 ) ///�����( ���� ����)
      return true;

    if ( iClass == 261 ) ///���� - ����,�׸���,ȸ�Ƿ»��
      return true;
  }
  return false;
}
#endif

#ifndef	__SERVER
bool tagBaseITEM::HasLife() {
  if ( IsEquipITEM() || GetTYPE() == ITEM_TYPE_RIDE_PART )
    return true;

  return false;
}
#endif
#ifndef	__SERVER
bool tagBaseITEM::HasDurability() {
  if ( IsEquipITEM() )
    return true;

  return false;
}
#endif

#ifndef	__SERVER
bool tagBaseITEM::IsEnableAppraisal() {
  if ( IsEmpty() )
    return false;

  bool b = IsEquipITEM();
  if ( IsEquipITEM() && m_nGEM_OP && m_nGEM_OP <= 300 && m_bIsAppraisal == 0 )
    return true;

  return false;
}
#endif

#ifndef	__SERVER
int tagBaseITEM::GetUpgradeCost() {
  if ( !IsEnableUpgrade() )
    return 0;
  return (int)(GetGrade() * (GetGrade() + 1) * ITEM_QUALITY( GetTYPE(), GetItemNO() ) * (ITEM_QUALITY(GetTYPE(), GetItemNO()) + 20) * 0.2f);
  //	return ITEM_QUALITY( GetTYPE(), GetItemNO() ) * 10 + GetGrade() * 100 ;
}

int tagBaseITEM::GetSeparateCost() {
  if ( !IsEnableSeparate() )
    return 0;

  return ITEM_QUALITY( GetTYPE(), GetItemNO() ) * 10 + 20;
}

int tagBaseITEM::GetAppraisalCost() {
  if ( !IsEnableAppraisal() )
    return 0;

  int64_t i64BasePrice = ITEM_BASE_PRICE( GetTYPE(), GetItemNO() );
  return (int)((i64BasePrice + 10000) * (GetDurability() + 50) / 10000);
}

const char* tagBaseITEM::GetName() {
  if ( IsEmpty() )
    return "NULL";

  if ( m_cType == ITEM_TYPE_MONEY )
    return STR_MONEY;
  if ( m_cType >= ITEM_TYPE_FACE_ITEM && m_cType < ITEM_TYPE_MONEY )
    return ITEM_NAME( m_cType, m_nItemNo );

  return "NULL";
}

#endif
