# Client Packet Migration Plan

Goal: every client receive handler reads its packet through the IDL-generated
class (`RoseCommon::Packet::Srv*`) from the packets repository, not through the
`Net_Prototype.h` structs. That's the rule the Rust server follows: the XML is
the source of truth, wrong XML gets fixed upstream, and nothing is hand-coded.

- Build: only `cmake-build-release-ssl` (ENABLE_SSL=ON, DISABLE_CRYPT=ON).
- Process: you approve one step at a time and test each one in game.
- Every step ends with a green build and the step's tests. No commits unless
  you ask; no Co-Authored-By trailers.
- Audit, packet by packet: `PACKET_AUDIT.md`.

## What the audit found

123 dispatched packets. Already migrated: LOGIN_REPLY, CHAN_CHAR_REPLY,
SET_ANIMATION.

| Bucket | Meaning | Count |
|---|---|---|
| X | Live misread today: the server sends the XML layout, the client reads the old struct | 2 |
| A | XML matches the client byte for byte; the migration is mechanical | ~80, ~65 of them sent by the server |
| B | XML differs; the server hand-writes the client's layout | 9 |
| C | No XML, or the XML is not built | ~12 |
| N | Not sent by the new server | ~20, mostly layout A |

**X, the live bugs. In both, the XML is wrong and the client is right.**
- `PAKWC_CHANGE_MAP_REPLY`
  - The server sends 42 bytes; the client reads 60 (`gsv_JOIN_ZONE`).
  - Exp, penalty exp, the economy block and world time are misread. The team
    number comes from past the end of the packet.
  - This is the "attacking may not start" bug in Status.md.
  - The live damage today is the team number and the exp display. The
    economy block is misread too, but `GLOBAL_VARS` arrives right after and
    overwrites it.
  - Status.md calls this a client-only fix ("migrating that handler"). That's
    only half of it: the XML is wrong too. It has `xp`/`penalizeXp` as u16,
    while SET_EXP and LEVELUP use i64, and the server narrows them with
    `as u16` in `character.rs`. Migrating the client to today's XML would
    build a 16-bit exp into the client. The XML should be: i64 xp, i64
    penalizeXp, the `tagVAR_GLOBAL` block (i16, i32, i16, u8, u8[11], u32),
    u32 worldTime, i32 teamNumber.
- `PAKWC_BILLING_MESSAGE`
  - The XML holds the `_EXT` layout under the plain id. The client reads
    `{u8 type, [string]}`.
  - The effect is harmless by luck: a "lifetime free user" chat line on every
    zone entry.

**The generated headers in the SSL build are stale.** `gen/packets` is dated
2026-08-15; the XML is from 2026-11-15. With the old headers:
- `RIDE_REQUEST` reads 7 bytes instead of 5.
- `SHOP_OPEN` has no `skin`.
- `PLAYER_CHAR` has no tail.
- `CLAN_COMMAND` is empty.
- `BANK_LIST_REPLY` has no money.
- `TOGGLE_MOVE`, `PARTY_REPLY` and `QUEST_DATA_REPLY` have old enums.
- `HOTBAR_SET_ICON_REPLY` and both `QUEST_REWARD_*` headers are missing.

Nothing can be migrated until these are regenerated.

**B: XML/IDL changes needed upstream before the client can migrate.** The
server hand-writes these today to match the client. If the client moved to the
current XML, it would break against the running server.

| Packet | What the XML needs |
|---|---|
| `SKILL_CAST_SELF`, `SKILL_CAST_TARGET`, `SKILL_CAST_POSITION` | skill id i16, not i8; an optional trailing NPC motion byte (found by size) |
| `SKILL_EFFECT`, `SKILL_DAMAGE` | u16 caster INT, not float; `SKILL_DAMAGE` also needs an optional 18-byte drop tail |
| `SKILL_CANCEL` | enum off by one: the XML has 0/1/2, the client 1/2/3. **Not in "Packets Off the XML".** |
| `CLEAR_STATUS` | 0 to 2 i16 values, one per cleared HP/MP bit |
| `DAMAGE` | optional drop tail (absent on a kill without a drop) |
| `USE_ITEM` | optional `index` (the broadcast short form) |
| `PARTY_MEMBER` | a count-prefixed list, plus a count −1 "left" form |

