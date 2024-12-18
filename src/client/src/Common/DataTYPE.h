/*
  ** �� ����Ÿ text �ۼ���.
  numobj 1
  obj 1
    mesh 		3Ddata\????.ZMS		-- �޽� ����
    mat 		3Ddata\????.tga		-- �ؽ��� ����
    isskin   	0					-- skinning		on:1, off:0		*���� bonenumber ���� ���ȵȴ� !
    alpha 		0					-- alpha		on:1, off:0
    twoside 	0					-- twoside		on:1, off:0
    alphatest	1                   -- �����׽�Ʈ ����. on:1, off:0.              2003/10/10 �߰���.
    alpharef	128                 -- �����׽�Ʈ�� �񱳰�. 0-255.                2003/10/10 �߰���.
    ztest		1                   -- Z�׽�Ʈ.  on:1, off:0.                     2003/10/10 �߰���.
    zwrite		1                   -- Z�� ����. on:1, off:0.                     2003/10/10 �߰���.
    blendtype	0                   -- ������Ÿ��. �⺻:0, ���:1.                2003/10/10 �߰���.
    specular	0                   -- ����ŧ�� ����. on:1, off:0.				  2003/10/10 �߰���.
    alphavalue 1.0				-- ���� �⺻ ��ü ����. �⺻�� 1.0		2004/07/23 �߰���.
    glowtype 0					-- ���� �۷ο� Ÿ�� �⺻�� 0					2004/07/25 �߰���.
    glowcolor 0.5 0.5 0.5    -- �۷ο� �÷�. �⺻ 1, 1, 1. ���� 0���� 1����. 2004/07/22 �߰���.
    pos	0.0 0.0 0.0					-- export���� ��ϵǴ� ���� 
    rot -0.xxx 0.xxx -0.xxx -0.xxx	-- export���� ��ϵǴ� ���� 
    scale 1.0 1.0 1.0				-- export���� ��ϵǴ� ���� 
    linkdummy	?					-- ���� �������� ��� ��ġ�� ���� ��ȣ.
    bonenumber	?					-- ��ġ�� ���� ��ȣ				*���� isskin�� ���� ���ȵȴ� !
    parent		0					
    collision	4					-- �浹 üũ, �ʵ� ������, �ǹ� �������� 4�Է�  Ǯ, �����ٵ��� �ʿ� ����.

    mob_stop	3DData\???.ZMO		-- ���� ���� �޽� ���ϸ��̼��̵Ǵ� ��� �� �ൿ�� �´� ��� ������ ���	
    mob_move	3DData\???.ZMO		-- ������ ��� ������ ������ �ʿ� ���� ���� ���� �߰����� �ʴ´�.
    mob_attack	3DData\???.ZMO      -- ����� �������� ������ ���� ����� ���ȴ�.
    mob_die		3DData\???.ZMO
    mob_hit		3DData\???.ZMO
    mob_etc		3DData\???.ZMO

    avt_walk	3DData\???.ZMO		-- �ƹ�Ÿ ���� �޽� ���ϸ��̼��� �Ǵ°��� �� ���ۿ� �´� ��� ������ ���.
    avt_attack  3DData\???.ZMO		-- ���� ��� ������ ����.
    avt_stop1	3DData\???.ZMO
    avt_stop2	3DData\???.ZMO
    avt_run		3DData\???.ZMO
    avt_sitting	3DData\???.ZMO
    avt_sit		3DData\???.ZMO
    avt_standup	3DData\???.ZMO
    avt_stop3	3DData\???.ZMO
    avt_hit		3DData\???.ZMO
    avt_fall	3DData\???.ZMO
    avt_die		3DData\???.ZMO
    avt_raise	3DData\???.ZMO


  numpoint 1
  point 1
    effect		3DDATA\????.eft		-- ����Ʈ�� ��ũ�� ȿ�� ���� ������ ��������...
    parent 1						-- ����Ʈ�� ��ġ�� obj ��ȣ
    pos	0.0 0.0 0.0					-- export���� ��ϵǴ� ���� 
    rot -0.xxx 0.xxx -0.xxx -0.xxx	-- export���� ��ϵǴ� ���� 
    scale 1.0 1.0 1.0				-- export���� ��ϵǴ� ���� 


  �ɸ��� :
    isskin �� bonenumber �� ���ÿ� 1�� �Ǹ� �ʵȴ�.

  ���� :
    -- ���Ⱑ ��â�� linkdummy�� �ݵ�� ����Ѵ�.


  �ǹ� & ������ :
    -- �浹 üũ�� �ʿ��� �κ��� collision �� ����Ѵ�.
    -- �����ٵ� �ɸ��� ���ʿ� ��ġ�ϴ°͵��� ��� collision�� 0���� �ϰų� �����Ѵ�.

  �ʵ� ������ :
    -- �ݵ�� collision ���� 4�� �����Ѵ�.
*/
/*
  ** ĳ���� ���� ����Ʈ ����.
    0 : ������
    1 : �޼�
    2 : �޼� ����
    3 : �����
    4 : ��
    5 : ��
    6 : ����
    7 : Ÿ�� ����
*/

/*
  ** ���� ����Ʈ ���� ( ȿ�� ���� ����Ʈ�� 3���� ����)
    0 ~ 1 : Į�� ��� �ܻ��� �׷��� ���� 
    0 : ���Ÿ� ���� �Ѿ� �߻� ��ġ.
    1 ~ ������ : ��Ÿ ȿ�� ���� ��ġ. Į�ϰ�� 2�� ����Ʈ ����.
*/

