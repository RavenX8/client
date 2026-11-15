// The packets the Rust server pinned in rose-proto's tests/vectors/semantic.txt
// (one "<Class> <frame hex>" per line), read and written again with the
// client's generated classes. A class generated from older XML than the
// server's reads a different number of bytes, so its size or its bytes differ.
//
// The values each frame carries are in rose-proto's tests/semantic_vectors/*.rs;
// the packets the client has been migrated to are checked field by field below.

#include <gtest/gtest.h>

#include <cstdint>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "cli_attack.h"
#include "cli_change_char_req.h"
#include "cli_change_map_req.h"
#include "cli_channel_list_req.h"
#include "cli_craft_enhance_req.h"
#include "cli_craft_req.h"
#include "cli_create_char_req.h"
#include "cli_delete_char_req.h"
#include "cli_hp_req.h"
#include "cli_join_server_req.h"
#include "cli_join_server_token_req.h"
#include "cli_login_token_req.h"
#include "cli_logout_req.h"
#include "cli_mouse_cmd.h"
#include "cli_normal_chat.h"
#include "cli_select_char_req.h"
#include "cli_shop_buysell_reply.h"
#include "cli_shop_open.h"
#include "cli_srv_select_req.h"
#include "cli_stop_moving.h"
#include "isc_server_register.h"
#include "srv_accept_reply.h"
#include "srv_attack.h"
#include "srv_billing_message.h"
#include "srv_chan_char_reply.h"
#include "srv_change_map_reply.h"
#include "srv_channel_list_reply.h"
#include "srv_char_list_reply.h"
#include "srv_craft_enhance_reply.h"
#include "srv_create_char_reply.h"
#include "srv_damage.h"
#include "srv_delete_char_reply.h"
#include "srv_hp_reply.h"
#include "srv_inventory_data.h"
#include "srv_join_server_reply.h"
#include "srv_levelup.h"
#include "srv_login_reply.h"
#include "srv_logout_reply.h"
#include "srv_mob_char.h"
#include "srv_mouse_cmd.h"
#include "srv_normal_chat.h"
#include "srv_npc_char.h"
#include "srv_party_member.h"
#include "srv_player_char.h"
#include "srv_quest_data.h"
#include "srv_remove_object.h"
#include "srv_ride_request.h"
#include "srv_select_char_reply.h"
#include "srv_set_exp.h"
#include "srv_set_item.h"
#include "srv_shop_open.h"
#include "srv_srv_select_reply.h"

namespace {

using namespace RoseCommon;
using namespace RoseCommon::Packet;

using Bytes = std::vector<uint8_t>;

struct Vector {
  std::string name;
  Bytes       frame;
};

Bytes from_hex(const std::string& hex) {
  Bytes out;
  for (size_t i = 0; i + 1 < hex.size(); i += 2) {
    out.push_back(static_cast<uint8_t>(std::stoul(hex.substr(i, 2), nullptr, 16)));
  }
  return out;
}

std::vector<Vector> load_vectors() {
  std::vector<Vector> out;
  std::ifstream       in(ROSE_SEMANTIC_VECTORS);
  std::string         line;
  while (std::getline(in, line)) {
    std::istringstream fields(line);
    Vector             v;
    std::string        hex;
    if (fields >> v.name >> hex) {
      v.frame = from_hex(hex);
      out.push_back(std::move(v));
    }
  }
  return out;
}

const std::vector<Vector>& vectors() {
  static const std::vector<Vector> all = load_vectors();
  return all;
}

const Vector& first(const std::string& name) {
  for (const auto& v : vectors()) {
    if (v.name == name) return v;
  }
  throw std::runtime_error("no vector named " + name);
}

// What the class itself works out its size to be, rather than the size it
// read from the header (which get_size() returns once a packet is read).
template <class T>
struct Probe : T {
  explicit Probe(T&& packet) : T(std::move(packet)) {}

