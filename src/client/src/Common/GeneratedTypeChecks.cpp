#include "StdAfx.h"
#include "CHotICON.h"
#include "CInventory.h"
#include "CItem.h"
#include "CUserDATA.h"

// The client's own types and limits against the ones the packets library
// generates (data_consts.xml, item.xml, equipped_item.xml, hotbar_item.xml).
// Handlers copy between them field by field, and some old structs are still
// copied whole, so a change on either side has to fail the build here.

static_assert( INVENTORY_TOTAL_SIZE == RoseCommon::MAX_ITEMS, "inventory slots" );
static_assert( MAX_HOT_ICONS == RoseCommon::MAX_HOTBAR_ITEMS, "hotbar slots" );
static_assert( MAX_LEARNED_SKILL_CNT == RoseCommon::MAX_SKILL_COUNT, "skill slots" );
static_assert( MAX_MAINTAIN_STATUS == RoseCommon::MAX_STATUS_EFFECTS, "status effects" );
static_assert( MAX_PRICE_TYPE == RoseCommon::MAX_SELL_TYPE, "item price rates" );

static_assert( sizeof( tagBaseITEM ) == 6, "Item: a 16-bit header and a 32-bit body" );
static_assert( sizeof( tagPartITEM ) == 4, "EquippedItem: 32 bits" );
static_assert( sizeof( tagHotICON ) == 2, "HotbarItem: 16 bits" );