/*
  ** �ƹ�Ÿ �� �ε���.
    0 :
    1 :
    2 :
    3 :
    4 : �Ӹ�
    5 :
    6 :
*/

/*
// ������ �Һз�ǥ !!!
����     (111)	
�Ȱ�     (112)	
��Ÿ     (113)	
������    (121)	
������    (122)	
�Ϲݸ���  (123)	
�Ӹ���� (124)	
������   (131)	
������   (132)	
�Ϲݿ�   (133)	
�ǵ鷿    (141)	
�����尩  (142)	
�Ϲ��尩  (143)	
����ȭ    (151)	
��������  (152)	
�ϹݽŹ� (153)	
��� (161)	
�賶     (162)	
����     (163)	
ȭ����   (164)	
�Ѿ���   (165)	
��ź��   (166)	
�����   (171)	����ִ� ������ ������ �ٿ��� ������Ų��.
����     (172)	
�Ѽհ�   (211)	������ ���Ͽ����� 1�� ~ 100������
�Ѽ� �б� (212)	
��հ�    (221)	ũ�Ⱑ ū ���� ����
������ ���Ͽ����� 101�� ~ 200������
â        (222)	
��� ���� (223)	
Ȱ        (231)	���Ÿ� ���ݿ� ����
������ ���Ͽ����� 201�� ~ 300������
��        (232)	
���������� (241)	������ ���⳪ ����
������ ���Ͽ����� 301�� ~ 400������
���� ����  (242)	
īŸ��    (251)	������,�޼տ� ��� ������ �����Ǵ� ����.
������ ���Ͽ����� 401�� ~ 500������
�̵���    (252)	
����     (253)	
����      (261)	* �Ѽ� ���⸦ �������� ��, �ٸ� �տ� ���� ��� ������ �� �ִ�. * ���� ���� �ƹ�Ÿ�� �޼տ� �����ȴ�.
��������  (262)	* �ҽû�ǰ, �������� ����.
��ǰ      (311)	ü��ȸ����, �ٷ°�ȭ��, DHA������
����      (312)	�����ٺ�ť, ���, �ָԹ�
��������  (313)	1ȸ�� ������ �ߵ���Ű�� ������
��ų����  (314)	���ο� ��ų�� ���� ���ִ� ������
����      (411)	���̾Ƹ��, ��� ��..
����ǰ    (412)	���ڱ�, �׸�, ������ ��..
�ݼ�      (421)	ö, ����, �� ��..
���ݼ�    (422)	���������� ���յ� Ư�� �ݼ�
����      (423)	�븮��, ���, ���� ��..
����      (424)	����, ���� ��..
����      (425)	�Ұ���, �簡�� ��..
�ʰ�      (426)	����õ, ��, ���, ���� ��..
�������  (427)	���ݼ�, ��������� ���
ȭ��ǰ    (428)	��ǰ, ���� ���� ���� �� ���.
�۸�����, �Ҵ�, ���׳׽�, ȭ��, ��������
����ǰ    (429)	����, ����, ����, ����, ������, ��������, 
������ũ(���� ����), �� ����å
�ڿ���    (430)	ö����, ��ź, ������, ����,����,ȭ�� ��..
�����丷, ����, �ٻ��, ���̲���,�����̾�, ���粮��, ���� ��
����Ʈ ��ǰ(441)	 ���ǿ���, ������Ű, ��������
������     (442)	 ���Ȱ� �ź���, ���� �ʴ���
*/

#ifndef	__DATATYPE_H
#define	__DATATYPE_H
//-------------------------------------------------------------------------------------------------

#define	BONE_IDX_PELVIS		0
#define	BONE_IDX_HEAD		4

#define DUMMY_IDX_R_HAND	0
#define	DUMMY_IDX_L_HAND	1
#define DUMMY_IDX_L_SHIELD	2
#define DUMMY_IDX_BACK		3
#define DUMMY_IDX_MOUSE		4
#define DUMMY_IDX_EYES		5
#define	DUMMY_IDX_CAP		6

//-------------------------------------------------------------------------------------------------

#ifndef	__POINT16
#define	__POINT16

struct tPOINT16 {
  short m_nX;
  short m_nY;
};
#endif

#ifndef	__POINTF
#define	__POINTF
typedef CVec2 tPOINTF;
/*
struct tPOINTF {
  float	m_fX;
  float	m_fY;
*/
#endif

#ifndef	__SERVER
#define	NAME_MESH			"@MS%03d"
#define	NAME_MOTION			"@AM%03d"
#define	NAME_SKEL			"@SK%03d"
#define	NAME_MATERIAL		"@MT%03d"
#define	NAME_TERRAIN_MAT	"#TR%03d"
#define	NAME_FIXEDOBJ		"@FM%03dp%d"
#define	NAME_CNSTOBJ		"@CN%03dp%d"
#define	NAME_TREEOBJ		"@TE%03dp%d"
#define	NAME_COLLISIONOBJ	"@COLL%03dp%d"
#define	NAME_ITEMOBJ		"@FI%03dp%d"
#define	NAME_MOB_MODEL		"@MB%s-%d"				// create order
#define	NAME_PARTICLE		"@PT%d"
#define	NAME_BULLET			"@BU%d"

typedef LPDIRECT3DDEVICE9       LPD3DDEVICE;
typedef IDirect3DTexture9       ID3DTexture;
typedef LPDIRECT3DTEXTURE9      LPD3DTEXTURE;
typedef D3DMATERIAL9            D3DMATERIAL;
typedef LPDIRECT3DVERTEXBUFFER9 LPD3DVERTEXBUFFER;

typedef D3DVECTOR t_POSITION;
#else
  typedef	tPOINTF						t_POSITION;
