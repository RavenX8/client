/*
  $Header: /Client/CObjCHAR.h 353   05-10-20 2:45p Gioend $
*/
#ifndef	__COBJCHAR_H
#define	__COBJCHAR_H
#include "IO_Model.h"
#include "CModelCHAR.h"
#include "Common/CObjAI.h"
#include "Common/io_stb.h"
#include "Common/IO_AI.H"
#include "Net_Prototype.h"
#include "Interface/CEnduranceProperty.h"
#include "GameProc/CDayNNightProc.h"
#include "GameProc/Objvibration.h"

#include "ObjectCommand/ObjCommand.h"

#include <vector>
#include <numeric>

#ifdef	__VIRTUAL_SERVER
#include "Common\\CRegenAREA.h"
#endif

#ifndef	__SERVER
#include "GameProc/ChangeActionMode.h"
#endif

#define	QUEST_EVENT_ON_DEAD		0

const int INVALID_DUMMY_POINT_NUM = 999;

//struct uniDAMAGE;

///
/// Damage class
///
struct tagDAMAGE {
  short   m_nTargetObjIDX;
  int64_t m_wDamage;
};

class CChangeVisibility;
class CObjCHAR_Collision;
class CObjFIXED;
class CClanMarkUserDefined;
//박지호 
class CObjCART;
class CObjAVT;

///
///	CObjCHAR class inherited from COjbAI
/// This is base class of AVATA, MOB class.
///

class CObjCHAR : public CObjAI {

protected:
  int m_iHP;
  int m_iMP;
  int m_iMaxHP;
  int m_iMaxMP;

  CChangeVisibility* m_pChangeVisibility;
  bool               m_bStopDead; /// Stop 상태로 사망..( 즉 죽는 모션으로의 전환이 없다. )

public:
  float m_fHeightOfGround; // 기본 지형에서 캐릭터 중심점 높이( 상대 높이 )
  bool  m_bDead;           /// 서버로부터 Dead damage 를 받았는가?
  /// 설정상의 고도( 비행유닛의 고도 )
  float m_fAltitude;

protected :
  CObjCHAR_Collision* m_pCollision; // 충돌 정보
  float               m_fStature;   // 신장(키)
  float               m_fScale;
  HNODE               m_hNodeGround; // 밟고 있는 오브젝트의 엔진 핸들

  bool  m_bUseResetPosZ; // Z값을 새로 갱신할 지 여부.
  float m_fResetPosZ;    // 새로 갱신할 Z값. 다음 높이 계산 때, 이 값으로 갱신되어야 한다.
  float m_fModelSPEED;   // 모델 이동 속도.

  /// 팀정보..
  int m_iTeamInfo; // 팀 정보

  void Add_EFFECT(CMODEL<CCharPART>* pCharPART, short nPartIDX, short nPointIDX, t_HASHKEY HashEffectFILE, bool bLinkNODE                    = true);
  void LoadTRAIL(CMODEL<CCharPART>*  pCharPART, short nPartIDX, bool  bLinkBONE, int       iColorNO, int        iDuration, int iBaseDummyIdx = 0);
  void UnloadTRAIL(short             nPartIDX);

  CAI_OBJ* AI_FindFirstOBJ(int iDistance) override;
  CAI_OBJ* AI_FindNextOBJ() override;

  bool m_bFrameING;

protected:
  CCharMODEL* m_pCharMODEL;

  CEffect** m_ppBoneEFFECT;
  short     m_nEffectPointCNT[ MAX_BODY_PART ];
  CEffect** m_pppEFFECT[ MAX_BODY_PART ]; // 기본 효과.

  int m_iLastDummyIDX;

  HNODE m_hNodeMODEL;

  /// Model nodes..

  union {
    HNODE* m_phPartVIS[ MAX_BODY_PART ];

    struct {
      // 순서는 t_CharPART에 따라서...
      HNODE* m_phFaceVIS;
      HNODE* m_phHairVIS;
      HNODE* m_phHelmetVIS;

      HNODE* m_phArmorVIS;
      HNODE* m_phGauntletVIS;
      HNODE* m_phBootsVIS;

      HNODE* m_phGoggleVIS;
      HNODE* m_phKnapsackVIS;

      HNODE* m_phRWeaponVIS;
      HNODE* m_phLWeaponVIS;
    };
  };

  /// Nodes of Trailes..
  union {
    HNODE m_hTRAIL[ 2 ]; // right=0, left=1
    struct {
      HNODE m_hR_TRAIL;
      HNODE m_hL_TRAIL;
    };
  };

  union {
    short m_nCharIdx;  // NPC, MOB 번호
    bool  m_bIsFemale; // 아바타: 남자0, 여자 1
    BYTE  m_btRace;    // 종족...
  };

  int  GetCurrentFrame() override { return getMotionFrame( this->m_hNodeMODEL ); }
  void ActionEVENT(int iActionIDX) override;

  DWORD m_dwLastRecoveryUpdateTime; ///HP와 MP가 자동 회복되어진 이전 시간	
  DWORD m_dwFrameElapsedTime;       /// 이전 처리에서.. 흐른시간..
  DWORD m_dwElapsedTime;            /// 이전 처리에서.. 흐른시간..

public:
  void ClearTimer();

  CObjCHAR_Collision* GetCollisionUtility() { return m_pCollision; }

private:

  ///
  ///	Process action frame..
  ///
  void ActionSpecialFrame(int iActionIDX);

  void ActionInFighting(int     iActionIDX);
  void ActionBow(int            iActionIDX);
  void ActionGun(int            iActionIDX);
  void ActionSkill(int          iActionIDX);
  void ActionImmediateSkill(int iActionIDX);

  void FireEffectBullet(CObjCHAR* pTarget, int iBulletIDX, bool bDummy, int iSkillIDX);

  /// 44,64,75, 45,65,75 처리..
  void ShowCastingEffect(int       iActionIDX);
  void ActionSkillCastingFrame(int iCastingOrder);
  /// 연타공격 효과( 더미.. )
  void RepeatedlyHitEffect(int iHitOrder);

  enum {
    LEFT_WEAPON = 0,
    RIGHT_WEAPON,
  };

public:
  void Adjust_HEIGHT(void        ) override;
  void ResetCUR_POS(D3DXVECTOR3& Pos);

  /// @brief 서버에서 받은 높이값으로 다음번 Adjust_Height()에서 갱신되는 높이값 설정.
  /// @param bUseResetPosZ 갱신 높이값을 사용할지 여부. 높이값을 설정할 때에는 true, 초기화할 때에는 false를 입력한다.
  /// @param fPosZ 새로 갱신될 높이값.
  /// @see GetResetPosZ()
  void SetResetPosZ(bool bUseResetPosZ, float fPosZ) {
    if ( m_bUseResetPosZ = bUseResetPosZ ) {
      // = 오타아님.
      m_fResetPosZ = fPosZ;
    }
  }

  /// @brief 서버에서 받은 높이값이 있는지의 여부 및 그 높이값을 얻는다.
  /// @param fResetPosZ 새로 변경할 높이 값
  /// @return 유효하다면 true, 유효하지 않다면 false 를 리턴
  /// @see SetResetPosZ()
  bool GetResetPosZ(float& fResetPosZ) {
    if ( m_bUseResetPosZ ) {
      fResetPosZ = m_fResetPosZ;
      return true;
    }
    return false;
  }

  bool LoadModelNODE(char* szName);
  void UnloadModelNODE();

  void Link_EFFECT(void  );
  void Unlink_EFFECT(void);

  void New_EFFECT(short nPartIdx, short nItemNo, bool bLinkNODE = true);

  void CreateSpecificPART(char* szName, int      iPartIDX);
  void DeleteSpecificPART(short nPartIdx, HNODE* pVIS);

  void CreatePARTS(char* szName);
  void DeletePARTS(bool  bDelEFFECT = true);

  bool CreateCHAR(char* szName, CCharMODEL* pMODEL, short nCharPartCNT, const D3DVECTOR& Position);
  void DeleteCHAR(void  ); // avatar, mob 삭제

