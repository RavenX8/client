/*
	$Header: /Client/CObjGROUND.h 32    04-07-08 6:10p Jeddli $
*/
#ifndef	__COBJGROUND_H
#define	__COBJGROUND_H
#include "CObjFIXED.h"
#include "Common/CItem.h"

///
/// 필드 아이템...
///

class CObjITEM : public CObjFIXED {
private:
  HNODE m_hDummyPosition;
  HNODE m_hAnimatable;

  static HNODE m_hMotion;

public :
          CObjITEM();
  virtual ~CObjITEM();

  tagITEM m_ITEM;

  DWORD m_dwCreatedTIME;
  WORD  m_wOwnerServerObjIDX;
  WORD  m_wRemainTIME;

  //////////////////////////////////////////////////////////////////////////////////////////
  /// < Inherited from CGameObj 

  /*override*/
  int Get_TYPE() override { return OBJ_ITEM; }

  /*override*/
  void InsertToScene(void) override;
  /*override*/
  void RemoveFromScene(bool bIncludeEFFECT = true) override;
  /*override*/
  char* Get_NAME() override;

  /*override*/
  int Proc() override;

  /// < End 
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////
  /// < Inherited from CObjFIXED 

  /*override*/
  char* Make_ZNAME(int iCreateOrder, short nPartIdx) override { return CStr::Printf( NAME_ITEMOBJ, iCreateOrder, nPartIdx ); }

  /*override*/
  void Delete() override;

  /// < End 
  //////////////////////////////////////////////////////////////////////////////////////////

  static bool SetMotion(const char* strMotion);
  static void ClearMotion();

  bool CreateAnimatable();

};

///
/// 나무, 풀, 바위...
///

class CObjTREE : public CObjFIXED {
private:

public :
          CObjTREE();
  virtual ~CObjTREE();

  //////////////////////////////////////////////////////////////////////////////////////////
  /// < Inherited from CGameObj 

  /*override*/
  int Get_TYPE() override { return OBJ_GROUND; }

  /// < End 
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////
  /// < Inherited from CObjFIXED 

  /*override*/
  char* Make_ZNAME(int iCreateOrder, short nPartIdx) override { return CStr::Printf( NAME_TREEOBJ, iCreateOrder, nPartIdx ); }

  /// < End 
  //////////////////////////////////////////////////////////////////////////////////////////	
};

///
/// 건물
///

class CObjCNST : public CObjFIXED {
private:

public :
          CObjCNST();
  virtual ~CObjCNST();

  //////////////////////////////////////////////////////////////////////////////////////////
  /// < Inherited from CGameObj 

  /*override*/
  int Get_TYPE() override { return OBJ_CNST; }

  /// < End 
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////
  /// < Inherited from CObjFIXED 

  /*override*/
  char* Make_ZNAME(int iCreateOrder, short nPartIdx) override { return CStr::Printf( NAME_CNSTOBJ, iCreateOrder, nPartIdx ); }

  /// < End 
  //////////////////////////////////////////////////////////////////////////////////////////	
};

//-------------------------------------------------------------------------------------------------
#endif