#endif

#define	MIN_PRICE_TYPE		1
#define	MAX_PRICE_TYPE		11

#ifndef	__ENUM_CHARPART
#define	__ENUM_CHARPART

#define FACE_EXPRESSION_COUNT	7
#define HAIR_TYPE_COUNT			5

//*************************************************************************************************
/// �����ֱ� ���� �� ��ȣ..
enum t_CharPART {
  BODY_PART_FACE = 0,
  // 0 link to bone
  BODY_PART_HAIR = 1,
  BODY_PART_HELMET = 2,

  BODY_PART_ARMOR = 3,
  // 3 skinning
  BODY_PART_GAUNTLET = 4,
  BODY_PART_BOOTS = 5,

  BODY_PART_GOGGLE = 6,
  // 6 link to dummy
  BODY_PART_FACE_ITEM=BODY_PART_GOGGLE,
  // 6 link to dummy
  BODY_PART_KNAPSACK = 7,

  // ������, �޼� ���� ���� !!!
  BODY_PART_WEAPON_R = 8,
  // 8 link to dummy	
  BODY_PART_WEAPON_L = 9,

  MAX_BODY_PART = 10
};
#endif

//*************************************************************************************************
enum t_ObjTAG {
  OBJ_NULL = 0,
  OBJ_MORPH,
  OBJ_ITEM,
  OBJ_COLLISION,
  OBJ_GROUND,
  OBJ_CNST,
  OBJ_NPC,
  // OBJ_NPC >= character object ...
  OBJ_MOB,
  OBJ_AVATAR,
  OBJ_USER,
  OBJ_CART,
  OBJ_CGEAR,
  OBJ_EVENTOBJECT,
  OBJ_MAX_TYPE
};

//*************************************************************************************************
/// �����ϰ� �ִ� ������...
enum t_EquipINDEX {
  EQUIP_IDX_NULL = 0,

  EQUIP_IDX_FACE_ITEM,
  EQUIP_IDX_HELMET,
  EQUIP_IDX_ARMOR,
  EQUIP_IDX_KNAPSACK,
  EQUIP_IDX_GAUNTLET,
  EQUIP_IDX_BOOTS,
  // 5
  EQUIP_IDX_WEAPON_R=7,
  EQUIP_IDX_WEAPON_L,
  EQUIP_IDX_NECKLACE,
  EQUIP_IDX_RING,
  EQUIP_IDX_EARRING,
  // 2003. 10. 27 : �������� ������ 2��...

  MAX_EQUIP_IDX // 11
};

enum t_eSHOT {
  SHOT_TYPE_ARROW=0,
  // ȭ���
  SHOT_TYPE_BULLET,
  // ��/�ֱ���
  SHOT_TYPE_THROW,
  // ��ô���
  MAX_SHOT_TYPE = 3
};

///
/// cart PET_TYPE == bone type
///
enum PET_TYPE {
  PET_TYPE_CART01 = 21,
  PET_TYPE_CASTLE_GEAR01 = 31,
  PET_TYPE_MAX,
};

enum t_eRidePART {
  RIDE_PART_BODY = 0,
  RIDE_PART_ENGINE,
  RIDE_PART_LEG,

#if defined(_GBC)
  RIDE_PART_ABIL,
  RIDE_PART_ARMS,
#else
  RIDE_PART_ARMS,		
#endif

  MAX_RIDING_PART

};

/*
#ifdef _NEWUI

enum t_InvTYPE 
{
  INV_WEAPON			= 0,
  INV_USE,
  INV_ETC,
  INV_MILEAGE_AVATA,	

  INV_TYPE_SIZE,

  INV_RIDING			= INV_TYPE_SIZE,
  INV_ETC_PAT,
  INV_MILEAGE_PAT,

  MAX_INV_TYPE,
};
#else
*/
enum t_InvTYPE {
  INV_WEAPON = 0,
  INV_USE,
  INV_ETC,
  INV_RIDING,
  MAX_INV_TYPE
};

/*
#endif
*/

#define	INVENTORY_PAGE_SIZE		( 5 * 6 )
#define	INVENTORY_TOTAL_SIZE	( MAX_EQUIP_IDX + MAX_SHOT_TYPE + MAX_INV_TYPE*INVENTORY_PAGE_SIZE + MAX_RIDING_PART )
#define	INVENTORY_USE_ITEM0		( MAX_EQUIP_IDX + INVENTORY_PAGE_SIZE )				/// �Ҹ� ������ ���� �κ� ��ȣ
#define INVENTORY_ETC_ITEM0		( INVENTORY_USE_ITEM0  + INVENTORY_PAGE_SIZE )		/// ��Ÿ ������ ���� �κ� ��ȣ
#define	INVENTORY_SHOT_ITEM0	( MAX_EQUIP_IDX + MAX_INV_TYPE*INVENTORY_PAGE_SIZE )
#define	INVENTORY_RIDE_ITEM0	( INVENTORY_SHOT_ITEM0 + MAX_SHOT_TYPE )

#ifndef __SERVER///.nAvy.
#define INVENTORY_ITEM_INDEX_0		MAX_EQUIP_IDX	///���ӻ� �κ��丮�� ù��° INDEX NO
#define INVENTORY_ITEM_INDEX_LAST	MAX_EQUIP_IDX + MAX_INV_TYPE*INVENTORY_PAGE_SIZE - 1///���ӻ� �κ��丮�� ������ INDEX NO
#endif

enum t_eItemCLASS {
  JEWEL_ITEM_RING = 171,
  // ���� 
  JEWEL_ITEM_NECKLACE = 172,
  // �����
  JEWEL_ITEM_EARRING = 173,
  // �Ͱ���

  WEAPON_ITEM_USE_ARROW = 231,
  // SHOT_TYPE_ARROW �Ҹ��ϴ� ����
  WEAPON_ITEM_USE_ARROW2 = 271,
  // SHOT_TYPE_ARROW �Ҹ��ϴ� ����(�Ѽտ��Ÿ�����)
  WEAPON_ITEM_USE_BULLET = 232,
  // SHOT_TYPE_BULLET �Ҹ��ϴ� ����
  WEAPON_ITEM_USE_THROW = 233,
  // SHOT_TYPE_THROW �Ҹ��ϴ� ����
  WEAPON_ITEM_USE_BULLET2 = 253,
  // SHOT_TYPE_BULLET �Ҹ��ϴ� ����
  WEAPON_ITEM_NOT_USE_BULLET = 242,
  // SHOT_TYPE_BULLET �Ҹ��ϴ� ����

  WEAPON_ITEM_SHIELD = 261,
  // ���� ������

  USE_ITEM_SKILL_DOING = 313,
  // �Ҹ� ���������� ��ų ���
  USE_ITEM_SKILL_LEARN = 314,
  // �Ҹ� ���������� ��ų ���

  USE_ITEM_REPAIR_ITEM = 315,
  // �Ҹ� ���������� ��� ����
  USE_ITEM_EVENT_ITEM = 316,
  // ���� �̺�Ʈ ��ũ��..
  USE_ITEM_FUEL = 317,
  // ���� ������
  USE_ITEM_VOLATILITY_ITEM = 320,
  // ����� �ٷ� �Ҹ�Ǵ� ������

  USE_ITEM_MAINTAIN_ITEM = 321,
  // ���� �ð����� �����Ǵ� ������(â�����Ἥ��,â��Ȯ��...)

  TUNING_PART_BODY_CART = 511,
  // Ʃ�׺�ǰ - īƮ �ٵ�
  TUNING_PART_BODY_CASTLEGEAR = 512,
  // Ʃ�׺�ǰ - �ɽ���� �ٵ�
  TUNING_PART_ENGINE_CART = 521,
  // Ʃ�׺�ǰ - īƮ ����
  TUNING_PART_ENGINE_CASTLEGEAR = 522,
  // Ʃ�ú�ǰ - ĳ����� ����
  TUNING_PART_WEAPON_CASTLEGEAR = 552,
};

//*************************************************************************************************
/// �������� �ߺз� ���� --> STB ���� !!.
enum t_eITEM {
  ITEM_TYPE_FACE_ITEM = 1,
  // 1	LIST_FACEITEM.stb	�� ���	
  ITEM_TYPE_HELMET,
  // 2	LIST_CAP.stb
  ITEM_TYPE_ARMOR,
  // 3	LIST_BODY.stb
  ITEM_TYPE_GAUNTLET,
  // 4	LIST_ARMS.stb
  ITEM_TYPE_BOOTS,
  // 5	LIST_FOOT.stb
  ITEM_TYPE_KNAPSACK,
  // 6	LIST_BACK.stb
  ITEM_TYPE_JEWEL,
  // 7	LIST_JEWEL.stb		��ű� : ����� ����

  ITEM_TYPE_WEAPON,
  // 8	LIST_WEAPON.stb		����
  ITEM_TYPE_SUBWPN,
  // 9	LIST_SUBWPN.stb

  ITEM_TYPE_USE,
  // 10	LIST_USEITEM.stb	�Ҹ�

  ITEM_TYPE_ETC,
  // 11
  ITEM_TYPE_GEM = ITEM_TYPE_ETC,
  // 11	LIST_JEMITEM.stb	��Ÿ : ����
  ITEM_TYPE_NATURAL,
  // 12	LIST_NATURAL.stb
  ITEM_TYPE_QUEST,
  // 13	LIST_QUESTITEM.stb
  ITEM_TYPE_RIDE_PART=14,

  ITEM_TYPE_MONEY = 0x1f // 31	max item type value !!!
};

//*************************************************************************************************
/// NPC ��� �ε���
enum {
  MOB_ANI_STOP = 0,
  //	MOB_ANI_STOP2,
  MOB_ANI_MOVE,
  MOB_ANI_ATTACK,
  MOB_ANI_HIT,
  MOB_ANI_DIE,
  MOB_ANI_RUN,
  MOB_ANI_CASTION01,
  MOB_ANI_SKILL_ACTION01,
  MOB_ANI_CASTION02,
  MOB_ANI_SKILL_ACTION02,
  MOB_ANI_ETC,

  MAX_MOB_ANI
};

//*************************************************************************************************
/// �ƹ�Ÿ ��� �ε��� 
enum {
  AVT_ANI_STOP1 = 0,
  AVT_ANI_STOP2 = 1,
  AVT_ANI_WALK = 2,
  AVT_ANI_RUN = 3,
  AVT_ANI_SITTING = 4,
  AVT_ANI_SIT = 5,
  AVT_ANI_STANDUP = 6,
  AVT_ANI_STOP3 = 7,
  AVT_ANI_ATTACK = 8,
  AVT_ANI_ATTACK2 = 9,
  AVT_ANI_ATTACK3 = 10,

  AVT_ANI_HIT = 11,
  AVT_ANI_FALL = 12,
  AVT_ANI_DIE = 13,
  AVT_ANI_RAISE = 14,
  AVT_ANI_JUMP1 = 15,
  AVT_ANI_JUMP2 = 16,

  AVT_ANI_PICKITEM = 17,
  MAX_AVT_ANI
};