  void SetTeamInfo(int iTeamInfo) { m_iTeamInfo = iTeamInfo; }

  //////////////////////////////////////////////////////////////////////////////////////////	
  /// < Inherited from CGameObj virtual functions
  /// <

  /*override*/
  bool IsInViewFrustum() override { return (0 != inViewfrustum( GetZMODEL() )); }
  /*override*/
  bool IsIntersect(float& fCurDistance) override { return CGameOBJ::IsIntersect( m_hNodeMODEL, fCurDistance ); }
  /*override*/
  void InsertToScene(void) override;
  /*override*/
  void RemoveFromScene(bool bIncludeEFFECT = true) override;

  /*override*/
  D3DXVECTOR3& Get_CurPOS() override;
  /// <
  /// < End
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////	
  /// < Inherited from CObjAI virtual functions
  /// <

  /*override*/
  void SetEffectedSkillFlag(bool bResult) override;
  /*override*/
  bool bCanActionActiveSkill() override;
  /*override*/
  void SetStartSkill(bool bResult) override { m_bCanStartSkill = bResult; }
  /*override*/
  bool bCanStartSkill() override { return m_bCanStartSkill; } /// 서버로 부터 캐스팅 스타트 패킷을 받았는가?

  /*override*/
  int Get_MaxHP() override { return 1; }
  /*override*/
  int Get_MaxMP() override { return 1; }

  /*override*/
  int Get_HP() override { return m_iHP; }
  /*override*/
  int Get_MP() override { return m_iMP; }

  /*override*/
  void Set_HP(int iHP) override;
  /*override*/
  void Set_MP(int iMP) override;

  /*override*/
  bool SetCMD_ATTACK(int iServerTarget) override { return CObjAI::SetCMD_ATTACK( iServerTarget ); }

  /*override*/
  bool IsInRANGE(CObjCHAR* pT, int iRange) override { return CD3DUtil::distance( (int)m_PosCUR.x, (int)m_PosCUR.y, (int)pT->m_PosCUR.x, (int)pT->m_PosCUR.y ) <= iRange; }

  /*override*/
  void Adj_MoveSPEED(WORD wSrvDIST, const D3DVECTOR& PosGOTO) override;
  /*override*/
  void Adj_AniSPEED(float fAniSpeed) override;
  /*override*/
  int Get_AttackRange() override;
  /*override*/
  void Set_MOTION(short nActionIdx, float fMoveSpeed = 0, float fAniSpeed = 1.0f, bool bAttackMotion = false, int iRepeatCnt = 0) override;
  /*override*/
  void Move_COMPLETED() override;

  /*override*/
  void Reset_Position() override;

  ///
  /// concerned command
  ///
  /*override*/
  virtual void SetCMD_ATTACK(int iServerTarget, WORD wSrvDIST, const D3DVECTOR& PosGOTO);
  /*override*/
  bool Attack_START(CObjCHAR* pTarget) override;
  /*override*/
  bool Skill_START(CObjCHAR* pTarget) override;
  /*override*/
  bool Casting_START(CObjCHAR* pTarget) override;
  /*override*/
  void Casting_END() override;
  /*override*/
  void Attack_END(bool bStopTrail = true) override;
  /*override*/
  void MoveStart() override;

  ///*override*/void			SetCMD_SIT (void);
  ///*override*/void			SetCMD_STAND (void);
  ///*override*/void			SetCMD_MOVE (tPOINTF &PosFROM, tPOINTF &PosTO, int iServerTarget);
  ///*override*/void			SetCMD_DIE ();
  ///*override*/bool			SetCMD_TOGGLE (BYTE btTYPE);

  //
  ///*override*/void			SetCMD_STOP ();
  ///*override*/void			SetCMD_MOVE (float fPosX, float fPosY, BYTE btRunMODE);
  ///*override*/void			SetCMD_RUNnATTACK(int iTargetObjTAG)

  ///*override*/void			SetCMD_Skill2SELF (short nSkillIDX);
  ///*override*/bool			SetCMD_Skill2OBJ (int iServerTarget, short nSkillIDX);
  ///*override*/void			SetCMD_Skill2POS (tPOINTF &PosGOTO, short nSkillIDX);

  /// 모델을 타겟 위치를 바라보도록 회전.
  /*override*/
  void Set_ModelDIR(t_POSITION& PosToView, bool bImmediate = false) override;
  /*override*/
  void Set_ModelDIR(float fAngleDegree, bool bImmediate = false) override;

  /// 모델의 이동 속도를 변경한다.
  /*override*/
  void Set_ModelSPEED(float fMoveSpeed) override {
    m_fModelSPEED = fMoveSpeed;
    setModelMoveVelocity( m_hNodeMODEL, m_fModelSPEED );
  }

  /// 모델의 이동 속도를 얻어온다.
  /*override*/
  float Get_ModelSPEED() override {
    return m_fModelSPEED;
  }

  /*
  각 계층간의 오브젝트 타입들은 Get_WalkSPEED(), GetOri_RunSPEED() 의 구현을 통해서 이동속도를 구한다.!!
  결국 Get_WalkSPEED(), GetOri_RunSPEED() 의 상속구현에 신경써라..
  */
  /*override*/
  float Get_MoveSPEED() override;
  float Get_DefaultSPEED() override;

  /*override*/
  virtual short Get_nAttackSPEED();
  /*override*/
  float Get_fAttackSPEED() override;

  /*override*/
  DWORD GetIngDurationStateFLAG() override { return m_EndurancePack.GetStateFlag(); }; /// 현재 지속 상태 플레그값을 얻는다.
  /*override*/
  short GetIngDurationStateSKILL(eING_TYPE eTYPE) override { return 0; }; /// 현재 지속 상태에 적용된 스킬번호

  /// RECOVER_STATE_CHECK_TIME 간격으로 Get_RecoverHP에서 구한 HP, Get_ReocverMP에서 구한 MP를 더해 준다.
  /*override*/
  void RecoverHP(short nRecoverMODE) override;
  /*override*/
  void RecoverMP(short nRecoverMODE) override;

  /*override*/
  void Do_DeadEvent(CObjCHAR* pTarget) override {};

  /*override*/
  void SetMotionRepeatCount(int iRepeatCount) override { setRepeatCount( GetZMODEL(), iRepeatCount ); };

protected:
  CChangeActionMode m_ChangeActionMode;

public:
  /*override*/
  void ChangeActionMode(int iActionMode) override { return m_ChangeActionMode.ChangeActionMode( iActionMode ); }

  /*override*/
  int GetPetMode() override { return -1; }

  /// <
  /// < End
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////	
  /// < Inherited from CAI_OBJ virtual functions
  /// <

  /*override*/
  int Get_TeamNO() override { return m_iTeamInfo; } /// 서버에서 받은 팀번호
  /*override*/
  BYTE Is_DAY() override { return g_DayNNightProc.IsDay(); } /// 존 시간을 체크 저녁시간보다 작은가?
  /*override*/
  int Get_ZoneTIME() override { return g_DayNNightProc.GetZoneTime(); } /// 존 시간을 체크 저녁시간보다 작은가?

  /*override*/
  float Get_CurXPOS() override { return m_PosCUR.x; }
  /*override*/
  float Get_CurYPOS() override { return m_PosCUR.y; }
  /*override*/
  float Get_CurZPOS() override { return m_PosCUR.z; }
  /*override*/
  float Get_BornXPOS() override { return m_PosBORN.x; }
  /*override*/
  float Get_BornYPOS() override { return m_PosBORN.y; }

  /*override*/
  int Get_RANDOM(int iMod) override { return (rand() % iMod); }
  /*override*/
  float Get_MoveDISTANCE() override { return CD3DUtil::distance( m_PosBORN, m_PosCUR ); }
  /*override*/
  float Get_DISTANCE(CAI_OBJ* pObj) override { return CD3DUtil::distance( m_PosCUR, ((CObjCHAR*)pObj)->m_PosCUR ); }
  /*override*/
  void Special_ATTACK() override;
  /*override*/
  void Set_EMOTION(short nEmotionIDX) override;

