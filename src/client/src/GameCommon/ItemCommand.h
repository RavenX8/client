#ifndef _ItemCommand_
#define _ItemCommand_
/// 인벤토리에서 장착 아이템의 기본 Action Command
#include "tgamectrl/tcommand.h"

struct tagBaseITEM;

class CTCmdItemEquipInInventory : public CTCommand {
public:
          CTCmdItemEquipInInventory(void ) {}
  virtual ~CTCmdItemEquipInInventory(void) {}
  bool    Exec(CTObject*                 pObj) override;

};

class CTCmdItemUseInInventory : public CTCommand {
public:
          CTCmdItemUseInInventory(void ) {}
  virtual ~CTCmdItemUseInInventory(void) {}
  bool    Exec(CTObject*               pObj) override;
};

class CTCmdItemEtcInInventory : public CTCommand {
public:
          CTCmdItemEtcInInventory(void ) {}
  virtual ~CTCmdItemEtcInInventory(void) {}
  bool    Exec(CTObject*               pObj) override;
};

class CTCmdItemEquiped : public CTCommand {
public:
          CTCmdItemEquiped(void ) {}
  virtual ~CTCmdItemEquiped(void) {}
  bool    Exec(CTObject*        pObj) override;
};

class CTCmdItemBullet : public CTCommand {
public:
          CTCmdItemBullet(void ) {}
  virtual ~CTCmdItemBullet(void) {}
  bool    Exec(CTObject*       pObj) override;
};

#endif
