#include "storm/big/Ops.hpp"

uint32_t ExtractLowPart(uint64_t& value) {
    auto low = static_cast<uint32_t>(value);
    value >>= 32;

    return low;
}

uint32_t ExtractLowPartSx(uint64_t& value) {
    auto low = static_cast<uint32_t>(value);
    value >>= 32;

    if (value >= 0x80000000) {
        reinterpret_cast<uint32_t*>(&value)[0] = value;
        reinterpret_cast<uint32_t*>(&value)[1] = -1;
    }

    return low;
}

uint64_t MakeLarge(uint32_t low, uint32_t high) {
    return low + (static_cast<uint64_t>(high) << 32);
}