  /// <
  /// < End
  //////////////////////////////////////////////////////////////////////////////////////////

  /// 모든 내부의 리스트 정리
  void ClearAllEntityList();

  /////////////////////////////////////////////////////////////////////////////////
  /// 외부에서 붙여진 이펙트들..
  classDLLIST<CEffect*> m_ExternalEffectLIST;

  classDLLIST<CEffect*> m_WeatherEffectLIST;
public:
  void AddExternalEffect(CEffect*    pEffect);
  void DeleteExternalEffect(CEffect* pEffect);

  void ClearExternalEffect();

  /// 날씨
  void AddWeatherEffect(CEffect* pEffect);
  void DeleteWeatherEffect();

  /////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////
  /// Skill result effect 등록리스트
private:

  struct stEFFECT_OF_SKILL {
    DWORD m_dwCreateTime;
    int   iSkillIDX;
    bool  bDamageOfSkill;
    int   iCasterINT; /// 시전자의 INT 계산식에 필요

    gsv_DAMAGE_OF_SKILL EffectOfSkill; /// damage_of_skill 이 effect_of_skill 을 상속받았으므로 편이를 위해서
  };

  std::vector<stEFFECT_OF_SKILL> m_EffectedSkillList;
  int                            m_iEffectedSkillIndex;
  bool                           m_bProcEffectedSkill;
  bool                           m_bCanStartSkill;

public:
  /*void								SetEffectedSkillIndex( int iEffectedSkillIdx )
  { 
    m_iEffectedSkillIndex = iEffectedSkillIdx; 
    m_bProcEffectedSkill = true;
  }*/
  void PushEffectedSkillToList(int iSkillIDX, gsv_DAMAGE_OF_SKILL EffectedSkill, int iCasterINT, bool bDamageOfSkill = false);
  void ProcTimeOutEffectedSkill();
  bool ProcEffectedSkill();
  void ProcOneEffectedSkill(stEFFECT_OF_SKILL* pEffectOfSkill);

  void ProcEffectOfSkillInDamageOfSkill(int iSkillIDX, int iObjIDX, CObjCHAR* pChar, stEFFECT_OF_SKILL* pEffectOfSkill);
  void ApplyEffectOfSkill(int               iSkillIDX, int iObjIDX, CObjCHAR* pChar, stEFFECT_OF_SKILL* pEffectOfSkill);
  /////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////
  /// Damage 등록리스트
private:
  struct stAPPLY_DAMAGE {
    DWORD     m_dwCreateTime;
    int       m_iAttacker;
    uniDAMAGE m_Damage;
  };

  std::vector<stAPPLY_DAMAGE> m_DamageList;

public:
  void PushDamageToList(int iAttacker, uniDAMAGE wDamage);

  uniDAMAGE PopCurrentAttackerDamage(int    iAttacker);
  uniDAMAGE PopTotalDamageFromList(int64_t& iMaxDamage);

  /// 모든 데이지 처리( 죽거나.. 뭐 그런 상황에서.. 정리 )
  void ClearAllDamage();

  /// 시간제한 처리
  void ProcDamageTimeOut();

  /// 타격치 바로 표시
  void CreateImmediateDigitEffect(int64_t wDamage);

  /// Damage_of_Skill => 일반 데미지로 전환저장..
  void ConvertDamageOfSkillToDamage(gsv_DAMAGE_OF_SKILL stDamageOfSkill);
  /////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////
  /// 필드아이템 드랍 등록리스트( 혹시 한번에 여러개 떨어뜨릴지 몰라서 벡터로서 )
private:
  std::vector<tag_DROPITEM> m_FieldItemList;

public:
  void PushFieldItemToList(tag_DROPITEM& wDamage) { m_FieldItemList.push_back( wDamage ); }
  void DropFieldItemFromList();
  /////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////
  /// 지속성 Entity 등록
public:
  CEndurancePack m_EndurancePack; /// 초기화 리스트 이용 꼭..

public:
  bool AddEnduranceEntity(int iEntityIDX, int iStatusSTBNO, int iEnduranceTime, int iEntityType) {
    return m_EndurancePack.AddEntity( iEntityIDX, iStatusSTBNO, iEnduranceTime, iEntityType );
  }

  int GetEnduranceEntityCount() { return m_EndurancePack.GetEntityCount(); }

  int                   GetEnduranceStateSTBNO(int Index) {
    CEnduranceProperty* pEntity = m_EndurancePack.GetEntity( Index );
    if ( pEntity == nullptr )
      return 0;

    return pEntity->GetStateSTBNO();
  }

  /// 유리상태 해지, 불리상태 해지 등..
  int ProcFlushStateSkill(int iStateTableIndex) { return m_EndurancePack.ProcFlushStateSkill( iStateTableIndex ); }
  /////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////
  /// 몹 소환 관련
  /// 액션 프레임에 맞추가 위해 저장해 둔다.
private:
  gsv_MOB_CHAR m_SummonMob;
  bool         m_bHaveSummonedMob; /// 소환해야될 몹이 있는가?

public:
  void SetSummonMobInfo(gsv_MOB_CHAR& MobInfo);
  void SummonMob();

  /////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// queuing the command 
  //////////////////////////////////////////////////////////////////////////////////////////////////

  CObjCommandManager m_CommandQueue;

  /// 현재 명령을 적용할수 있는가?
  /*override*/
  bool CanApplyCommand() override;
  void ProcQueuedCommand();

  void ClearAllCommand() { m_CommandQueue.ClearCommand(); }

  /*override*/
  void PushCommandSit() override;
  /*override*/
  void PushCommandStand() override;
  /*override*/
  void PushCommandStop() override;
  /*override*/
  void PushCommandMove(const D3DVECTOR& PosTo, BYTE btRunMODE) override;
  /*override*/
  void PushCommandMove(WORD wSrvDIST, const D3DVECTOR& PosTO, int iServerTarget) override;
  /*override*/
  void PushCommandAttack(int iServerTarget, WORD wSrvDIST, const D3DVECTOR& PosTO) override;
  /*override*/
  void PushCommandDie() override;
  /*override*/
  void PushCommandToggle(BYTE btTYPE) override;
  /*override*/
  void PushCommandSkill2Self(short nSkillIDX) override;
  /*override*/
  void PushCommandSkill2Obj(WORD wSrvDIST, const D3DVECTOR& PosTO, int iServerTarget, short nSkillIDX) override;
  /*override*/
  void PushCommandSkill2Pos(const D3DVECTOR& PosGOTO, short nSkillIDX) override;

  //////////////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// Object Vibration
  //////////////////////////////////////////////////////////////////////////////////////////////////
public:
  CObjvibration m_ObjVibration;

public:
  void StartVibration();

  //////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef	__VIRTUAL_SERVER
  CRegenPOINT					*m_pRegenPOINT;
  classDLLNODE< CObjCHAR* >	*m_pRegenListNODE;
#endif

  //////////////////////////////////////////////////////////////////////////////////////////	
  /// > CObjCHAR virtual functions
  /// >

  // default npc skeleton
  virtual HNODE Get_SKELETON() { return m_pCharMODEL->GetZSKEL(); }

  virtual bool Check_EVENT(CGameOBJ* pSourOBJ, short nEventIDX = -1) { return false; }

  virtual bool  IsFemale() =0;
  virtual short IsMagicDAMAGE() =0;

  virtual tagMOTION* Get_MOTION(short nActionIdx) =0 { *(int*)nullptr = 10; };

  virtual int Add_HP(int iAdd);
  virtual int Add_MP(int iAdd) { return (m_iMP += iAdd); }

  virtual int Sub_HP(int iSub);
  virtual int Sub_MP(int iSub) { return (m_iMP -= iSub); }

  virtual void  Add_EXP(short nExp) =0 { *(int*)nullptr = 10; };
  virtual short GetOri_WalkSPEED() =0 { *(int*)nullptr  = 10; };
  virtual short GetOri_ATKSPEED() =0 { *(int*)nullptr   = 10; };
  virtual short GetOri_RunSPEED() =0 { *(int*)nullptr   = 10; };

