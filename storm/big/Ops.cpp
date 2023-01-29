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

void FromUnsigned(BigBuffer& buffer, uint32_t value) {
    buffer[0] = value;
    buffer.SetCount(1);
}

uint64_t MakeLarge(uint32_t low, uint32_t high) {
    return low + (static_cast<uint64_t>(high) << 32);
}

void ToBinaryAppend(TSGrowableArray<uint8_t>& output, const BigBuffer& buffer) {
    for (uint32_t i = 0; i < buffer.Count() * 4; i++) {
        auto byte = buffer[i / 4] >> (8 * (i & 3));

        if (byte || (i / 4) + 1 < buffer.Count()) {
            *output.New() = byte;
        }
    }
}

void ToBinary(TSGrowableArray<uint8_t>& output, const BigBuffer& buffer) {
    output.SetCount(0);
    ToBinaryAppend(output, buffer);
}
