/*
	$Header: /Client/Game_FUNC.h 34    05-06-19 5:33p Navy $
*/
#ifndef	__GAME_FUNC_H
#define	__GAME_FUNC_H
#include "LUA_Func.h"
//-------------------------------------------------------------------------------------------------

#define	GF_SCRIPT
typedef const char*	ZSTRING;

//-------------------------------------------------------------------------------------------------
GF_SCRIPT
void	GF_log ( ZSTRING szStr );

GF_SCRIPT
int		GF_GetEffectUseFile ( ZSTRING szEffectFile );

GF_SCRIPT
int		GF_GetEffectUseIndex ( int iEffectIndex );

GF_SCRIPT
void	GF_EffectOnObject ( int iObject, int iEffectHash );

GF_SCRIPT
void	GF_WeatherEffectOnObject ( int iObject, int iEffectHash );

GF_SCRIPT
void	GF_DeleteEffectFromObject ( int iObject );

GF_SCRIPT
int		GF_GetTarget ( int iObject );

GF_SCRIPT
int		GF_GetMotionUseFile ( ZSTRING szMotionFile );

GF_SCRIPT
void	GF_SetMotion ( int iObject, int iMotionHashKEY, int iRepeatCnt );

GF_SCRIPT
void	GF_error ( void );


/*
=================================================================================
	���� ȯ�� ������
=================================================================================
*/
GF_SCRIPT
int		GF_getVariable ( int iVarTYPE );

GF_SCRIPT
ZSTRING	GF_getName ( void );

GF_SCRIPT
void	GF_setVariable ( int iVarTYPE, int iValue );

GF_SCRIPT
int		GF_getZone ( void );

GF_SCRIPT
ZSTRING	GF_getDate ( void );

GF_SCRIPT
ZSTRING	GF_getGameVersion ( void );

enum {
	GTV_POPULATION=0,
	GTV_PRICE,
	GTV_LIFEQUALITY,
} ;

GF_SCRIPT
int		GF_getTownVar ( int iVarTYPE );


/*
=================================================================================
	���� ���� : GF_System.cpp
=================================================================================
*/

/* TOWN_RATE �����ϱ� */
GF_SCRIPT
void	GF_setTownRate ( int iTownRate );

/* TOWN_RATE �������� */
GF_SCRIPT
int		GF_getTownRate ( void );

/* WORLD_RATE �������� */
GF_SCRIPT
int		GF_getWorldRate ( void );

/* WORLD_RATE �����ϱ� */
GF_SCRIPT
void	GF_setWorldRate ( int iWorldRate );

/* ������ ���� �˾Ƴ��� */
GF_SCRIPT
int		GF_getItemRate ( int iItemTYPE );

/* ������ ������ �������� */
GF_SCRIPT
int		GF_checkTownItem ( int iItemTYPE );



/*
=================================================================================
������ �κ��丮�� �ֱ� / �����ϱ� / ���� �˾Ƴ���
=================================================================================
*/

/* ������ �ε��� �˾Ƴ��� */
GF_SCRIPT
int		GF_getIDXOfInvItem ( int nItemNo, int nItemType );

/* ������ ���� �˾Ƴ��� */
GF_SCRIPT
int		GF_checkNumOfInvItem ( int nItemNo, int nItemType );

/* ��� ������ �κ��� �ֱ� */
GF_SCRIPT
void	GF_giveEquipItemIntoInv ( int cQuality, int cResmelt, int cJAMMING3, int cJAMMING2, int cJAMMING1, int nItemNo, int cType );

/* ������ �κ����� ������ */
GF_SCRIPT
void	GF_takeItemFromInv ( int nNum, int nItemNo, int cType );

/* �Ҹ�/��Ÿ ������ �ֱ� */
GF_SCRIPT
void	GF_giveUsableItemIntoInv ( int iQuantity, int iItemNo, int iItemType );

/*
=================================================================================
	�ƹ�Ÿ ������ ���� / �����ϱ�
=================================================================================
*/

/* �ƹ�Ÿ ������ ����/�����ϱ� bWear : 1(����), 0(����) , �κ��丮�� ��� ���� */
GF_SCRIPT
void	GF_setEquipedItem ( int nPartIDX, int iItemIDX, int bWear );

/* ������ ���� (���� ==> �κ�) */
GF_SCRIPT
void	GF_putoffItem ( int iPartIDX, int iType );

/* ������ ���� (�κ� ==> ����) */
GF_SCRIPT
void	GF_putonItem ( int iPartIDX, int iItemNo, int iItemType );


/*
=================================================================================
	�ƹ�Ÿ ���� ���� : GF_ITEM.CPP
=================================================================================
*/

/* �ƹ�Ÿ ���ݼ����� ���� */
GF_SCRIPT
int		GF_checkUserMoney ( void );


/* �ƹ�Ÿ ���� �ֱ� */
GF_SCRIPT
void	GF_addUserMoney ( int iAmount );

/* �ƹ�Ÿ �������� �������� */
GF_SCRIPT
void	GF_takeUserMoney ( int iAmount );

