#pragma once

#include <cstdint>

// The generated item types (item.xml, equipped_item.xml), as the client's own
// item structs (tagBaseITEM, tagPartITEM in Common/CItem.h) hold them. No
// client headers here, so the packet tests can check it too.

namespace GeneratedItems {

// An equipped item, the 32-bit word of EquippedItem: id 10 bits, gem or option
// 9, socket 1, grade 4, from the low bit up. The character list and the select
// reply send the face and hair as this word too.
struct Equipped {
  uint16_t id;
  uint16_t gemOption;
  uint8_t  socket;
  uint8_t  grade;
};

constexpr Equipped equipped(uint32_t word) {
  return { static_cast<uint16_t>( word & 0x3FF ),
           static_cast<uint16_t>( ( word >> 10 ) & 0x1FF ),
           static_cast<uint8_t>( ( word >> 19 ) & 0x1 ),
           static_cast<uint8_t>( ( word >> 20 ) & 0xF ) };
}

} // namespace GeneratedItems
