#ifndef STORM_CRYPTO_S_ARC4_KEY_HPP
#define STORM_CRYPTO_S_ARC4_KEY_HPP

#include <cstdint>

struct SARC4Key {
    uint8_t state[256];
    uint8_t x;
    uint8_t y;
};

#endif
