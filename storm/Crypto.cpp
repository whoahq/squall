#include "storm/Crypto.hpp"
#include "storm/Error.hpp"
#include <cstring>

void SARC4PrepareKey(const void* data, uint32_t len, SARC4Key* key) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(data);
    STORM_VALIDATE_END_VOID;

    key->x = 0;
    key->y = 0;

    // ARC4 key-scheduling algorithm

    for (uint32_t i = 0; i < 256; i++) {
        key->state[i] = i;
    }

    uint32_t j = 0;
    for (uint32_t i = 0; i < 256; i++) {
        j = (j + key->state[i] + static_cast<const uint8_t*>(data)[i % len]) % 256;

        // Swap values
        auto si = key->state[i];
        auto sj = key->state[j];
        key->state[i] = sj;
        key->state[j] = si;
    }
}

void SARC4ProcessBuffer(void* data, uint32_t len, const SARC4Key* inKey, SARC4Key* outKey) {
    if (inKey != outKey) {
        memcpy(outKey, inKey, sizeof(SARC4Key));
    }

    // Note: The original implementation uses two loops. The first loop decrypts/encrypts data
    // 4-bytes-at-a-time. The second loop decrypts/encrypts all bytes outside of that alignment
    // 1-byte-at-a-time. For simplicity's sake, our implementation handles 1-byte-at-a-time.

    auto x = outKey->x;
    auto y = outKey->y;

    for (uint32_t i = 0; i < len; i++) {
        x += 1;
        auto sx = outKey->state[x];

        y += sx;
        auto sy = outKey->state[y];

        outKey->state[x] = sy;
        outKey->state[y] = sx;

        static_cast<uint8_t*>(data)[i] ^= outKey->state[static_cast<uint8_t>(sx + sy)];
    }

    outKey->x = x;
    outKey->y = y;
}