**C: XML to write or build upstream.**
- Not listed in `rosecommon/CMakeLists.txt`, layout already right:
  `srv_skill_cast_position.xml`, `srv_messenger_chat.xml`.
- Bodies commented out: MEMO, MESSENGER (the sketch is wrong too: u32 tags, no
  prefix).
- Missing:
  - The REPAIR_NPC and REPAIR_USE_ITEM replies. The server re-frames
    `SrvSetMoneyAndItem` / `SrvSetItem`, so the client can decode them with
    those classes now.
  - GM_COMMAND, INIT_DATA, SERVER_DATA, CHATROOM, ANNOUNCE_TEXT and ERROR.
    None are sent.
- Cleanup: remove the duplicate unbuilt XMLs `srv_setexp`,
  `srv_set_money_and_inventory` and `srv_set_var_reply`.

## Steps

Each step is one approval and one in-game test.

### Step 0: upstream fixes for the live bugs (packets repo + server). Done, committed (packets 17e832e, osIROSE-new fb7233ed). Pay type stays 1.
Branch `fix/change-map-reply-billing` in both `packets` (from `origin/master`) and
`osIROSE-new` (from `rust-rewrite`). Nothing pushed: no GitHub, server to gitlab only.
- `srv_change_map_reply.xml`: 54-byte body as `gsv_JOIN_ZONE`. `srv_billing_message.xml`:
  `messageType u8 = 1, functionType i8, payFlag u32` (`gsv_BILLING_MESSAGE2`).
- Server: rose-proto regenerated (only these two files differ), `as u16` narrowing gone,
  byte-layout tests in `zone/src/packets/character.rs`, `semantic.txt` re-pinned.
- Not run: integration tests (Docker was not running), the C++ oracle (no
  `oracle/build/oracle`, so `cpp_reads_the_values_we_wrote` skips), the C++ servers' build.
- In-game test of step 0, with today's client: rebuild the containers from the
  server branch (`docker compose up -d --build`), attack a monster, check the exp
  bar after a map change. First integration test to run once Docker is up:
  `gateway/tests/world_flow.rs:173` (`team_number == 2`).
- Until merged, the client must build against the local checkout:
  `-DFETCHCONTENT_SOURCE_DIR_OSIROSE_COMMON=C:/Users/redfl/CLionProjects/packets`.
- Golden vectors for step 1 already exist: `rust/crates/rose-proto/tests/vectors/semantic.txt`
  (`<Class> <frame hex>` per line, values in `tests/semantic_vectors/*.rs`).

Original description:
- Packets repo:
  - Fix `srv_change_map_reply.xml` to the layout above.
  - Fix `srv_billing_message.xml` to `{u8 type, optional string}`, and move
    the current layout to `_EXT`.
- Server: run `regenerate.sh`, drop the `as u16` narrowing in
  `zone/src/packets/character.rs`, and run `cargo test -p rose-proto` with the
  oracle. Fix the BILLING_MESSAGE send.
- This is cross-repo work. Should I do the packets/server edits (on branches
  there), or will you?

### Step 1: regenerate the headers and add a test target (client build only)
Done on client branch `feat/packet-tests` (from `network-rework`).
- Packets updated to master `e967c94`; `gen/packets` regenerated (206 headers). The IDL
  `packet_generator` in the build was also stale (no `choice`), rebuilt from its pin `f36b43c`.