//*************************************************************************************************
/// �ƹ�Ÿ ���� ��Ī�ϴ� �ε���..
enum t_VarINDEX {
  SV_SEX = 0,
  SV_BIRTH,
  SV_CLASS,
  SV_UNION,
  SV_RANK,
  SV_FAME,
  SV_STR,
  SV_DEX,
  SV_INT,
  SV_CON,
  SV_CHA,
  SV_SEN,
  SV_EXP,
  SV_LEVEL,
  SV_POINT,
};

//*************************************************************************************************
/// �ƹ�Ÿ �ɷ�ġ ����
enum t_AbilityINDEX {
  AT_SEX = 2,
  // ��0,��1
  AT_BIRTHSTONE = 3,
  // ź����
  AT_CLASS = 4,
  // ����
  AT_UNION = 5,
  // �Ҽ�
  AT_RANK = 6,
  // ���
  AT_FAME = 7,
  // ����
  AT_FACE = 8,
  // �󱼸��
  AT_HAIR,

  AT_STR = 10,
  // �⺻ �ɷ�ġ
  AT_DEX,
  AT_INT,
  AT_CON,
  AT_CHARM,
  AT_SENSE = 15,

  AT_HP,
  AT_MP,
  AT_ATK,
  AT_DEF,
  AT_HIT,
  AT_RES = 21,
  AT_AVOID,
  AT_SPEED,
  AT_ATK_SPD,
  AT_WEIGHT,
  AT_CRITICAL = 26,
  AT_RECOVER_HP,
  AT_RECOVER_MP,

  AT_SAVE_MP = 29,

  AT_EXP = 30,
  AT_LEVEL,
  AT_BONUSPOINT = 32,

  AT_PK_FLAG = 33,
  AT_TEAM_NO = 34,

  AT_HEAD_SIZE,
  AT_BODY_SIZE,
  AT_SKILLPOINT = 37,

  AT_MAX_HP = 38,
  // 2004. 2. 3 �߰� 
  AT_MAX_MP,

  AT_MONEY = 40,

  /// �нú� ��ų �ɷ�ġ..
  AT_PSV_ATK_POW_NO_WEAPON = 41,
  AT_PSV_ATK_POW_1HAND_WPN = 42,
  // ���ݷ� : �Ѽ�	<< AT_PSV_ATK_POW_SWORD �� ���� �Ѽ����� ���� 2004. 2. 26 by icarus
  AT_PSV_ATK_POW_2HAND_WPN = 43,
  //			���	<< AT_PSV_ATK_POW_AXE	�б⿡�� ����� ����
  AT_PSV_ATK_POW_BOW = 44,
  AT_PSV_ATK_POW_GUN = 45,
  AT_PSV_ATK_POW_STAFF_WAND = 46,
  AT_PSV_ATK_POW_AUTO_BOW = 47,
  // ����
  AT_PSV_ATK_POW_KATAR_PAIR = 48,
  // �̵���

  AT_PSV_ATK_SPD_BOW = 49,
  AT_PSV_ATK_SPD_GUN = 50,
  AT_PSV_ATK_SPD_PAIR = 51,
  // �̵���

  AT_PSV_MOV_SPD = 52,
  AT_PSV_DEF_POW,
  AT_PSV_MAX_HP,
  AT_PSV_MAX_MP = 55,
  AT_PSV_RECOVER_HP,
  AT_PSV_RECOVER_MP,
  AT_PSV_WEIGHT = 58,

  AT_PSV_BUYING = 59,
  // ���� ���� ���� ���
  AT_PSV_SELLING = 60,
  // �ȱ� ���� ���� ���

  AT_PSV_SAVE_MP = 61,
  // MP ����

  AT_PSV_SUMMON_MOB_CNT = 62,
  // ��ȯ���� ����
  AT_PSV_DROP_RATE = 63,
  // ��� Ȯ��

  AT_AFTER_PASSIVE_SKILL = 71,
  // �нú� ��ų �����ɷ�ġ ����...
  AT_RACE = AT_AFTER_PASSIVE_SKILL,
  // ���� init_avatar.stb ���ι�ȣ / 2

  // ���� 2004.5.27 �߰�
  AT_DROP_RATE = 72,
  // ��� ������
  AT_FAME_G = 73,
  AT_FAME_B = 74,

  AT_CURRENT_PLANET = 75,
  // ���� �÷����ϰ� �ִ� �༺ ��ȣ

  AT_STAMINA = 76,
  // ���׹̳�
  AT_FUEL = 77,
  // ���� ����..

  AT_IMMUNITY,

  AT_UNION_POINT1 = 81,
  // ���պ� ����Ʈ
  AT_UNION_POINT2,
  AT_UNION_POINT3,
  AT_UNION_POINT4,
  AT_UNION_POINT5,
  AT_UNION_POINT6,
  AT_UNION_POINT7,
  AT_UNION_POINT8,
  AT_UNION_POINT9,
  AT_UNION_POINT10 = 90,

  AT_GUILD_NO = 91,
  // ��� ��ȣ
  AT_GUILD_SCORE,
  // ��� Ŭ�� ����
  AT_GUILD_POS,
  // ��� ����

  AT_MAINTAIN_ABILITY = 94,
  AT_BANK_FREE = AT_MAINTAIN_ABILITY,
  // â�� ���� 
  AT_BANK_ADDON = 95,
  // â�� Ȯ��
  AT_STORE_SKIN = 96,
  // â�� ���
  AT_PATHP = 97,

