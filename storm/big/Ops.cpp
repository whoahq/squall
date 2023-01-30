#include "storm/big/Ops.hpp"

void Add(BigBuffer& a, const BigBuffer& b, const BigBuffer& c) {
    uint64_t carry = 0;
    uint32_t i = 0;
    for (i = 0; carry || b.IsUsed(i) || c.IsUsed(i); i++) {
        carry += static_cast<uint64_t>(b[i]) + c[i];
        a[i] = ExtractLowPart(carry);
    }

    a.SetCount(i);
}

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

void FromBinary(BigBuffer& buffer, const void* data, uint32_t bytes) {
    buffer.Clear();

    for (uint32_t i = 0; i < bytes; i++) {
        auto byte = static_cast<const uint8_t*>(data)[i];
        auto v7 = (i & 3) ? buffer[i / 4] : 0;
        buffer[i / 4] = v7 + (byte << (8 * (i & 3)));
    }
}

void FromUnsigned(BigBuffer& buffer, uint32_t value) {
    buffer[0] = value;
    buffer.SetCount(1);
}

uint64_t MakeLarge(uint32_t low, uint32_t high) {
    return low + (static_cast<uint64_t>(high) << 32);
}

void Mul(BigBuffer& a, const BigBuffer& b, uint64_t c) {
    // TODO assertion c <= SMALL_BOUND

    uint64_t carry = 0;
    uint32_t i = 0;
    for (i = 0; carry || b.IsUsed(i); i++) {
        carry += b[i] * c;
        a[i] = ExtractLowPart(carry);
    }

    a.SetCount(i);
}

void Mul(BigBuffer& a, const BigBuffer& b, const BigBuffer& c, BigStack& stack) {
    auto& aa = stack.MakeDistinct(a, &a == &b || &a == &c);
    aa.Clear();

    uint32_t i = 0;
    for (i = 0; b.IsUsed(i); i++) {
        uint64_t carry = 0;

        uint32_t j = 0;
        for (j = 0; c.IsUsed(j); j++) {
            carry += aa[i + j] + b[i] * static_cast<uint64_t>(c[j]);
            aa[i + j] = ExtractLowPart(carry);
        }

        aa[i + j] = ExtractLowPart(carry);
    }

    stack.UnmakeDistinct(a, aa);
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
