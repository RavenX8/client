#ifndef _iconcommand_
#define _iconcommand_
#include "tgamectrl/TCommand.h"

//class CTCmdItemIconInBuyList : public CTCommand
//{
//public:
//	CTCmdItemIconInBuyList(void){}
//	virtual ~CTCmdItemIconInBuyList(void){}
//	virtual bool Exec( CTObject* pObj );
//};
//
//class CTCmdItemIconInSellList : public CTCommand
//{
//public:
//	CTCmdItemIconInSellList(void){}
//	virtual ~CTCmdItemIconInSellList(void){}
//	virtual bool Exec( CTObject* pObj );
//};

class CTCmdRemoveItemInBuyList : public CTCommand {
public:
          CTCmdRemoveItemInBuyList(void) {}
  virtual ~CTCmdRemoveItemInBuyList() {}
  bool    Exec(CTObject* pObj) override;
};

class CTCmdRemoveItemInSellList : public CTCommand {
public:
          CTCmdRemoveItemInSellList(void ) {}
  virtual ~CTCmdRemoveItemInSellList(void) {}
  bool    Exec(CTObject*                 pObj) override;
};
#endif