  AT_PSV_RES,
  AT_PSV_HIT,
  AT_PSV_CRITICAL,
  AT_PSV_AVOID,
  AT_PSV_SHIELD_DEF,
  AT_PSV_IMMUNITY,
  AT_AFTER_PASSIVE_SKILL_2ND,

  AT_EXP_BOOST = 106,
  // Raven
  AT_DROP_BOOST = 107,
  // Raven
  AT_EXP_NONE = 108,
  // Raven
  AT_MAX
};

//*************************************************************************************************
/// ������ ��밡���� �ɷ�ġ��..
enum t_BasicAbilityINDEX {
  BA_STR=0,
  // 			short	m_nSTR;			// �ٷ� 1~200
  BA_DEX,
  // short	m_nDEX;			// ��ø 1~200
  BA_INT,
  // short	m_nINT;			// ���� 1~200
  BA_CON,
  // short	m_nCON;			// concentration ���߷� 1~200
  BA_CHARM,
  // short	m_nCHARM;		// �ŷ� 1~200
  BA_SENSE,
  // short	m_nSENSE;		// ���� 1~200

  BA_MAX
};

//*************************************************************************************************
// �ƹ�Ÿ ���� ����ǥ.		//
// #define	CLASS_VISITOR		0	// 0~9 �湮��
//
// #define	CLASS_SOLDIER_111	111	// 1�� ����
// #define	CLASS_SOLDIER_121	121	// 2�� ����Ʈ
// #define	CLASS_SOLDIER_122	122	// 2�� è��
// #define	CLASS_SOLDIER_131	131	// 3�� ����...
// #define	CLASS_SOLDIER_132	132	// 3�� ����...
//
// #define	CLASS_MAGICIAN_211	211	// 1�� ����
// #define	CLASS_MAGICIAN_221	221	// 2�� ������
// #define	CLASS_MAGICIAN_222	222	// 2�� Ŭ����
// #define	CLASS_MAGICIAN_231	231	// 3�� ����..
// #define	CLASS_MAGICIAN_232	232	// 3�� ����..
//
// #define	CLASS_MIXER_311		311	// 1�� ȣĿ
// #define	CLASS_MIXER_321		321	// 2�� ���̴�
// #define	CLASS_MIXER_322		322	// 2�� ��ī��Ʈ
// #define	CLASS_MIXER_331		331	// 3�� ����
// #define	CLASS_MIXER_332		333	// 3�� ����
//
// #define CLASS_MERCHANT_411	411	// 1�� ����
// #define CLASS_MERCHANT_421	421	// 2�� �����־�
// #define CLASS_MERCHANT_422	422	// 2�� ��Ƽ��
// #define CLASS_MERCHANT_431	431	// 3�� ����
// #define CLASS_MERCHANT_432	432	// 3�� ����

//*************************************************************************************************
/// ���� ���� ����.
enum eING_TYPE {
  ING_NULL=0,

  ING_PROC = 1,
  ING_INC_HP = ING_PROC,
  // ó�� : hp ��ȭ
  ING_INC_MP,
  // ó�� : mp ��ȭ
  ING_POISONED,
  // ó�� : �ߵ�

  ING_INC_MAX_HP=4,
  // ���� : �ִ� hp
  ING_INC_MAX_MP,
  // ���� : �ִ� mp

  ING_CHECK_START = 6,

  ING_INC_MOV_SPD=ING_CHECK_START,
  // ���� : �̼�/����
  ING_DEC_MOV_SPD,
  // ���� : �̼�/����
  ING_INC_ATK_SPD=8,
  ING_DEC_ATK_SPD,
  ING_INC_APOWER=10,
  // ���� : ���ݷ�
  ING_DEC_APOWER,
  ING_INC_DPOWER=12,
  // ���� : ����
  ING_DEC_DPOWER,
  ING_INC_RES=14,
  // ���� : �׸���
  ING_DEC_RES,
  ING_INC_HIT=16,
  // ���� : ����
  ING_DEC_HIT,
  ING_INC_CRITICAL=18,
  // ���� : ũ��Ƽ��
  ING_DEC_CRITICAL,
  ING_INC_AVOID=20,
  // ���� : ȸ�Ƿ� 
  ING_DEC_AVOID,
  ING_DUMB=22,
  // ���� : ��� : skill stb 5�� �÷��� 7~14�� ��ų ��� ����.
  ING_SLEEP,
  // ���� : ���� : ���� �Ҵ� ���·�.. ���� ������ Ǯ��
  ING_FAINTING,
  // ���� : ���� : ����� ������ ���� �޾Ƶ� ��Ǯ��
  ING_DISGUISE,
  // ���� : ���� : 
  ING_TRANSPARENT,
  // ���� : ���� :
  ING_SHIELD_DAMAGE=27,
  // ���� : ���� ������
  ING_DUMMY_DAMAGE=28,
  // ���� : �߰� ������

  ING_CHECK_END = ING_DUMMY_DAMAGE,

  ING_DEC_LIFE_TIME=29,
  // ó�� : ��ȯ�� �ɸ��� ���� �ð�...	�߰�: 2004. 4. 6 

  ING_CLEAR_GOOD,
  // �ﰢ : ���� ���� ����
  ING_CLEAR_BAD,
  // �ﰢ : �Ҹ� ���� ����
  ING_CLEAR_ALL,
  // �ﰢ : ��� ���� ����
  ING_CLEAR_INVISIBLE,
  // �ﰢ : �������� ����(ING_DISGUISE,ING_TRANSPARENT) 

  ING_TAUNT,
  // ���� : Ÿ��Ʈ
  ING_REVIVE,
  // ���� : ��Ȱ

  ING_MAX_STATUS
};