- `tests/` (`ClientPacketTests`, when `BUILD_TESTS`): googletest v1.15.2. Each line of the
  server's `semantic.txt` (`ROSE_SEMANTIC_VECTORS`, default `../osIROSE-new/rust/crates/
  rose-proto/tests/vectors/semantic.txt`) is read and written again by its generated class;
  the size the class computes and the bytes must match. `SrvChangeMapReply` and
  `SrvBillingMessage` are checked field by field. 57 tests pass; with the pre-step-0 vectors
  the change map reply fails its size and both fail their fields.
- OpenSSL: `openssl_runtime` copies `libssl-*.dll` and `libcrypto-*.dll` from
  `<OpenSSL>/bin` into `bin/`; `TRose` and the tests depend on it.
- Verified in a scratch build; `cmake-build-release-ssl` is rebuilt from CLion.

### Step 2: CHANGE_MAP_REPLY and GLOBAL_VARS (after step 0)
- `Recv_gsv_JOIN_ZONE(SrvChangeMapReply&&)`, plus `SET_EXP`/`LEVELUP` while
  there (same exp area).
- `Recv_gsv_SET_GLOBAL_VAR(SrvGlobalVars&&)`. This is housekeeping: it
  already reads correctly (the `tagVAR_GLOBAL*` cast is layout-exact).
- In-game test: nothing should change. Since step 0 the server sends exactly
  what `gsv_JOIN_ZONE` reads, so the attack and exp fixes are tested in step 0
  with today's client; this step only moves the handler to the generated class.
- Store trades should still work. If they're refused with `PRICE_DIFF`, the
  cause is not this step.

### Steps 3–9: bucket A, one area per step
The pattern follows the existing three: `SrvX::create(reinterpret_cast<const
uint8_t*>(packet))` in the dispatch, and the handler takes `SrvX&&`. Each step
adds golden-vector tests for its packets. The ordering puts the login path
first, so a broken step shows up immediately.

3. **Login and character select:** CHAR_LIST, CREATE/DELETE_CHAR,
   CHANNEL_LIST, SRV_SELECT_REPLY (`CSelectServer::RecvSelectServer` too),
   JOIN_SERVER_REPLY (both sockets), SELECT_CHAR, INVENTORY_DATA, QUEST_DATA.
   - SELECT_CHAR and QUEST_DATA memcpy whole structs into `CGame`, so they are
     mapped field by field. This is the riskiest step.
4. **Movement and view:** MOVE, MOUSE_CMD, STOP, REMOVE_OBJECT, NPC_CHAR,
   MOB_CHAR, PLAYER_CHAR (variable status/subFlag/clan tail), TELEPORT_REPLY
   (stored in `CGame::SetLoadingData`), TOGGLE_MOVE, SPEED_CHANGED.
5. **Combat and stats:** ATTACK, HP_REPLY, SET_HP_AND_MP, STAT_ADD_REPLY,
   SKILL_LEARN, SKILL_LEVEL_REPLY, SKILL_START, SKILL_FINISH.
6. **Items:** EQUIP_ITEM, EQUIP_ITEM_RIDE, EQUIP_PROJECTILE, DROP_ITEM,
   PICKUP_ITEM_REPLY, SET_ITEM, SET_MONEY, SET_MONEY_AND_ITEM, SET_ITEM_LIFE,
   the repair replies, HOTBAR_SET_ICON_REPLY.
7. **Chat and party:** NORMAL/SHOUT/WHISPER/PARTY/ALLIED/ALLIED_SHOUT chat,
   ANNOUNCE_CHAT, CLAN_CHAT, PARTY_REQ/REPLY/RULE/ITEM/LEVELEXP,
   PARTY_MEMBER_UPDATE.
8. **Trade, stores, bank, crafting:** TRADE, TRADE_ITEM, STORE_TRADE_REPLY,
   SHOP_OPEN/CLOSE/LIST_REPLY/BUYSELL_REPLY, BANK_LIST_REPLY/MOVE_ITEM/
   MOVE_MONEY, CRAFT_REPLY (`CMakeDLG::RecvResult` reads it raw),
   CRAFT_STATUS, CRAFT_ENHANCE_REPLY, APPRAISAL_REPLY.
9. **Quests, riding, clans, the rest:** QUEST_DATA_REPLY, the QUEST_REWARD_*
   packets (keep the ITEM→SET_ITEM fallthrough), NPC_EVENT, RIDE_REQUEST,
   RIDE_STATE_CHANGE, CLAN_COMMAND, LOGOUT_REPLY, and the not-sent A packets.

### Step 10+: bucket B and C, as the upstream XML lands
One step per upstream change: the packets PR, server regenerate (the server
stops hand-writing), then the client migrates. Suggested order: SKILL_CANCEL
(enum only), the skill packets, DAMAGE, USE_ITEM, CLEAR_STATUS, PARTY_MEMBER,
then messenger and memo. Optional tails need an IDL feature ("present if
bytes remain"), which goes into IDL upstream like `occursRef` did.

### Last: cleanup
Remove the `t_PACKET*` overloads and the old `gsv_*` structs once nothing reads
them. Update the server's Status.md and "Packets Off the XML".

## Questions for you
1. Step 0: will you do the packets XML and server changes, or should I?
2. Golden vectors from a Rust test on the server side (preferred), or vectors
   built by hand in C++?
3. Does the step grouping suit how you want to test, or do you want smaller
   steps?
