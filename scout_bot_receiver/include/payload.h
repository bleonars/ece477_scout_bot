#ifndef __PAYLOAD_H__
#define __PAYLOAD_H__

#include <cinttypes>

namespace Utils {
    uint8_t payload_to_u8(uint8_t *payload) {
        return payload[0];
    }

    uint16_t payload_to_u16(uint8_t *payload) {
        return ((payload[1] << 8) |
                 payload[0]);
    }

    uint32_t payload_to_u32(uint8_t *payload) {
        return ((payload[3] << 24) |
                (payload[2] << 16) |
                (payload[1] << 8)  |
                 payload[0]);
    }
    
    float payload_to_f32(uint8_t *payload) {
        uint32_t u32 = payload_to_u32(payload);
        
        return (*(float *) &u32);
    }
}

#endif