/*
=================================================================================
	����, ����Ʈ ����...	GF_EFFECT.cpp
=================================================================================
*/
GF_SCRIPT
void	GF_playSound ( ZSTRING szSoundFile,	int iRepeat, int iOuterOpt );

// GF_SCRIPT
// void	GF_playEventSound ( int iEventNo, ZSTRING szSoundFile, int iRepeat, int iOuterOpt );


/*
=================================================================================
	ī�޶� ����...
=================================================================================
*/
GF_SCRIPT
void	GF_rotateCamera ( int iPole, float iDegree );

GF_SCRIPT
void	GF_zoomCamera ( int iPercent );

GF_SCRIPT
void	GF_playEffect ( int iEffect, float x, float y );

// GF_SCRIPT
// void	GF_playEffectAtEvent ( int iEventNo, ZSTRING szEffectName );

/*
=================================================================================
�̵� / ����
=================================================================================
*/
GF_SCRIPT
void	GF_moveXY ( int iMapNo, int iX, int iY );

GF_SCRIPT
int		GF_movableXY ( int iMapNo, int iX, int iY );

GF_SCRIPT
int		GF_moveEvent ( int iEffectNo );

GF_SCRIPT
void	GF_warp ( int iWarpIndex );

GF_SCRIPT
ZSTRING	GF_getReviveZoneName ( );

GF_SCRIPT
void	GF_setRevivePosition ( );


/*
=================================================================================
	����...
=================================================================================
*/
GF_SCRIPT
void	GF_spawnMonXY ( int iMonNo, int iMonLevel, int iX, int iY );

GF_SCRIPT
void	GF_spawnMonAtEvent ( int iMonNo, int iMonLevel, int iEvnetNo );


/*
=================================================================================
���� ����
=================================================================================
*/
GF_SCRIPT
void	GF_openStore ( int iNPCNO, int bSpecialTab );

/*
=================================================================================
����â�� ����
=================================================================================
*/
GF_SCRIPT
void	GF_openBank ( int iNPCNO );

/*
=================================================================================
���ϸ���������â�� ����
=================================================================================
*/

GF_SCRIPT
void	GF_openDeliveryStore ( );


/*
=================================================================================
���� ����
=================================================================================
*/
GF_SCRIPT
void	GF_repair ( int iNPCNO );


/*
=================================================================================
�и� / ����
=================================================================================
*/
GF_SCRIPT
void	GF_openSeparate ( int iNPCNO );


/*
=================================================================================
����
=================================================================================
*/
GF_SCRIPT
void	GF_openUpgrade ( int iNPCNO );


/*
=================================================================================
������ ����
=================================================================================
*/
GF_SCRIPT
void	GF_appraisal ( );

/*
=================================================================================
Ŭ�� â�� / ��ü
=================================================================================
*/
GF_SCRIPT
void	GF_organizeClan ( int iNPCNO );

GF_SCRIPT
void	GF_disorganizeClan ( int iNPCNO );

//----------------------------------------------------------------------------------------------------
///
/// ���ӳ� �̺�Ʈ�� ���� ȿ�� ���
///
//----------------------------------------------------------------------------------------------------

/*
�ƹ�Ÿ ����
*/

/// �ƹ�Ÿ ó�� ������ �� ����� ȿ��
void SE_CharCreate  ( int iObject );
/// �ƹ�Ÿ�� ó�� ���忡 �����Ҷ�
void SE_CharJoinZone( int iObject ); 
/// �ƹ�Ÿ �����
void SE_CharDie		( int iObject );
/// �ƹ�Ÿ ����
void SE_CharWarp	( int iObject );
/// �ƹ�Ÿ ������
void SE_CharLevelUp ( int iObject );
/// ��Ƽ ������
void SE_PartyLevelUp ( int iObject );

///���� ����
void SE_StartMake ( int iObject );
///���� ����
void SE_StartUpgrade ( int iObject );
/// ���� ����
void SE_SuccessMake ( int iObject );
/// ���� ����
void SE_SuccessUpgrade ( int iObject );
/// �и�/���� ����
void SE_SuccessSeparate ( int iObject );
/// ���� ����
void SE_FailMake ( int iObject );
///���� ����
void SE_FailUpgrade ( int iObject );

///��ų������
void SE_SkillLevelUp ( int iObject );




/*
���� ����
*/

/// ���� ������
void SE_MobDie ( int iObject );



///
/// Log/OutputDebugString
///
GF_SCRIPT
void GF_LogString ( ZSTRING szMessage );

/*
�����ѹ� ���׷�����
*/
GF_SCRIPT
int	GF_Random ( int iMax );


bool InitScript ();
void FreeScript ();




//------------------------------------------------------------------------------------------------
/// ���� ����Ʈ
//------------------------------------------------------------------------------------------------
void SE_WeatherEffect ( int iObject, int iWeatherType );




//------------------------------------------------------------------------------------------------
/// �ƾ� ������
//------------------------------------------------------------------------------------------------
GF_SCRIPT
int GF_ChangeState ( int iNewState );



//-------------------------------------------------------------------------------------------------
#endif

