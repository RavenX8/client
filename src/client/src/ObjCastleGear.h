#ifndef _OBJ_CASTLE_GEAR_
#define _OBJ_CASTLE_GEAR_

#include "cobjcart.h"

enum enumCASTLE_GEAR_ANI {
  CASTLE_GEAR_ANI_STOP1 = 0,
  CASTLE_GEAR_ANI_MOVE,
  CASTLE_GEAR_ANI_ATTACK01,
  CASTLE_GEAR_ANI_ATTACK02,
  CASTLE_GEAR_ANI_ATTACK03,
  CASTLE_GEAR_ANI_DIE,
  CASTLE_GEAR_ANI_CASTING,
  CASTLE_GEAR_ANI_SKILL,
};

//----------------------------------------------------------------------------------------------------
/// class CObjCastleGear
/// switch 문으로 CObjCART 로 처리 할수도 있었지만 차후 Castle Gear 만의 기능추가를 위해서 분리..
//----------------------------------------------------------------------------------------------------

class CObjCastleGear : public CObjCART {
public:
  CObjCastleGear(void );
  ~CObjCastleGear(void);

  //////////////////////////////////////////////////////////////////////////////////////////	
  /// < Inherited from CGameOBJ virtual functions
  /// <

  /*override*/
  int Get_TYPE() override { return OBJ_CGEAR; }

  /// <
  /// < end	
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////	
  /// < Inherited from CObjAI virtual functions
  /// <

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
  int GetANI_Skill() override;

  /*override*/
  void SetCMD_ATTACK(int iServerTarget, WORD wSrvDIST, const D3DVECTOR& PosGOTO) override;
  /// <
  /// < End
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////	
  /// < Inherited from CObjCHAR virtual functions
  /// <

  /// <
  /// < end	
  //////////////////////////////////////////////////////////////////////////////////////////

  bool Create(CObjCHAR* pParent, int iCartType, D3DVECTOR& Position) override;

};

#endif ///_OBJ_CASTLE_GEAR_
