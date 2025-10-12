#ifndef ENGINE_NETWORK_CUSTOM_PROTOCOL_HPP
#define ENGINE_NETWORK_CUSTOM_PROTOCOL_HPP
#include <stdint.h>
#include <stddef.h>
namespace engine { namespace network {
struct PacketHeader {
    uint64_t sequence_id;
    uint8_t ack_flag;
    uint16_t payload_length;
    uint8_t reserved;
    uint32_t checksum;
    uint8_t payload[]; 
};
}}
#endif
