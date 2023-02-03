#include "storm/big/Ops.hpp"

void Add(BigBuffer& a, const BigBuffer& b, uint32_t c) {
    uint64_t carry = c;
    uint32_t i = 0;
    for (i = 0; carry || b.IsUsed(i); i++) {
        carry += b[i];
        a[i] = ExtractLowPart(carry);
    }

    a.SetCount(i);
}

void Add(BigBuffer& a, const BigBuffer& b, const BigBuffer& c) {
    uint64_t carry = 0;
    uint32_t i = 0;
    for (i = 0; carry || b.IsUsed(i) || c.IsUsed(i); i++) {
        carry += static_cast<uint64_t>(b[i]) + c[i];
        a[i] = ExtractLowPart(carry);
    }

    a.SetCount(i);
}

int32_t Compare(const BigBuffer& a, const BigBuffer& b) {
    int32_t result = 0;

    for (int32_t i = 0; a.IsUsed(i) || b.IsUsed(i); i++) {
        if (a[i] != b[i]) {
            result = b[i] < a[i] ? 1 : -1;
        }
    }

    return result;
}

void Div(BigBuffer& a, uint32_t* b, const BigBuffer& c, uint64_t d) {
    // TODO STORM_ASSERT(d <= SMALL_BOUND);

    auto index = c.Count();
    a.SetCount(index);

    uint64_t data = 0;
    while (index > 0) {
        InsertLowPart(data, c[--index]);
        a[index] = data / d;
        data %= d;
    }

    a.Trim();
    *b = data;
}

uint32_t ExtractLowPart(uint64_t& value) {
    auto low = static_cast<uint32_t>(value);
    value >>= 32;

    return low;
}

uint32_t ExtractLowPartLargeSum(uint64_t& value, uint64_t add) {
    auto carry = (value += add) < add;
    auto low = ExtractLowPart(value);
    value += static_cast<uint64_t>(carry) << 32;

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

uint32_t HighBitPos(const BigBuffer& buffer) {
    uint32_t index = buffer.Count();
    if (index == 0) {
        return 0;
    }

    while (buffer[--index] == 0) {
        if (index == 0) {
            return 0;
        }
    }

    uint32_t mask = 0x80000000;
    uint32_t bitIndex = 32;
    while (bitIndex > 0) {
        bitIndex--;

        if (buffer[index] & mask) {
            return (index * 32) + bitIndex;
        }

        mask >>= 1;
    }

    return 0;
}

void InsertLowPart(uint64_t& value, uint32_t low) {
    value = (value << 32) | low;
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

void SetOne(BigBuffer& buffer) {
    buffer.SetCount(1);
    buffer[0] = 1;
}

void SetZero(BigBuffer& buffer) {
    buffer.Clear();
}

void Sub(BigBuffer& a, const BigBuffer& b, const BigBuffer& c) {
    uint64_t borrow = 0;
    uint32_t i = 0;
    for (i = 0; b.IsUsed(i) || c.IsUsed(i); i++) {
        borrow += b[i] - static_cast<uint64_t>(c[i]);
        a[i] = ExtractLowPartSx(borrow);
    }

    a.SetCount(i);

    STORM_ASSERT(!borrow);
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