  /// 아바타의 경우 현재 소모중인 총알에 영향을 받는다.
  virtual int Get_BulletNO();

  /// 특정 좌표로 이동하여 높이 보정. 높이는 현재 좌표 위에 놓인 오브젝트 중 가장 높은 위치
  virtual void DropFromSky(float fX, float fY);

  virtual int Get_INT() { return 0; } /// !!!Cal 클래스에서 CObjCHAR 의 맴버로 접근하는부분이 있어 이것만..

  /// 지속형의 변경수치 적용을 위해서 현재 적용되어있는 능력수치( 패시브 스킬 포함 )
  virtual int Get_DefaultAbilityValue(int iType) { return 1; };

  ///
  /// 루프를 돌리는 상태의 경우라도 이 플래그의 상태에따라 모션 리셋( STOP, MOVE... )
  ///
  virtual void SetUpdateMotionFlag(bool bUpdate) {}
  virtual bool GetUpdateMotionFlag() { return false; }

  ///
  /// 내가 누군가에게 링크가 되었는가?
  ///
  virtual bool IsChild() { return false; }

  //----------------------------------------------------------------------------------------------------	
  /// @brief 각 오브젝트 타입마다 추가 처리사항들이 조금씩 있다.
  //----------------------------------------------------------------------------------------------------
  int Proc(void) override;

  //----------------------------------------------------------------------------------------------------	
  /// @brief 캐릭터 위에 이펙트를 붙인다.
  //----------------------------------------------------------------------------------------------------
  virtual void ShowEffectOnCharByIndex(int iEffectIDX, int iSoundIDX = 0, bool bWeatherEffect = false);
  virtual void ShowEffectOnCharByHash(int  iHash, int      iSoundIDX = 0, bool bWeatherEffect = false);

  /// >
  /// > End
  //////////////////////////////////////////////////////////////////////////////////////////

  /// 모델을 타겟 위치를 바라보도록 회전.	
  void Add_ModelDIR(float fAngleDegree);

  D3DXVECTOR3 Get_GotoPOSITION() { return m_PosGOTO; }
  D3DXVECTOR3 Get_BornPOSITION() { return m_PosBORN; }

  float Get_DISTANCE(t_POSITION PosBASE) { return CD3DUtil::distance( m_PosCUR, PosBASE ); }

public:
  /// not overrided function
  int Get_RWeapon() { return Get_R_WEAPON(); }

          CObjCHAR();
  virtual ~CObjCHAR();

  HNODE              GetZMODEL() { return m_hNodeMODEL; }
  CMODEL<CCharPART>* GetPartMODEL(short nPartIDX) { return m_pCharMODEL->GetCharPART( nPartIDX ); }
  int                Get_CharNO() override { return m_nCharIdx; }

  bool LinkDummy(HNODE      hLinkNODE, short nDummyIDX);
  bool Link2LastDummy(HNODE hLinkNODE);
  int  GetLastDummyIDX() { return m_iLastDummyIDX; }

  bool IsIntersect(HNODE hModel) { return (intersectNode( m_hNodeMODEL, hModel ) != 0); }

  void  SetSCALE(float fScale) { m_fScale = fScale, setScale( m_hNodeMODEL, fScale, fScale, fScale ); }
  float Get_SCALE() override { return m_fScale; }

  void Rotate(D3DXQUATERNION& Rotate) { setQuaternion( m_hNodeMODEL, Rotate.w, Rotate.x, Rotate.y, Rotate.z ); }
  /// NPC의 경우 STB에 강제로 높이가 들어가 있으면 그 높이를 사용한다.
  virtual void GetScreenPOS(D3DVECTOR& PosSCR);

  bool LinkEffectToPOINT(CEffect* pEffect, short nPartIDX, short nPointIDX);
  bool LinkEffectToDUMMY(CEffect* pEffect, short nDummyIDX);

  void Add_EFFECT(short nPartIDX, short nPointIDX, t_HASHKEY HashEffectFILE);
  void Del_EFFECT(short nPartIDX);

  void Warp_POSITION(float fX, float fY);

  void Dead();
  bool ProcessSkillHit(CObjCHAR* pFromOBJ);
  bool Hitted(CObjCHAR*          pFromOBJ, int iEffectIDX, int iSkillIDX = 0, bool bIsSkillEffect = false, bool bIsBulletHit = false, bool bJustEffect = false);

  void Set_UserMOITON(t_HASHKEY  HashMOTION, float fMoveSpeed, int iRepeatCnt = 0);
  void Set_UserMOITON(tagMOTION* pMotion, float    fMoveSpeed, int iRepeatCnt = 0);

  void  SetCollisionGroundNode(HNODE hNodeGroundIn) { m_hNodeGround = hNodeGroundIn; }
  HNODE GetCollisionGroundNode() { return m_hNodeGround; } // 바닥에 위치한 오브젝트의 엔진 핸들을 리턴함. 지형이나, 물 등은 제외된다. 없으면 0.

public:
  tagDAMAGE m_DeadDAMAGE;
  long      m_lDeadTIME;
  void      Apply_DAMAGE(CObjCHAR* pAtkOBJ, int64_t wDamage);

  //----------------------------------------------------------------------------------------------------
  /// @brief 특정 액션 중간에 장비를 교체했을시( 특히 무기 ) 바뀐 장비에 맞는 모션으로 교체
  //----------------------------------------------------------------------------------------------------
  void UpdateMotionByChangingEquip();

  //----------------------------------------------------------------------------------------------------
  /// @brief 내가 팻일 경우의 무기발사
  //----------------------------------------------------------------------------------------------------
  void FireBulletWithPetWeapon(CObjCHAR* pTarget);

private:
  int m_iLastCastingSkill;
public:
  void SetLastCastingSkill(int iSkillIDX) { m_iLastCastingSkill = iSkillIDX; }
  int  GetLastCastingSkill() { return m_iLastCastingSkill; }

  //----------------------------------------------------------------------------------------------------
  /// @brief 클릭할수 있는 객체인가?
  //----------------------------------------------------------------------------------------------------
  virtual bool CanClickable() { return true; }

private:
  WORD        m_dwClanID;
  WORD        m_wClanMarkBack;
  WORD        m_wClanMarkCenter;
  int         m_iClanLevel;
  std::string m_strClanName;
  BYTE        m_btClanPos;

  CClanMarkUserDefined* m_ClanMarkUserDefined;
public:
  void        SetClan(DWORD dwClanID, WORD wMarkBack, WORD wMarkCenter, const char* pszName, int iLevel, BYTE btClanPos);
  DWORD       GetClanID();
  WORD        GetClanMarkBack();
  WORD        GetClanMarkCenter();
  const char* GetClanName();
  BYTE        GetClanPos();
  void        SetClanPos(BYTE btPos);
  void        ResetClan();
  void        SetClanLevel(int iLevel);
  int         GetClanLevel();
  void        SetClanMark(WORD wMarkBack, WORD wMarkCenter);

  CClanMarkUserDefined* GetUserDefinedClanMark() { return m_ClanMarkUserDefined; }
  void                  SetUserDefinedClanMark(CClanMarkUserDefined* pUserDefinedClanMark); //{ m_ClanMarkUserDefined = pUserDefinedClanMark; }

  /// 서버와의 HP/MP 차이를 보정하기 위한 변수/함수
protected:
  int m_ReviseHP;
  int m_ReviseMP;
  int m_AruaAddMoveSpeed; /// 아루아 여신상태 일경우 증가되는 이동속도

public:
  void SetReviseHP(int hp);
  void SetReviseMP(int mp);

  void SHOW() { setVisibilityRecursive( GetZMODEL(),TRUE ); }
  void HIDE() { setVisibilityRecursive( GetZMODEL(),FALSE ); }

  //-----------------------------------------------------------------------------------
  //조성현
  bool m_bDisguise;

