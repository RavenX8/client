# v3 branch analysis: data-type alignment vs packet changes

Method: client `git merge-base v3 network-rework` = 71c1da0, 18 commits on v3. Each file was diffed after removing all
non-ASCII bytes and CR, with `diff -w -B`, so the CP949/UTF-8 Korean comment re-encodings drop out. Only code
changes are listed below. Packets repo: merge-base(v3, origin/master) = 66ad64e, 9 commits on v3. Nothing was modified.

**Key fact: client-v3 and packets-v3 only work with each other.** d9a8924 set `GIT_TAG v3` in
`cmake/modules/Build-osirose-common.cmake`, and packets-v3 rewrote `item.xml` and `equipped_item.xml` from bitfield
unions to byte-wide fields. That is a wire-format change, and it is the only reason client-v3 widened `tagPartITEM`.
The current branch fetches packets `master` (`FETCHCONTENT_SOURCE_DIR_OSIROSE_COMMON` is empty in
cmake-build-release-ssl/CMakeCache.txt), where the old client bitfields already match the generated types bit for bit.

## 1. Client v3, by commit

### (a) Data-type alignment

| # | Commit | Change | Independent of (b)? |
|---|---|---|---|
| A1 | e892071 | `src/client/src/Common/DataTYPE.h:731-755`: all 21 `#define CLASS_*` commented out. `CUserDATA.cpp` (42 uses, lines ~199, 317-353, 380-405, 1786) now uses `RoseCommon::CLASS_SOLDIER_121` etc. This silently changes CLASS_MIXER_332 from 333 to 332. | Yes |
| A2 | e892071 | `DataTYPE.h:955` `#define MAX_UNION_COUNT 10` commented out. `CUserDATA.h:95` now `short m_nUnionPOINT[RoseCommon::MAX_UNION_COUNT]`, and `#include "dataconsts.h"` is added at CUserDATA.h:8. | Yes |
| A3 | e892071 | `Net_Prototype.h:918` `#define MAX_DAMAGE 0x05F5E0FF` commented out. `Calculation.cpp` (5 sites: 461, 493, 548, 589, 823) now `iDamage > RoseCommon::MAX_DAMAGE`. The value is the same (99,999,999). | Yes |
| A4 | e892071, reworked in 8fc1926 and 2666dd8 | `tagBaseITEM::operator=` from `SrvSetItem::Item`, `SrvSetMoneyAndItem::Item`, and (2666dd8) `SrvInventoryData::Item`, in Common/CItem.cpp and CItem.h. Final form is below. | The operator itself is independent. Its callers are (b). |
| A5 | 2666dd8 | `tagPartITEM::operator=(const SrvSelectCharReply::EquippedItem&)`: `m_nItemNo=get_id(); m_nGEM_OP=get_gem_opt(); m_bHasSocket=get_socket(); m_cGrade=get_grade();` | Operator independent. Caller is (b). |
| A6 | 2666dd8 | Common/CItem.h tagBaseITEM, quantity struct: adds `unsigned short m_nReserved1 : 1;` after `m_nItemNo_1:10`. This only names the padding bit, so the layout does not change. It does reuse the name of the existing `m_nReserved1:11` in the money struct. MSVC accepts that (checked with a small compile), but it is pointless. | Yes. Drop it. |
| A7 | 1b3ce8d | **tagPartITEM widened** in both `src/client/src/Common/CItem.h` and `src/common/CItem.h`: `unsigned int m_nItemNo:10, m_nGEM_OP:9, m_bHasSocket:1, m_cGrade:4` (4 bytes) becomes `uint16_t m_nItemNo; uint16_t m_nGEM_OP; int8_t m_bHasSocket; uint8_t m_cGrade;` (6 bytes; both sizes checked with static_assert on MSVC). This mirrors packets-v3's `equipped_item.xml`. | **No. It is a v3 wire change.** See §4. |
| A8 | ea958af | `CObjCHAR.h:1404` `SetAllPARTS` now maps by `CInventory::GetBodyPartToItemType(i)` instead of `GetBodyPartToEquipSlot(i)+1`. | **No.** It depends on how v3's `Recv_gsv_SELECT_CHAR` fills `m_PartITEM`. |

