#ifndef ENGINE_NETWORK_CUSTOM_PROTOCOL_HPP
#define ENGINE_NETWORK_CUSTOM_PROTOCOL_HPP
#include <stdint.h>
#include <stddef.h>
namespace engine { namespace network {
#pragma pack(push, 1)
struct PacketHeader {
    uint64_t sequence_id;
    uint8_t ack_flag;
    uint16_t payload_length;
    uint8_t reserved;
    uint32_t checksum;
    uint8_t payload[]; 
};
#pragma pack(pop)
static_assert(sizeof(PacketHeader) == 16, "PacketHeader struct alignment mismatch");
static_assert(offsetof(PacketHeader, sequence_id) == 0, "offset mismatch");
static_assert(offsetof(PacketHeader, ack_flag) == 8, "offset mismatch");
static_assert(offsetof(PacketHeader, payload_length) == 9, "offset mismatch");
static_assert(offsetof(PacketHeader, reserved) == 11, "offset mismatch");
static_assert(offsetof(PacketHeader, checksum) == 12, "offset mismatch");
}}
#endif