  //------------------------------------------------------------------------------------	
  //박지호
  //카트 변수들...
  CObjCART* m_pObjCART;  //카트 클래서 
  CObjCHAR* m_pRideUser; //2인승 플레이어 

  int   m_iPetType;
  short m_skCartIDX; //카트 스킬 이펙트 (현재 사용무)
  WORD  m_iRideIDX;  //2인승 유저 인덱스 

  float m_saveSpeed; //스피트 저장 변수 

  BOOL m_bUseCartSkill;
  BOOL m_IsRideUser; //2인승 탑승유무 

  //아로아 여신상태 
  BOOL m_IsAroa;   //아로아 설정 유무 
  BOOL m_IsCartVA; //카트 바이브레이션 (흔들기)

  void ChangeHittedEffect(CObjCHAR*    pObj, BOOL bA, int& hitIDX);
  void SetNewCommandAfterSkill_PET(int iSkillNO) override;
  void Stop_Cart(void                  );
  void Ride_Cansel_Motion(void         );

  void ReleaseRideUser(void );
  void ReleaseRideUser(WORD irideIDX);
  void Set_SitMode(void     );

  int ProcCMD_Skill2OBJECT_PET() override;

  BOOL SetRideUser(WORD irideIDX = 0);

  short& GetCartSKIDX(void) { return m_skCartIDX; }

  BOOL& GetUseCartSkill(void) { return m_bUseCartSkill; }

  //내가 상대의 카트에 탑승 했는지 체크(TRUE: 탑승, FALSE: 아님)
  BOOL& IsRideUser(void) { return m_IsRideUser; }

  //나의 카트에 상대방을 탑승 시켰는지 체크(0: 아님, 숫자: 탑승 시킴)
  WORD GetRideUserIndex(void) { return m_iRideIDX; }

  //아로아 여신 상태설정 함수 
  BOOL& SetAroaState(void) { return m_IsAroa; }

  BOOL& SetCartVA(void) { return m_IsCartVA; }

  float& SaveSpeed(void) { return m_saveSpeed; }
  //------------------------------------------------------------------------------------

  /// 최종진 : 아루아 여신 상태일경우의 추가 능력치 계산 함수
  virtual void Calc_AruaAddAbility();
};

///
///	MOB class inherited from COjbCHAR
///

class CObjMOB : public CObjCHAR {
private:
  short m_nQuestIDX;
  int   m_iMobAniSkill;

public :
  static DWORD m_dwCreateOrder;

          CObjMOB();
  virtual ~CObjMOB();

  //////////////////////////////////////////////////////////////////////////////////////////	
  /// < Inherited from CGameObj virtual functions
  /// <

  /*override*/
  int Get_TYPE() override { return OBJ_MOB; }
  /*override*/
  char* Get_NAME() override { return (char*)NPC_NAME( m_nCharIdx ); }

  /// <
  /// < End
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////	
  /// < Inherited from CAI_OBJ virtual functions
  /// <

  /*override*/
  void Say_MESSAGE(char* szMessage) override;
  /*override*/
  bool Change_CHAR(int nCharIDX) override;
  /*override*/
  virtual bool Create_PET(int nCharIDX);
  /*override*/
  void Run_AWAY(int iDistance) override;

  /*override*/
  int Get_LEVEL() override { return NPC_LEVEL( m_nCharIdx ); }
  /*override*/
  int Get_ATK() override { return NPC_ATK ( m_nCharIdx ); }
  /*override*/
  int Get_DEF() override { return NPC_DEF ( m_nCharIdx ); }
  /*override*/
  int Get_RES() override { return NPC_RES ( m_nCharIdx ); }
  /*override*/
  int Get_HIT() override { return NPC_HIT ( m_nCharIdx ); }
  /*override*/
  int Get_CHARM() override { return 0; }
  /*override*/
  int Get_AVOID() override { return NPC_AVOID( m_nCharIdx ); }
  /*override*/
  int Get_SENSE() override { return Get_LEVEL(); } // LEVEL로 대체 !
  /*override*/
  int Get_GiveEXP() override { return NPC_GIVE_EXP( m_nCharIdx ); }
  /*override*/
  void Add_EXP(short nExp) override { }
  /*override*/
  int Get_CRITICAL() override { return (int)(this->Get_LEVEL() * 0.8); }

  /// <
  /// < End
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////	
  /// < Inherited from CObjAI virtual functions
  /// <

  /*override*/
  int Def_AttackRange() override { return NPC_ATK_RANGE( m_nCharIdx ); }

  /*override*/
  int GetANI_Stop() override { return MOB_ANI_STOP; }
  /*override*/
  int GetANI_Move() override { return (m_bRunMODE) ? MOB_ANI_RUN : MOB_ANI_MOVE; }
  /*override*/
  int GetANI_Attack() override { return MOB_ANI_ATTACK; }
  /*override*/
  int GetANI_Die() override {
    if ( m_bStopDead )
      return MOB_ANI_STOP;
    return MOB_ANI_DIE;
  }

  /*override*/
  int GetANI_Hit() override { return MOB_ANI_HIT; }
  /*override*/
  int GetANI_Casting() override { return m_iMobAniSkill; } /// 캐스팅동작
  /*override*/
  int GetANI_CastingRepeat() override { return m_iMobAniSkill; } /// 루프동작은 캐스팅동작으로 사용..
  /*override*/
  int GetANI_Skill() override { return m_iMobAniSkill + 1; }

  /*override*/
  int Get_MP() override { return 32767; }
  /*override*/
  int Get_R_WEAPON() override { return NPC_R_WEAPON( m_nCharIdx ); }
  /*override*/
  int Get_L_WEAPON() override { return 0; }
  /*override*/
  virtual void RecoverHPnMP(short nRecoverMODE) {};

  /// 최대 생명력	
  void Set_MaxHP(int iMaxHP) { m_iMaxHP = iMaxHP; }
  /*override*/
  int Get_MaxHP() override { return m_iMaxHP; }
  /// 최대 마나
  /*override*/
  int Get_MaxMP() override { return 100; }

  /*override*/
  int Get_AttackRange() override;

  /*override*/
  void Do_StopAI(void) override { g_AI_LIST.AI_Stop( NPC_AI_TYPE(m_nCharIdx), this ); }
  /*override*/
  void Do_AttackMoveAI(CObjCHAR* pTarget) override { g_AI_LIST.AI_AttackMove( NPC_AI_TYPE(m_nCharIdx), this, pTarget ); }
  /*override*/
  void Do_DamagedAI(CObjCHAR* pTarget, int iDamage) override {
    if ( !(CMD_BIT_INT & this->Get_COMMAND()) )
      g_AI_LIST.AI_Damaged( NPC_AI_TYPE(m_nCharIdx), this, pTarget, iDamage );
  }

  /*override*/
  void Do_KillAI(CObjCHAR* pTarget, int iDamage) override { g_AI_LIST.AI_Kill( NPC_AI_TYPE( m_nCharIdx ), this, pTarget, iDamage ); }
  /*override*/
  void Do_DeadAI(CObjCHAR* pTarget, int iDamage) override { g_AI_LIST.AI_Dead( NPC_AI_TYPE( m_nCharIdx ), this, pTarget, iDamage ); }

  /*override*/
  void Do_DeadEvent(CObjCHAR* pAtk) override;

  /*override*/
  float Get_DefaultSPEED() override {
    if ( !m_bRunMODE ) return GetOri_WalkSPEED();

    short nR = (GetOri_RunSPEED() + m_EndurancePack.GetStateValue( ING_INC_MOV_SPD )
                - m_EndurancePack.GetStateValue( ING_DEC_MOV_SPD ));
    return nR;
  }

  /*override*/
  short Get_nAttackSPEED() override {
    int iR = GetOri_ATKSPEED() + m_EndurancePack.GetStateValue( ING_INC_ATK_SPD )
             - m_EndurancePack.GetStateValue( ING_DEC_ATK_SPD );
    return iR;
  }