//*************************************************************************************************
/// ���� ���� ���� �÷��� 
#define	FLAG_ING_HP					0x00000001
#define	FLAG_ING_MP					0x00000002
#define	FLAG_ING_POISONED			0x00000004

#define	FLAG_ING_MAX_HP				0x00000010
#define	FLAG_ING_MAX_MP				0x00000020
#define	FLAG_ING_INC_MOV_SPEED		0x00000040
#define	FLAG_ING_DEC_MOV_SPEED		0x00000080
#define	FLAG_ING_INC_ATK_SPEED		0x00000100
#define	FLAG_ING_DEC_ATK_SPEED		0x00000200
#define	FLAG_ING_INC_APOWER			0x00000400
#define	FLAG_ING_DEC_APOWER			0x00000800
#define	FLAG_ING_INC_DPOWER			0x00001000
#define	FLAG_ING_DEC_DPOWER			0x00002000
#define	FLAG_ING_INC_RES			0x00004000
#define	FLAG_ING_DEC_RES			0x00008000
#define	FLAG_ING_INC_HIT			0x00010000
#define FLAG_ING_DEC_HIT			0x00020000
#define	FLAG_ING_INC_CRITICAL		0x00040000
#define	FLAG_ING_DEC_CRITICAL		0x00080000
#define	FLAG_ING_INC_AVOID			0x00100000
#define	FLAG_ING_DEC_AVOID			0x00200000
#define	FLAG_ING_DUMB				0x00400000
#define	FLAG_ING_SLEEP				0x00800000
#define	FLAG_ING_FAINTING			0x01000000
#define	FLAG_ING_DISGUISE			0x02000000
#define	FLAG_ING_TRANSPARENT		0x04000000
#define	FLAG_ING_SHIELD_DAMAGE		0x08000000
#define	FLAG_ING_DUMMY_DAMAGE		0x10000000

#define	FLAG_ING_DEC_LIFE_TIME		0x20000000		/// ��ȯ�� �ɸ����� �ʴ� HP����..
#define	FLAG_ING_REVIVE				0x40000000		/// ��Ȱ...
#define	FLAG_ING_TAUNT				0x80000000		/// Ÿ��Ʈ ��ų ������

/// ���� ���� �����... �̵�,����, ��� Ż����, �Ҹ� ������ ���, ��ȯ �Ҽ� ����.
#define FLAG_ING_IGNORE_ALL			( FLAG_ING_SLEEP | FLAG_ING_FAINTING )

// ���� ���� 
#define	FLAG_ING_INVISIBLE			( FLAG_ING_DISGUISE | FLAG_ING_TRANSPARENT )

#define	FLAG_ING_PROC_PER_TICK		( FLAG_ING_DEC_LIFE_TIME | FLAG_ING_POISONED )	/// �� ƽ���� ó��
#define	FLAG_ING_CHECK_PER_TICK		( 0xfffffff0 )									/// �� ƽ���� ���� �ð��� üũ�ϸ�� 

#define	FLAG_ING_BAD	(	FLAG_ING_POISONED		| FLAG_ING_DEC_MOV_SPEED| FLAG_ING_DEC_ATK_SPEED	\
              | FLAG_ING_DEC_APOWER		| FLAG_ING_DEC_DPOWER	| FLAG_ING_DEC_RES			\
              | FLAG_ING_DEC_HIT		| FLAG_ING_DEC_CRITICAL | FLAG_ING_DEC_AVOID		\
              |	FLAG_ING_DUMB			| FLAG_ING_SLEEP		| FLAG_ING_FAINTING	)
#define	FLAG_ING_GOOD	(	FLAG_ING_MAX_HP			| FLAG_ING_MAX_MP		| FLAG_ING_INC_MOV_SPEED	\
              | FLAG_ING_INC_ATK_SPEED	| FLAG_ING_INC_APOWER	| FLAG_ING_INC_DPOWER		\
              | FLAG_ING_INC_RES		| FLAG_ING_INC_HIT		| FLAG_ING_INC_CRITICAL		\
              | FLAG_ING_INC_AVOID		| FLAG_ING_SHIELD_DAMAGE| FLAG_ING_DUMMY_DAMAGE		\
              | FLAG_ING_DISGUISE		| FLAG_ING_TRANSPARENT	)
/// ��� ���� ������ dwSTATUS & FLAG_ING_CLEAR�� �����ؾ� ��...
#define	FLAG_ING_CLEAR	(	FLAG_ING_DEC_LIFE_TIME | FLAG_ING_STORE_MODE | FLAG_ING_INTRO_CHAT )

//*************************************************************************************************
/// ���� ���� ���� ���� �÷��� 
#define	FLAG_SUB_HIDE				0x00000001		// GM Hide��� : �� �÷��� ������ �ɸ��ʹ� �������� �ʴ´�.
#define	FLAG_SUB_STORE_MODE			0x00000002		/// ���� ���� ���̴�... ���� �Ұ�, �Ҹ� ������ ��밡��
#define	FLAG_SUB_INTRO_CHAT			0x00000004		// �Ұ���

#define	FLAG_SUB_ARUA_FAIRY			0x40000000		// �Ʒ�� ���� ����...
//#define	FLAG_SUB_PROC_PER_TICK
//#define	FLAG_SUB_CHECK_PER_TICK	  ( 0xfffffff0 )	// �� ƽ���� ���� �ð��� üũ�ϸ��

//*************************************************************************************************
/// eING_TYPE�� ���� ���·� ���� �÷��� �� �̴� ���̺�..
#ifndef	__ADTOOLS
const DWORD c_dwIngFLAG[] = {
  0,                 //	ING_NULL,			
  FLAG_ING_HP,       //	ING_INC_HP,			
  FLAG_ING_MP,       //	ING_INC_MP,			
  FLAG_ING_POISONED, //	ING_POISONED,		

  FLAG_ING_MAX_HP,        //	ING_INC_MAX_HP,		
  FLAG_ING_MAX_MP,        //	ING_INC_MAX_MP,		
  FLAG_ING_INC_MOV_SPEED, //	ING_INC_MOV_SPD,	
  FLAG_ING_DEC_MOV_SPEED, //	ING_DEC_MOV_SPD,	
  FLAG_ING_INC_ATK_SPEED, //	ING_INC_ATK_SPD,	
  FLAG_ING_DEC_ATK_SPEED, //	ING_DEC_ATK_SPD,	
  FLAG_ING_INC_APOWER,    //	ING_INC_APOWER,		
  FLAG_ING_DEC_APOWER,    //	ING_DEC_APOWER,		
  FLAG_ING_INC_DPOWER,    //	ING_INC_DPOWER,		
  FLAG_ING_DEC_DPOWER,    //	ING_DEC_DPWOER,		
  FLAG_ING_INC_RES,       //	ING_INC_RES,		
  FLAG_ING_DEC_RES,       //	ING_DEC_RES,		
  FLAG_ING_INC_HIT,       //	ING_INC_HIT,		
  FLAG_ING_DEC_HIT,       //	ING_DEC_HIT,		
  FLAG_ING_INC_CRITICAL,  //	ING_INC_CRITICAL,	
  FLAG_ING_DEC_CRITICAL,  //	ING_DEC_CRITICAL,	
  FLAG_ING_INC_AVOID,     //	ING_INC_AVOID,		
  FLAG_ING_DEC_AVOID,     //	ING_DEC_AVOID,		
  FLAG_ING_DUMB,          //	ING_DUMB,				// ���� : ��� : skill stb 5�� �÷��� 7~14�� ��ų ��� ����.
  FLAG_ING_SLEEP,         //	ING_SLEEP,				// ���� : ���� : ���� �Ҵ� ���·�.. ���� ������ Ǯ��
  FLAG_ING_FAINTING,      //	ING_FAINTING,			// ���� : ���� : ����� ������ ���� �޾Ƶ� ��Ǯ��
  FLAG_ING_DISGUISE,      //	ING_DISGUISE,			// ���� : ���� : 
  FLAG_ING_TRANSPARENT,   //	ING_TRANSPARENT,		// ���� : ���� :
  FLAG_ING_SHIELD_DAMAGE, //	ING_SHIELD_DAMAGE,		// ���� : ���� ������
  FLAG_ING_DUMMY_DAMAGE,  //	ING_DUMMY_DAMAGE,		// ���� : �߰� ������

  FLAG_ING_DEC_LIFE_TIME, // ING_DEC_LIFE_TIME		// �߰�: 2004. 4. 6 

  0, // ING_CLEAR_GOOD,			// �ﰢ : ���� ���� ����
  0, // ING_CLEAR_BAD,			// �ﰢ : �Ҹ� ���� ����
  0, // ING_CLEAR_ALL,			// �ﰢ : ��� ���� ����
  0, // ING_CLEAR_INVISIBLE,		// �ﰢ : �������� ����(ING_DISGUISE,ING_TRANSPARENT) 

  FLAG_ING_TAUNT,  // ING_TAUNT				���� : Ÿ��Ʈ
  FLAG_ING_REVIVE, // ING_REVIVE
};
#endif

//*************************************************************************************************
//*************************************************************************************************
#define	DEAD_HP				  ( -30000 )						/// ���� ������ HP��.
// #define	MAX_UNION_COUNT			10								/// �ִ� ���� ����

#define	ITEM_OBJ_FREE_TIME		  (    30 * 1000 )						/// ������ �������� ���� �ð�...
#define	ITEM_OBJ_LIVE_TIME		( (    60 * 1000 )+ITEM_OBJ_FREE_TIME )	/// ������ ó�� ������ ������� ���� �ð�.
#define	ITEM_OBJ_DROP_TIME		( (30*(60 * 1000))+ITEM_OBJ_FREE_TIME )	/// �κ��丮�� ���ڶ� �ٴۿ� ������ ������

#define	MIN_OBJECTS				1
#define	MAX_SERVER_OBJECTS		0x0ffff
#ifndef	__SERVER
#define	MAX_CLIENT_OBJECTS		4096
#endif

#define	MUL02(V)				( (V) << 1 )
#define	MUL04(V)				( (V) << 2 )
#define	MUL08(V)				( (V) << 3 )
#define	MUL16(V)				( (V) << 4 )

#define	DIV02(V)				( (V) >> 1 )
#define	DIV04(V)				( (V) >> 2 )
#define	DIV08(V)				( (V) >> 3 )
#define	DIV16(V)				( (V) >> 4 )

#define	RANDOM(V)				( rand()% V )
extern const float g_fMinHeight;
extern const float g_fMaxDistance;

#define AVT_CLICK_EVENT_RANGE	1000		/// �ƹ�Ÿ�� Ŭ�������� ������ �ּ� �Ÿ�
#define NPC_CLICK_EVENT_RANGE	250			/// NPC�� Ŭ�������� ������ �ּ� �Ÿ�
#define	ITEM_CLICK_EVENT_RANGE	150			/// ITEM Ŭ�������� ������ �ּ� �Ÿ�

#define PAT_DEFAULT_HP			1000		// PAT HP�� �⺻ ��
//-------------------------------------------------------------------------------------------------
#endif