  size_t packed_size() const {
    CRoseSizer sizer(0, 0);
    this->pack(sizer);
    return sizer.get_size() + 6;
  }
};

template <class T>
void round_trip(const Bytes& frame) {
  Probe<T> packet(T::create(frame.data()));
  ASSERT_EQ(packet.packed_size(), frame.size()) << "the class reads a different layout";
  const auto packed = packet.getPacked();
  EXPECT_EQ(Bytes(packed.get(), packed.get() + frame.size()), frame);
}

#define ROUND_TRIP(Class) { #Class, &round_trip<Class> }

const std::map<std::string, void (*)(const Bytes&)> kRoundTrips = {
    ROUND_TRIP(CliAttack),
    ROUND_TRIP(CliChangeCharReq),
    ROUND_TRIP(CliChangeMapReq),
    ROUND_TRIP(CliChannelListReq),
    ROUND_TRIP(CliCraftEnhanceReq),
    ROUND_TRIP(CliCraftReq),
    ROUND_TRIP(CliCreateCharReq),
    ROUND_TRIP(CliDeleteCharReq),
    ROUND_TRIP(CliHpReq),
    ROUND_TRIP(CliJoinServerReq),
    ROUND_TRIP(CliJoinServerTokenReq),
    ROUND_TRIP(CliLoginTokenReq),
    ROUND_TRIP(CliLogoutReq),
    ROUND_TRIP(CliMouseCmd),
    ROUND_TRIP(CliNormalChat),
    ROUND_TRIP(CliSelectCharReq),
    ROUND_TRIP(CliShopBuysellReply),
    ROUND_TRIP(CliShopOpen),
    ROUND_TRIP(CliSrvSelectReq),
    ROUND_TRIP(CliStopMoving),
    ROUND_TRIP(IscServerRegister),
    ROUND_TRIP(SrvAcceptReply),
    ROUND_TRIP(SrvAttack),
    ROUND_TRIP(SrvBillingMessage),
    ROUND_TRIP(SrvChanCharReply),
    ROUND_TRIP(SrvChangeMapReply),
    ROUND_TRIP(SrvChannelListReply),
    ROUND_TRIP(SrvCharListReply),
    ROUND_TRIP(SrvCraftEnhanceReply),
    ROUND_TRIP(SrvCreateCharReply),
    ROUND_TRIP(SrvDamage),
    ROUND_TRIP(SrvDeleteCharReply),
    ROUND_TRIP(SrvHpReply),
    ROUND_TRIP(SrvInventoryData),
    ROUND_TRIP(SrvJoinServerReply),
    ROUND_TRIP(SrvLevelup),
    ROUND_TRIP(SrvLoginReply),
    ROUND_TRIP(SrvLogoutReply),
    ROUND_TRIP(SrvMobChar),
    ROUND_TRIP(SrvMouseCmd),
    ROUND_TRIP(SrvNormalChat),
    ROUND_TRIP(SrvNpcChar),
    ROUND_TRIP(SrvPartyMember),
    ROUND_TRIP(SrvPlayerChar),
    ROUND_TRIP(SrvQuestData),
    ROUND_TRIP(SrvRemoveObject),
    ROUND_TRIP(SrvRideRequest),
    ROUND_TRIP(SrvSelectCharReply),
    ROUND_TRIP(SrvSetExp),
    ROUND_TRIP(SrvSetItem),
    ROUND_TRIP(SrvShopOpen),
    ROUND_TRIP(SrvSrvSelectReply),
};

#undef ROUND_TRIP

class SemanticVector : public ::testing::TestWithParam<size_t> {};

TEST(SemanticVectors, AreThere) {
  ASSERT_FALSE(vectors().empty()) << "no vectors in " << ROSE_SEMANTIC_VECTORS;
}

TEST_P(SemanticVector, ReadsAndWritesTheSameBytes) {
  const auto& v     = vectors().at(GetParam());
  const auto  check = kRoundTrips.find(v.name);
  ASSERT_NE(check, kRoundTrips.end()) << v.name << " has no class here: add it to kRoundTrips";
  check->second(v.frame);
}

INSTANTIATE_TEST_SUITE_P(
  Rust, SemanticVector, ::testing::Range<size_t>(0, vectors().size()),
  [](const ::testing::TestParamInfo<size_t>& info) {
    return vectors()[info.param].name + "_" + std::to_string(info.param);
  });

// The values rose-proto's semantic_vectors/zone.rs wrote.
TEST(SemanticVectors, ChangeMapReply) {
  const auto p = SrvChangeMapReply::create(first("SrvChangeMapReply").frame.data());
  EXPECT_EQ(p.get_objectIndex(), 7);
  EXPECT_EQ(p.get_hp(), 300);
  EXPECT_EQ(p.get_mp(), 80);
  EXPECT_EQ(p.get_xp(), 5'000'000'000LL);
  EXPECT_EQ(p.get_penalizeXp(), 77);
  EXPECT_EQ(p.get_craftRate(), 1);
  EXPECT_EQ(p.get_updateTime(), 2);
  EXPECT_EQ(p.get_worldRate(), 3);
  EXPECT_EQ(p.get_townRate(), 4);
  EXPECT_EQ(p.get_itemRate(10), 9);
  EXPECT_EQ(p.get_flags(), 0x01020304u);
  EXPECT_EQ(p.get_worldTime(), 70'000u);
  EXPECT_EQ(p.get_teamNumber(), 42);
}

TEST(SemanticVectors, BillingMessage) {
  const auto p = SrvBillingMessage::create(first("SrvBillingMessage").frame.data());
  EXPECT_EQ(p.get_messageType(), 3);
  EXPECT_EQ(p.get_functionType(), 'H');
  EXPECT_EQ(p.get_payFlag(), 2u);
}

} // namespace