  /*override*/
  float Get_fAttackSPEED() override {
    int iR = GetOri_ATKSPEED() + m_EndurancePack.GetStateValue( ING_INC_ATK_SPD )
             - m_EndurancePack.GetStateValue( ING_DEC_ATK_SPD );
    return iR / 100.0f;
  }

  /// <
  /// < End
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////	
  /// < Inherited from CObjCHAR virtual functions
  /// <

  /*override*/
  bool Check_EVENT(CGameOBJ* pSourOBJ, short nEventIDX) override;

  /*override*/
  bool IsFemale() override { return false; }
  /*override*/
  short IsMagicDAMAGE() override { return NPC_IS_MAGIC_DAMAGE( m_nCharIdx ); }

  /*override*/
  tagMOTION* Get_MOTION(short nActionIdx = MOB_ANI_STOP) override { return m_pCharMODEL->GetMOTION( nActionIdx ); }

  /*override*/
  short GetOri_WalkSPEED() override { return NPC_WALK_SPEED( m_nCharIdx ); }
  /*override*/
  short GetOri_RunSPEED() override { return NPC_RUN_SPEED( m_nCharIdx ); }
  /*override*/
  short GetOri_ATKSPEED() override { return NPC_ATK_SPEED( m_nCharIdx ); }

  /// 지속형의 변경수치 적용을 위해서 현재 적용되어있는 능력수치( 패시브 스킬 포함 )
  int Get_DefaultAbilityValue(int iType) override;

  /// NPC의 경우 STB에 강제로 높이가 들어가 있으면 그 높이를 사용한다.
  void GetScreenPOS(D3DVECTOR& PosSCR) override;

  /// <
  /// < End
  //////////////////////////////////////////////////////////////////////////////////////////

  bool Create(short nCharIdx, const D3DVECTOR& Position, short nQuestIDX, bool bRunMODE);

  void SetMobAniSkill(int iMobAniSkill = MOB_ANI_CASTION01) { m_iMobAniSkill = iMobAniSkill; }

  //------------------------------------------------------------------------------------------------
  /// 몬스터 정지시 사운드 출력..
  //------------------------------------------------------------------------------------------------
  void PlayStopSound();

  int Proc() override;

};

///
///	Npc class inherited from COjbMOB
///

class CObjNPC : public CObjMOB {
private:
  int m_iEventSTATUS;

public :

  //////////////////////////////////////////////////////////////////////////////////////////	
  /// < Inherited from CGameObj virtual functions
  /// <

  CObjNPC();

  int Get_TYPE() override { return OBJ_NPC; }
  int Proc() override;
  /// <
  /// < end	
  //////////////////////////////////////////////////////////////////////////////////////////	

  void SetEventValue(int iEventValue);
  int  GetEventValue();

  //-------------------------------------------------------------------------------------------------
  // 05.05.19 icarus:: 와우 스타일의 NPC 퀘스트 노출 작업...
public :
  // 서버에서 npc 추가 패킷을 받고 생성시 기본값 -1, 
  short m_nQuestSignal;
  //	int				Proc (void);

};

class CObjCART;

///
///	Avata class
///

class CObjAVT : public CObjCHAR {
protected:
  CCharMODEL m_CharMODEL;

  /// 캐릭터 표정
  int m_iExpression;

  /// 캐릭터 파츠
  union {
    tagPartITEM m_sPartItemIDX[ MAX_BODY_PART ];

    // short	m_nPartItemIDX[ MAX_BODY_PART ];
    struct {
      // 순서는 t_CharPART에 따라서...
      tagPartITEM m_sFaceIDX;   //short m_nFaceIDX;
      tagPartITEM m_sHairIDX;   //short m_nHairIDX;
      tagPartITEM m_sHelmetIDX; //short m_nHelmetIDX;

      tagPartITEM m_sArmorIDX;    //short m_nArmorIDX;
      tagPartITEM m_sGauntletIDX; //short m_nGauntletIDX;
      tagPartITEM m_sBootsIDX;    //short m_nBootsIDX;

      tagPartITEM m_sGoggleIDX;   //short m_nGoggleIDX;
      tagPartITEM m_sKnapsackIDX; //short m_nKnapsackIDX;

      tagPartITEM m_sRWeaponIDX; //short m_nRWeaponIDX;
      tagPartITEM m_sLWeaponIDX; //short m_nLWeaponIDX;
    };
  };

protected:
  std::string m_Name;
  int         m_iLevel;

  DWORD m_dwSitTIME;
  short m_nPsvAtkSPEED;

  short m_nRunSPEED;
  short m_nAtkAniSPEED;

  /// 교환할 무기가 세팅되면.. 모션이 끝난후에 바꾼다..
  int  m_iDoChangeWeaponR;
  int  m_iDoChangeWeaponL;
  bool m_bUpdateMotion;

  /// 현재 사용중인 총알 데이터..
  tagShotDATA m_ShotData[MAX_SHOT_TYPE];
  int         m_iCon;
  int         m_iRecoverHP;
  int         m_iRecoverMP;

  ///아이템 소지 무게비율: 
  BYTE m_btWeightRate;

  /// 스테미나			
  short m_nStamina;

public:
          CObjAVT();
  virtual ~CObjAVT();

  void SetShotData(int i, int iItemNo);

  void Set_CON(int          iCon) { m_iCon             = iCon; }
  void Set_AddRecoverHP(int iRecoverHP) { m_iRecoverHP = iRecoverHP; }
  void Set_AddRecoverMP(int iRecoverMP) { m_iRecoverMP = iRecoverMP; }

public :

  /// 캐릭터 표정
  void SetCharExpression(int iFaceNo) { m_iExpression = iFaceNo; }
  int  GetCharExpression() { return m_iExpression; }

  /// 교환할 무기가 세팅되면.. 모션이 끝난후에 바꾼다..
  void SetChangeWeaponR(int iWeaponNO) { m_iDoChangeWeaponR = iWeaponNO; }
  void SetChangeWeaponL(int iWeaponNO) { m_iDoChangeWeaponL = iWeaponNO; }
  int  GetChangeWeaponR() { return m_iDoChangeWeaponR; }
  int  GetChangeWeaponL() { return m_iDoChangeWeaponL; }

  /// 장비 교체시 장비에 걸려있던 속성 해제..
  void ClearRWeaponSkillEffect();
  void ClearLWeaponSkillEffect();

  ///
  /// 루프를 돌리는 상태의 경우라도 이 플래그의 상태에따라 모션 리셋( STOP, MOVE... )
  ///
  void SetUpdateMotionFlag(bool bUpdate) override { m_bUpdateMotion = bUpdate; }
  bool GetUpdateMotionFlag() override { return m_bUpdateMotion; }

  /// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  /// > virtual function of CObjAVT
  /// >

  virtual void Update_SPEED() {
    m_nAtkAniSPEED = Cal_AtkAniSPEED( this->GetPartITEM( BODY_PART_WEAPON_R ) );
    m_fRunAniSPEED = Cal_RunAniSPEED( m_nRunSPEED );
  }

  /// Virtual function
  virtual void Update(bool bUpdateBONE = false);

  virtual void Set_SEX(BYTE btRace) {
    m_btRace    = btRace;
    m_bIsFemale = btRace & 0x01;
    m_CharMODEL.Set_SEX( m_bIsFemale );
  }

  /// >
  /// > end	
  /// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  //////////////////////////////////////////////////////////////////////////////////////////	
  /// < Inherited from CGameOBJ virtual functions
  /// <

  /*override*/
  int   Get_TYPE() override { return OBJ_AVATAR; }
  char* Get_NAME() override { return (char*)m_Name.c_str(); }

  /// <
  /// < end	
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////	
  /// < Inherited from CAI_OBJ virtual functions
  /// <

  /*override*/
  bool Is_SameTYPE(int ObjTYPE) override { return (ObjTYPE == OBJ_AVATAR) || (ObjTYPE == OBJ_USER); }