Final v3 form of A4 (CItem.cpp, from 2666dd8):
```cpp
tagBaseITEM& tagBaseITEM::operator=(const RoseCommon::Packet::SrvInventoryData::Item& item) {
  m_nItemNo = item.get_header().get_id(); m_cType = item.get_header().get_type();
  m_bCreated = item.get_header().get_isCreated();
  m_nGEM_OP = item.get_data().get_gem_opt(); m_cDurability = item.get_data().get_durability();
  m_nLife = item.get_data().get_life(); m_bHasSocket = item.get_data().get_hasSocket();
  m_bIsAppraisal = item.get_data().get_isAppraised(); m_cGrade = item.get_data().get_refine();
  if (IsEnableDupCNT() && item.get_data().get_count()) { m_cType_1=...; m_nItemNo_1=...; m_uiQuantity = item.get_data().get_count(); }
  return *this; }
```
History: e892071 used `m_wHeader = get_header().get_header()`. 8fc1926 switched to per-field copies plus an unguarded
`if (get_count()) m_uiQuantity = get_count()`. 2666dd8 added the `IsEnableDupCNT()` guard ("item data that have life and
durability would have their data overwritten"). On master the `Data` type is a union: `count` overlays gem/dur/life,
so without the guard equipment gets garbage in its quantity. **Port the 2666dd8 form.** On master there is a simpler
and exactly equivalent version: `m_wHeader = h.get_header(); m_dwBody = d.get_count();`, because both sides use the
same 16+32-bit layout. The `m_cType_1/m_nItemNo_1` writes alias the same bits and are redundant.

### (b) Packet, handler and network changes (exclude)

- **d9a8924 OTP / token login.** `SendPACKET.cpp` `Send_cli_LOGIN_REQ`: when `m_bDirectLogin` is set it sends
  `CliLoginTokenReq::create(m_OTPToken)`. `Send_cli_JOIN_SERVER_REQ` now uses
  `CliJoinServerTokenReq::create(dwLSVID, token)` or `CliJoinServerReq::create(dwLSVID, m_pMD5Buff)`; the old raw
  struct fill is commented out. Also: Game.cpp `m_OTPToken.clear()`; CLogin.cpp:418 skips the ID box when
  `m_bDirectLogin`; CGameStateLogin.cpp:135 auto-calls `ConnectLoginServer()` for direct login. d94273c simplified the
  conditions to `m_bDirectLogin && !m_OTPToken.empty()`. 1b3ce8d made CApplication.cpp:343 accept `_session` as an
  alias for `_otp`. Note that the token packets already exist on packets master (`cli_login_token_req.h` and
  `cli_join_server_token_req.h` are in gen/), so this flow could be ported later if you want it. It is still packet work.
- **e892071 dispatch.** CNetwork.cpp: `Proc_ZonePacket(std::unique_ptr<t_PACKET>)` (reverted to `t_PACKET*` in
  2666dd8), plus about 100 `packet` → `packet.get()` edits. `Recv_lsv_CHANNEL_LIST_REPLY(SrvChannelListReply&&)`
  rewritten (RecvPACKET.cpp:~4040). `Recv_wsv_CHAR_LIST(SrvCharListReply&&)`. `Recv_gsv_SET_MONEYnINV(SrvSetMoneyAndItem&&)`
  and `Recv_gsv_SET_INV_ONLY(SrvSetItem&&)` (RecvPACKET.cpp:~2077-2120), which are the callers of A4, plus the
  REPAIR/P_STORE forwarders. RecvPACKET.h prototypes changed to match. `Net_Prototype.h:436`
  `wsv_CHAR_LIST::m_btCharCNT` uint8_t → **uint64_t**. CSelectAvata.cpp:98 `nI` short → uint64_t. Both follow
  packets-v3's `occursSize="uint64_t"`.
- **94fd23d, f3af7d0, ce4c915, 5db3d1c, 95fccc9: single-connection login.** New `NS_TRN_TO_WSV` ProcLEVEL
  (CNetwork.h:19). DisconnectFromServer and MoveZoneServer return early. `Recv_lsv_SELECT_SERVER` with an empty IP
  stays connected and sends `Send_cli_JOIN_SERVER_REQ`. Adds `GetProcLevel()`. CGameStateLoginVirtual::Enter skips the
  disconnect and calls `AcceptedConnectLoginSvr()`.
- **2666dd8.** Adds `NS_TRN_TO_GSV` and more early returns in CNetwork.cpp. New
  `Recv_gsv_SELECT_CHAR(SrvSelectCharReply&&)` (RecvPACKET.cpp:~742-834; fills BasicINFO, BasicAbility, GrowAbility and
  Skill field by field, but **drops `effects`/`m_STATUS` and the hotbar**: only `m_HotICONS.Init()`). New
  `Recv_gsv_INVENTORY_DATA(SrvInventoryData&&)`, which uses A4 for `m_ItemLIST[i]`. Dispatch for
  PAKWC_SET_MONEY_AND_ITEM / PAKWC_SET_ITEM. Removes a duplicated BasicAbility CopyMemory (harmless).
  `PacketHEADER.h:7` `MAX_PACKET_SIZE` 1024 → `#ifndef … 0xFFF` (packets-v3 also uses 0xFFF; master uses 0x7FF).
  CClientSOCKET.cpp:304 is a cosmetic paren change.
- **c80627e and 1b3ce8d.** They loop over the SELECT_CHAR equipped items and settle on
  `m_PartITEM[BODY_PART_HELMET+i] = equippedItems[i]`, which uses A5. That index is correct: there are
  MAX_VISIBLE_ITEMS = 8 items, so indexes 2..9 are all below MAX_BODY_PART = 10.
- **d1664d4, aad1db2, 677f786, 8fc1926.** New `CSelectAvata::RecvAvataList(SrvCharListReply&)` (CSelectAvata.cpp:150+,
  .h:37). It uses `get_face()/get_hair()` and, after 8fc1926, indexes `items[SrvCharListReply::EquippedPosition::HELMET…WEAPON_L]`
  (packet-local, 0-based enum) and shows weapons. 677f786 fixes a `std::string x = nullptr` crash.

### (c) Other

- c4c646f `.gitignore`: `.idea/`, `cmake-*/`.
- 1004943 `.github/workflows/build.yml`: msvc-dev-cmd arch x86 → x64 (2 jobs).
- 2666dd8 `cmake/platform/win32/platform.cmake:50`: `IF(ENABLE_ASAN) add_compiler_flags(/fsanitize=address)`.
- e892071 `winMAIN.cpp:205`: `TI_ReadSysInfoFile()` commented out (non-debug builds).
- d9a8924 `GIT_TAG v3` in Build-osirose-common.cmake. **Do not keep.**
- `zz_build_count` bumps in most commits.

## 2. Packets repo: v3 vs origin/master

v3 commits: 8076196, 3bcc786, 7e75948, fa81d16, fc602c5, fbc83cc, 37fba5a, fa71c93, 9f5b0b7. origin/master is 22
commits past the merge-base.

**Type XML (all of it changes the wire format, so none of it is a pure type change):**
- `item.xml`: Header `{type:5,id:10,isCreated:1 | header u16}` becomes `{type u8, id u16, isCreated u8}`, so 2 → 4 bytes.
  Data `{gem_opt:9,dur:7,life:10,socket:1,appraised:1,refine:4 | count u32}` becomes the sequence
  `gem_opt u16, life u16, durability u8, hasSocket u8, isAppraised u8, refine u8, count u32`, so 4 → 12 bytes, with the
  field order changed (life before durability).
- `equipped_item.xml` and the inline copy in `srv_char_list_reply.xml`: `{id:10,gem_opt:9,socket:1,grade:4 | data u32}`
  becomes `id u16, gem_opt u16, socket i8, grade u8` (4 → 6 bytes).
- `include/types.h` HotbarItem: union `{type:5, slotId:11}` u16 becomes `uint8_t _type; uint16_t slotId` (2 → 3 bytes).
- `data_consts.xml`, `status_effect.xml`, `skill.xml`, `hotbar_item.xml`, `equipped_position.xml`: **unchanged.**

**Packet XML and other changes:** unbounded `occursSize` uint8_t → uint64_t (char list, channel list, set_item,
set_money_and_item; char-list items also get `occursSize="uint64_t"`). `cli/srv_equip_projectile`,
`srv_appraisal_reply` and `srv_party_levelexp` lose their bitfield unions. toggle_move enum RUN/SIT/DRIVE 2/8/9 →
0/1/2. Adds the token-login XMLs and ePacketType entries. `MAX_PACKET_SIZE` 0x7FF → 0xFFF. Deletes
`srv_set_money_and_inventory.xml`. isc_* packets are dropped from the build and factory. The `get_string`
`data.clear()` fix (fa71c93) **is already on master** (crosewriter.h:121, 131).

Verdict: packets-v3 has nothing to take for data types. Its "alignment with Rust types" consists of wire-format changes.

## 3. Current client (HEAD) vs the generated master types

**Which headers the client compiles.** The client uses `src/client/src/Common/{DataTYPE,CItem,CUserDATA}.h`.
- StdAfx.h:63 `#include "common/DataTYPE.h"` resolves relative to the including file, to src/client/src/Common/
  (the filesystem is case-insensitive).
- Net_Prototype.h:208-209 `"Common/DataTYPE.h"` (non-`__SERVER` branch) resolves through `-external:I src/client/src`.
- The only files compiled from src/common are CAssertNew, MiniDumper and Net_Prototype.h (src/client/CMakeLists.txt:439-443).
  No client file includes a bare `CItem.h`/`DataTYPE.h` from outside Common/.
- So `src/common/{DataTYPE,CItem,CUserDATA}.h` are dead for the client build, and 1b3ce8d's edit to src/common/CItem.h
  has no effect. The two DataTYPE.h copies differ only in formatting and comments; both have `CLASS_MIXER_332 333`
  (src/common/DataTYPE.h:680).

**data_consts.h** (`namespace RoseCommon`, constexpr) vs the client:

| Generated | Value | Client | Match |
|---|---|---|---|
| CLASS_MIXER_332 | 332 | DataTYPE.h:749 = **333** | **No.** Nothing in the client uses it, so the mismatch has no effect today. |
| other CLASS_* (20) | | DataTYPE.h:731-755 | Yes |
| MAX_UNION_COUNT | 10 | DataTYPE.h:955 | Yes |
| MAX_DAMAGE | 99999999 | Net_Prototype.h:918 0x05F5E0FF | Yes |
| MAX_STAT | 300 | MAX_BASIC_ABLITY CUserDATA.h:66 | Yes (different name) |
| MAX_STACK | 999 | MAX_DUP_ITEM_QUANTITY CItem.h:25 | Yes |
| MAX_SKILL_COUNT | 120 | MAX_LEARNED_SKILL_CNT CUserDATA.h:135 | Yes |
| MAX_HOTBAR_ITEMS | 32 | MAX_HOT_ICONS CHotICON.h:24 (`_NEWUI` off; with `_NEWUI` it is 48) | Yes |
| MAX_BUFF_STATUS / MAX_STATUS_EFFECTS | 40 | MAX_MAINTAIN_STATUS CUserDATA.h:70 | Yes |
| MAX_SELL_TYPE | 11 | MAX_PRICE_TYPE DataTYPE.h:232 | Yes |
| TAB_SIZE | 30 | INVENTORY_PAGE_SIZE 5*6 | Yes |
| MAX_INVENTORY | 120 | MAX_INV_TYPE(4)*30 | Yes |
| MAX_ITEMS | 140 | INVENTORY_TOTAL_SIZE = MAX_EQUIP_IDX 12 + MAX_SHOT_TYPE 3 + 120 + MAX_RIDING_PART 5 (5 only because `_GBC`, StdAfx.h:46). The "// 11" comment on MAX_EQUIP_IDX is wrong; the value is 12. | Yes |
| FIRST_BULLET_SLOT | 132 | INVENTORY_SHOT_ITEM0 = 12+120 | Yes |
| MAX_CONDITIONS_* 5/3/7/10, MAX_QUESTS 10, MAX_SWITCHES 16, QUEST vars/switches/items 10/32/6 | | QUEST_*_VAR_CNT, QUEST_PER_PLAYER, QUEST_SWITCH_CNT/32, CQuest.h:9-11 | Yes |
| MAX_WISHLIST | 30 | MAX_WISH_ITEMS CUserDATA.h:282 | Yes |
| MAX_VISIBLE_ITEMS | 8 | MAX_BODY_PART-2 | Yes |
| (epackettype) MAX_PACKET_SIZE | 0x7FF | PacketHEADER.h:7 = 1024 | **No.** It is a buffer cap, not a type. |

**Enums (rosecommon `enums.h`, hand-written, not IDL):**
- `RoseCommon::EquippedPosition` (GOGGLES=1 … EARING=11, MAX_EQUIP_ITEMS=12) equals `t_EquipINDEX` (FACE_ITEM=1 … EARRING=11, MAX_EQUIP_IDX=12).
- The **packet-local** `equipped_position.xml` enum (`SrvCharListReply::EquippedPosition`) is 0-based:
  HELMET, ARMOR, GAUNTLET, BOOTS, GOOGLES, BACKPACK, WEAPON_R, WEAPON_L, … That equals `BODY_PART_x - 2`, which is a
  different order from t_EquipINDEX. Two enums share the name and are easy to confuse.
- `RidingItem` BODY=135 equals INVENTORY_RIDE_ITEM0 (132+3). `BulletType` equals `t_eSHOT`.
- `ItemType` vs `t_eITEM`: the values match but the names differ. 7 is ITEM_RING vs JEWEL; 11 is ITEM_ETC_GEM vs
  ITEM_TYPE_ETC/GEM; **12 is ITEM_ETC vs ITEM_TYPE_NATURAL**; 13 is ITEM_ETC2 vs QUEST. Watch these if you substitute.

**Struct layouts vs master types:**
- `tagBaseITEM` (6 bytes: u16 header bits 5/10/1, u32 body 9/7/10/1/1/4 or quantity/money) equals `Item` {Header u16, Data u32}, bit for bit. Yes.
- `tagPartITEM` (unsigned int 10/9/1/4, sizeof 4) equals `EquippedItem` (10/9/1/4 | u32). Yes.
- `tagHotICON` (u16 5/11) equals master `HotbarItem` (types.h union). Yes.
- `tagMaintainSTATUS` {DWORD, short, short} equals `StatusEffect` {u32 expired, u16 value, u16 unknown}. Yes, except signedness.
- `tagSkillAbility` short[120] equals `skills` u16[MAX_SKILL_COUNT]. `Skill{id u16, level u8}` is a different type
  (used by skill packets) with no client struct to match.
- `tagBasicAbility` STR, DEX, INT, CON, CHARM, SENSE (short) equals the reply's str, dex, int_, con, charm, sense (u16).
- `tagGrowAbility` follows the SelectCharReply sequence field for field (i32 hp/mp, u32 xp, u16 level, u32 stat/skill
  points, u8 sizes, u32 penaltyXp, fame[2], unionPoints[10], u32 guild, u16, u8, pk, stamina, STATUS[40], patHp,
  patCoolTime). The only differences are short vs u16 on fame, union points, pk and stamina.
- `tagQuestData` / `CQUEST` equals `SrvQuestData` (16 switch DWORDs; quest = u16 id, u32 timer, u16 vars[10], u32 switches, Item[6]).

## 4. Recommendation

**Port, in this order** (each step is small and leaves the wire unchanged):
1. **Constants (A1-A3).** Delete the `CLASS_*`, `MAX_UNION_COUNT` and `MAX_DAMAGE` macros. Use `RoseCommon::` names
   in CUserDATA.cpp, CUserDATA.h:95 and Calculation.cpp. This also fixes the 332 value. Include the generated
   `data_consts.h` from CUserDATA.h, not `dataconsts.h`, which pulls in `types.h` → json.hpp and would put them in the
   PCH. Then delete the whole push_macro/pop_macro block in `GeneratedPackets.h`. **Ordering constraint:** until those
   macros are gone, any packet header that includes dataconsts must be reached only through GeneratedPackets.h.
   Otherwise StdAfx → DataTYPE.h macro-expands `constexpr uint32_t CLASS_SOLDIER_111` and the build breaks. (This is
   why v3 had to comment the macros out.)
2. **Item converters (A4, A5).** Port the `tagBaseITEM::operator=` and `tagPartITEM::operator=` from 2666dd8, with the
   `IsEnableDupCNT()` guard, or the equivalent `m_wHeader`/`m_dwBody` copy. Drop A6. `SrvSetItem::Item`,
   `SrvSetMoneyAndItem::Item`, `SrvInventoryData::Item` and `SrvQuestData::Item` are the same XML type generated once
   per packet, so a single template `from_item(const T&)` (or a static_assert that their getters agree) avoids four
   copies. Include the headers through GeneratedPackets.h. A semantic-vector test (tests/packets/) for an equipment
   item and a stackable item would catch the union/count trap.
3. Optional, after step 2: `static_assert(INVENTORY_TOTAL_SIZE == RoseCommon::MAX_ITEMS)`,
   `MAX_HOT_ICONS == MAX_HOTBAR_ITEMS`, `MAX_LEARNED_SKILL_CNT == MAX_SKILL_COUNT`, `MAX_MAINTAIN_STATUS == MAX_STATUS_EFFECTS`,
   and `sizeof(tagBaseITEM)==6`, `sizeof(tagPartITEM)==4`, `sizeof(tagHotICON)==2`. This puts the "mirror the
   generated types" goal into the code without changing any layout.

**Do not port:**
- **A7 (tagPartITEM widening).** It mirrors packets-v3's wire format. It would change sizeof from 4 to 6 and break every
  struct still parsed by union or memcpy: Net_Prototype.h:440 (`wsv_CHAR_LIST` item block), :503, :769 (`gsv_AVT_CHAR`
  m_PartITEM/m_RidingITEM), :1102 and :1115. It would also break CSelectAvata.cpp:111
  (`Packet_GetDataPtr(…, sizeof(tagPartITEM)*MAX_BODY_PART)`), the `CopyMemory(m_SelectedAvataInfo.m_PartITEM, …)` at
  RecvPACKET.cpp:714, and CObjCHAR's `m_sPartItemIDX`/`SetAllPetPARTS` CopyMemory (CObjCHAR.h:1413).
- **A8 (CObjCHAR SetAllPARTS remap).** It only makes sense with v3's SELECT_CHAR fill. Look at it again only when
  SELECT_CHAR is migrated.
- Everything in (b), the uint64 counts, MAX_PACKET_SIZE 0xFFF, the HotbarItem de-union and `GIT_TAG v3`.

**Risks:**
- Bitfield layout is compiler-defined. The client and the generated types happen to agree under MSVC with `pack(1)`.
  Keep conversions per field or by explicit u16/u32 words, never by `reinterpret_cast` of a generated object.
  Generated `PACK(struct{unsigned int …:5,:10,:1})` unions may be 4 bytes in memory even though the wire is 2.
- `CGame::m_SelectedAvataInfo` / `m_SelectedAvataINV` are still filled by `CopyMemory`/`memcpy` from
  `gsv_SELECT_CHAR`, `gsv_INVENTORY_DATA` and `gsv_QUEST_DATA` (RecvPACKET.cpp:714-733, 757, 766). Any size change to
  tagBaseITEM, tagPartITEM, tagGrowAbility, tagHotICON or tagQuestData silently corrupts those copies. Keep the
  sizeof static_asserts.
- If SELECT_CHAR is migrated later, use v3's handler only as a sketch. It omits `effects` (m_STATUS), the hotbar,
  `level`, and the pat HP/cooldown.