  /*override*/
  int Get_LEVEL() override { return m_iLevel; }
  /*override*/
  int Get_ATK() override { return 1; }
  /*override*/
  int Get_DEF() override { return 1; }
  /*override*/
  int Get_RES() override { return 1; }
  /*override*/
  int Get_HIT() override { return 1; }
  /*override*/
  int Get_CHARM() override { return 1; }
  /*override*/
  int Get_AVOID() override { return 1; }
  /*override*/
  int Get_SENSE() override { return 1; }
  /*override*/
  int Get_GiveEXP() override { return 1; }
  /*override*/
  void Add_EXP(short nExp) override { }
  /*override*/
  int Get_CRITICAL() override { return 1; }

  /// <
  /// < end	
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////	
  /// < Inherited from CObjAI virtual functions
  /// <

  /*override*/
  int Get_CON() override { return m_iCon; } // 유저일 경우 return pAVATAR->GetCur_CON();
  /*override*/
  int GetAdd_RecoverHP() override { return m_iRecoverHP; } // 유저일 경우 return pAVATAR->m_btRecoverHP;
  /*override*/
  int GetAdd_RecoverMP() override { return m_iRecoverMP; } // 유저일 경우 return pAVATAR->m_btRecoverHP;

  /*override*/
  short GetPsv_ATKSPEED(float fCurSpeed, short nRightWeaponItemNo) override { return m_nPsvAtkSPEED; }
  /*override*/
  int Def_AttackRange() override { return WEAPON_ATTACK_RANGE( 0 ); }

  /*override*/
  int GetANI_Stop() override;
  /*override*/
  int GetANI_Move() override;
  /*override*/
  int GetANI_Attack() override;
  /*override*/
  int GetANI_Die() override;
  /*override*/
  int GetANI_Hit() override;
  /*override*/
  int GetANI_Casting() override;
  /*override*/
  int GetANI_CastingRepeat() override;
  /*override*/
  int GetANI_Skill() override;
  /*override*/
  int GetANI_Sitting() override;
  /*override*/
  int GetANI_Standing() override;
  /*override*/
  int GetANI_Sit() override;
  /*override*/
  int GetANI_Ride() override;
  /*override*/
  int GetANI_PickITEM() override;

  //	/*override*/int				Get_R_WEAPON ()			{	return this->m_nRWeaponIDX;				}
  //	/*override*/int				Get_L_WEAPON ()			{	return this->m_nLWeaponIDX;				}
  /*override*/
  int Get_R_WEAPON() override { return this->m_sRWeaponIDX.m_nItemNo; }
  /*override*/
  int Get_L_WEAPON() override { return this->m_sLWeaponIDX.m_nItemNo; }

  /*override*/
  int Get_MaxHP() override { return m_iMaxHP + m_EndurancePack.GetStateValue( ING_INC_MAX_HP ); }
  /*override*/
  int Get_MaxMP() override { return m_iMaxMP + m_EndurancePack.GetStateValue( ING_INC_MAX_MP ); }

  /*override*/
  bool ToggleRunMODE(float fAdjRate) override;
  /*override*/
  bool ToggleSitMODE() override;

  ///
  /// Check PET MODE
  /// if mode < 0, this char is normal mode
  /*override*/
  int GetPetMode() override;
  /*override*/
  bool CanAttackPetMode() override;
  /*override*/
  void SetCMD_PET_MOVE(const D3DVECTOR& PosTO, BYTE btRunMODE) override;
  /*override*/
  void SetCMD_PET_MOVE(WORD wSrvDIST, const D3DVECTOR& PosTO, int iServerTarget) override;
  /*override*/
  void SetCMD_PET_STOP(void) override;
  /*override*/
  void SetCMD_PET_ATTACK(int iServerTarget, WORD wSrvDIST, const D3DVECTOR& PosTO) override;

  /*override*/
  void RideCartToggle(bool bRide) override;

  /// <
  /// < end	
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////	
  /// < Inherited from CObjCHAR virtual functions
  /// <

private:
  /*override*/
  HNODE Get_SKELETON() override { return g_DATA.Get_SKELETON( this->m_bIsFemale ); }

public:
  /*override*/
  bool IsFemale() override { return m_bIsFemale; }
  /*override*/
  short IsMagicDAMAGE() override { return WEAPON_IS_MAGIC_DAMAGE( Get_R_WEAPON() ); }

  /*override*/
  tagMOTION* Get_MOTION(short nActionIdx = AVT_ANI_STOP1) override;

  /*override*/
  short GetOri_ATKSPEED() override { return m_nAtkAniSPEED; }
  /*override*/
  short GetOri_WalkSPEED() override { return WALK_CmPerSec; }
  /*override*/
  short GetOri_RunSPEED() override { return m_nRunSPEED; }
  int   GetOri_MaxHP() override;

  /*override*/
  int Get_BulletNO() override;

  /// 지속형의 변경수치 적용을 위해서 현재 적용되어있는 능력수치( 패시브 스킬 포함 )
  /*override*/
  int Get_DefaultAbilityValue(int iType) override;

  ///
  /// 내가 누군가에게 링크가 되었는가?
  ///
  /*override*/
  bool IsChild() override { return ((m_pObjCART == nullptr) ? false : true); }

  //----------------------------------------------------------------------------------------------------
  /// CObjCHAR 이 하는일말고 추가적으로 좀 할일이 있다.
  //----------------------------------------------------------------------------------------------------
  /*override*/
  int Proc() override;

  /// <
  /// < end	
  //////////////////////////////////////////////////////////////////////////////////////////

  void SetOri_RunSPEED(short nRunSpeed) { m_nRunSPEED       = nRunSpeed; }
  void SetPsv_AtkSPEED(short nPsvAtkSpeed) { m_nPsvAtkSPEED = nPsvAtkSpeed; }

  /// 최대 생명력	
  void Set_MaxHP(int iMaxHP) { m_iMaxHP = iMaxHP; }

  short GetPartITEM(short nPartIdx) { return m_sPartItemIDX[nPartIdx].m_nItemNo; }
  void  SetPartITEM(short nPartIdx, tagPartITEM& sITEM) { m_sPartItemIDX[nPartIdx] = sITEM; }

  void          SetPartITEM(short nPartIdx, tagITEM& sITEM) {
    tagPartITEM sPartITEM;
    sPartITEM.m_nItemNo      = sITEM.GetItemNO();
    sPartITEM.m_bHasSocket   = sITEM.m_bHasSocket;
    sPartITEM.m_cGrade       = sITEM.GetGrade();
    sPartITEM.m_nGEM_OP      = sITEM.GetGemNO();
    m_sPartItemIDX[nPartIdx] = sPartITEM;
  }

  void SetPartITEM(short  nPartIdx, short nItemNo);
  void SetAvataName(char* szName) { m_Name = szName; }

  void SetAllPARTS(tagPartITEM* pPartItemIDX) {
    ::ZeroMemory(m_sPartItemIDX, sizeof(tagPartITEM)*MAX_BODY_PART);
    m_sPartItemIDX[BODY_PART_FACE]                         = pPartItemIDX[0];
    m_sPartItemIDX[BODY_PART_HAIR]                         = pPartItemIDX[1];
    short     nEquipSlot                                   = 0;
    for ( int i                                            = 2; i < MAX_BODY_PART; ++i ) {
      nEquipSlot                                           = CInventory::GetBodyPartToEquipSlot( i ) + 1;
      if ( nEquipSlot != MAX_EQUIP_IDX ) m_sPartItemIDX[i] = pPartItemIDX[nEquipSlot];
    }
  }

  void SetAllPetPARTS(tagPartITEM* pPetPartItemIDX) { ::CopyMemory( m_sPetPartItemIDX, pPetPartItemIDX, sizeof(tagPartITEM)*MAX_RIDING_PART); }

  void SetAvtLevel(int iLevel) { m_iLevel = iLevel; }

  bool Create(const D3DVECTOR& Position, BYTE btCharRACE);

  //----------------------------------------------------------------------------------------------------		
  /// @brief 재밍, 재련관련 이펙트..
  //----------------------------------------------------------------------------------------------------
  void CreateGemmingEffect();
  void DeleteGemmingEffect();

  //--------------------------------------------------------------------------------------------------------
  //2005. 06.15 박 지호 
  void Set_RareITEM_Glow(void);

  void CreateGradeEffect();
  void DeleteGreadEffect();
  void Process_JOIN_RIDEUSER(void);
  //--------------------------------------------------------------------------------------------------------

  //----------------------------------------------------------------------------------------------------	
  /// 현재 좌표(m_PosCUR.x, m_PosCUR.y)의 높이를 리턴
  /// @pReachable 도달 가능한 위치인지의 여부. 이전 프레임의 위치로부터 도달가능하지 않다면, false 값이 세팅됨.
  /// @return 높이값. 도달 불가능한 위치라면, 가장 높은 오브젝트의 위치를 리턴.
  //----------------------------------------------------------------------------------------------------
  float GetHeight(bool* pReachable = nullptr);

  //////////////////////////////////////////////////////////////////////////////////////////
  ///
  /// 개인상점 관련 데이터
  ///
  //////////////////////////////////////////////////////////////////////////////////////////
private:
  bool        m_bPersonalStoreMode;
  std::string m_strPersonalStoreTitle;

public:
  void        SetPersonalStoreTitle(char* strTitle, int iPersonalStoreType = 0);
  const char* GetPersonalStoreTitle();
  bool        IsPersonalStoreMode();

  CObjFIXED* m_pObjPersonalStore;

  //////////////////////////////////////////////////////////////////////////////////////////
  ///
  /// 팻 시스템 관련 데이터..
  ///
  //////////////////////////////////////////////////////////////////////////////////////////

public:

  friend CObjCART;
  /// 팻 파츠..
  /// 물론.. CObjPET 내부에 이 데이터들은 존재한다.. 여기서일단 편의를 위해 관리해준다.

  //박지호::카트파츠 수정 
#if defined(_GBC)
  union {
    //short				m_nPetPartItemIDX[ MAX_RIDING_PART ];
    tagPartITEM m_sPetPartItemIDX[ MAX_RIDING_PART ];

    struct {
      // 순서는 t_CharPART에 따라서...
      tagPartITEM m_sEngineIDX; // short			m_nEngineIDX;
      tagPartITEM m_sBodyIDX;   // short			m_nBodyIDX;
      tagPartITEM m_sLegIDX;    // short			m_nLegIDX;
      tagPartITEM m_sAbilIDX;   // short			m_nAbilIDX;	
      tagPartITEM m_sWeaponIDX; // short			m_nWeaponIDX;	
    };
  };
#else
  union 
  {
    //short				m_nPetPartItemIDX[ MAX_RIDING_PART ];
    tagPartITEM		m_sPetPartItemIDX[ MAX_RIDING_PART ];
    struct 
    {
      // 순서는 t_CharPART에 따라서...
      tagPartITEM	m_sEngineIDX;	// short			m_nEngineIDX;
      tagPartITEM	m_sBodyIDX;		// short			m_nBodyIDX;
      tagPartITEM	m_sLegIDX;		// short			m_nLegIDX;
      tagPartITEM	m_sAbilIDX;		// short			m_nAbilIDX;	
    } ;
  } ;
#endif

  //	int						m_iPetType;
  //	CObjCART*				m_pObjCART;

  CEffect* m_pWeaponJemEffect;

public:
  void SetPetType(int iPetType) { m_iPetType = iPetType; }
  WORD GetPetState();

  bool CreateCartFromMyData(bool bShowEffect                                                                             = false);
  bool CreateCart(unsigned int   iPetType, int iEnginePart, int iBodyPart, int iLegPart, int iAbilIPart, int iWeaponPart = 0);
  void DeleteCart(bool           bShowEffect                                                                             = false);

  void SetPetParts(unsigned int iPetPartIDX, unsigned int iItemIDX, bool bJustInfo = true);
  void UpdatePet();

  void SetWeightRate(BYTE btWeightRate);
  BYTE GetWeightRate();
  //////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////

  //----------------------------------------------------------------------------------------------------	
  /// @brief 팻모드 상태일때는 팻의 좌표를 리턴하고, 일반적일때는 내 좌표를 리턴함
  //----------------------------------------------------------------------------------------------------
  const D3DVECTOR& GetWorldPos();

  //----------------------------------------------------------------------------------------------------	
  /// @brief 모델에 등록된 renderUnit외의것들에 대한 충돌판정.. (아바타일경우만)
  //----------------------------------------------------------------------------------------------------
  bool IsIntersectAccessory(float& fCurDistance);

  virtual short GetStamina();
  void          SetStamina(short nStamina);

  //--------------------------------------------------------------------------------------
  /// 아바타의 스페셜한 상태
  //--------------------------------------------------------------------------------------
private:
  DWORD m_dwSubFLAG;

public:
  DWORD GetSpecialStateFLAG() { return m_dwSubFLAG; }
  void  ChangeSpecialState(DWORD dwSubFLAG);

  //----------------------------------------------------------------------------------------------------
  /// @brief 클릭할수 있는 객체인가?
  //----------------------------------------------------------------------------------------------------
  bool CanClickable() override;

};

//-------------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////////////////	
// 2005. 6. 20	박 지호 
//
// 여신소환 연출 클래스 
///////////////////////////////////////////////////////////////////////////////////////////	

typedef enum {
  GOD_NONE=0,
  GOD_APPEAR_PARTCLE,
  GOD_APPEAR_GODDESS,
  GOD_SPRITE_EFFECT,
  GOD_END_EFFECT,
  MAX_GOD_EFFECT

} GOD_STATE;

typedef struct _tagGODDESSSTR {

  BOOL      bEnable;   //GOD SetUp 상태 
  GOD_STATE god_State; //GOD 행동상태

  HNODE hGoddess;
  HNODE hSprite;

  WORD        idxMaster;    //연결될 캐릭터 오브젝트 
  DWORD       sTick, eTick; //출력에 관한 틱
  D3DXVECTOR3 cpos;         //출력될 위치 

  CObjMOB* pGODModel;
  CObjMOB* pSprModel;

  CEffect* pEffect[2];
  HNODE    hParticle[2];

  float frame; //프레임

  float fviewGODD; //비지블	
  float fviewSpr;

  void Init(void) {
    bEnable   = FALSE;
    god_State = GOD_NONE;

    pGODModel = nullptr;
    pSprModel = nullptr;

    hGoddess = NULL;
    hSprite  = NULL;

    for ( int i = 0; i < 2; i++ ) {
      pEffect[i]         = nullptr;
      hParticle[i]       = NULL;
    }

    idxMaster = 0;
    frame     = 0;
    fviewGODD = 0;
    fviewSpr  = 0;

    cpos = D3DXVECTOR3( 0, 0, 0 );

    sTick = eTick = 0;

  }

} GODDESSSTR, *LPGODDESSSTR;

typedef std::map<WORD, GODDESSSTR>::iterator GODLIST;

class CGoddessMgr {

  BOOL                  m_IsUse;
  DWORD                 m_Count;
  std::map<WORD, GODDESSSTR> m_list;

  DWORD m_dwAvata; //아바타 자신의 On/Off
  BOOL  m_IsAvata;

public:
  CGoddessMgr();
  ~CGoddessMgr();

  BOOL Init(void                );
  void Release(void             );
  void Release_List(GODDESSSTR* pGDS);
  BOOL Permission_Country(void  );

  BOOL Register_God_And_Sprite(void   );
  BOOL Register_RenderObj(GODDESSSTR& gds);
  BOOL SetProcess(DWORD               bonOff, WORD idx, BOOL bPrograss = FALSE);

  void Set_GDSData(GODDESSSTR& gds, BOOL bonOff, BOOL bPrograss);
  void Update(void             );
  int  ProcessVisible(float&   fv, float fseq);

  BOOL& IsAvataAroa(void ) { return m_IsAvata; }
  BOOL& IsGetUse(void    ) { return m_IsUse; }
  DWORD IsAvataState(void) { return m_dwAvata; }

};

extern CGoddessMgr goddessMgr;

#endif
