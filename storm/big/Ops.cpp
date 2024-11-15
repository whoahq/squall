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

void And(BigBuffer& a, const BigBuffer& b, const BigBuffer& c) {
    uint32_t i = 0;
    for (; b.IsUsed(i) || c.IsUsed(i); i++) {
        a[i] = c[i] & b[i];
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

void Div(BigBuffer& a, BigBuffer& b, const BigBuffer& c, const BigBuffer& d, BigStack& stack) {
    c.Trim();
    d.Trim();

    auto cCount = c.Count();
    auto dCount = d.Count();

    if (dCount > cCount) {
        b = c;
        SetZero(a);

        return;
    }

    if (dCount <= 1) {
        Div(a, &b[0], c, d[0]);
        b.SetCount(1);

        return;
    }

    uint32_t allocCount = 0;
    auto& cc = stack.Alloc(&allocCount);
    auto& dd = stack.Alloc(&allocCount);
    auto& work = stack.Alloc(&allocCount);

    uint32_t shift = 0x1F - (HighBitPos(d) & 0x1F);
    Shl(cc, c, shift);
    Shl(dd, d, shift);

    uint32_t t = dd[dCount - 1] + 1;
    auto v12 = cCount - dCount + 1;
    a.SetCount(v12);

    uint32_t index;
    if (v12) {
        while (true) {
            index = v12 - 1;
            a.SetOffset(v12 - 1);
            cc.SetOffset(v12 - 1);

            if (t) {
                a[0] = MakeLarge(cc[dCount - 1], cc[dCount]) / t;
            } else {
                a[0] = cc[dCount];
            }

            if (a[0]) {
                Mul(work, dd, a[0]);
                Sub(cc, cc, work);
            }

            while (cc[dCount] || Compare(cc, dd) >= 0) {
                a[0]++;
                Sub(cc, cc, dd);
            }

            if (index == 0) {
                break;
            }

            v12 = index;
        }
    }

    Shr(b, cc, shift);
    b.Trim();

    stack.Free(allocCount);
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
    uint32_t low = value & 0xFFFFFFFF;
    value >>= 32;

    if (value >= 0x80000000) {
        value |= 0xFFFFFFFFULL << 32;
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

void FromStr(BigBuffer& buffer, const char* str) {
    SetZero(buffer);
    for (; *str; str++) {
        Mul(buffer, buffer, 10);
        Add(buffer, buffer, *str - '0');
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

int32_t IsEven(const BigBuffer &num) {
    return num.Count() == 0 || (num[0] & 1) == 0;
}

int32_t IsOdd(const BigBuffer &num) {
    num.Trim();
    return num.Count() != 0 && (num[0] & 1) != 0;
}

int32_t IsOne(const BigBuffer &num) {
    num.Trim();
    return num.Count() == 1 && num[0] == 1;
}

int32_t IsZero(const BigBuffer &num) {
    num.Trim();
    return num.Count() == 0;
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

void MulMod(BigBuffer& a, const BigBuffer& b, const BigBuffer& c, const BigBuffer& d, BigStack& stack) {
    uint32_t allocCount = 0;
    auto& scratch = stack.Alloc(&allocCount);

    Mul(scratch, b, c, stack);
    Div(scratch, a, scratch, d, stack);

    stack.Free(allocCount);
}

void PowMod(BigBuffer& a, const BigBuffer& b, const BigBuffer& c, const BigBuffer& d, BigStack& stack) {
    c.Trim();

    if (c.Count() == 0) {
        SetOne(a);
        return;
    }

    uint32_t allocCount = 0;
    auto& temp = stack.Alloc(&allocCount);
    auto& b2 = stack.Alloc(&allocCount);
    auto& b3 = stack.Alloc(&allocCount);

    auto& aa = stack.MakeDistinct(a, &a == &b || &a == &c || &a == &d);

    MulMod(b2, b, b, d, stack);
    MulMod(b3, b2, b, d, stack);

    const BigBuffer* bPower[3];
    bPower[0] = &b;
    bPower[1] = &b2;
    bPower[2] = &b3;

    SetOne(aa);

    for (uint32_t i = c.Count() - 1; i + 1 > 0; i--) {
        auto v12 = c[i];
        uint32_t v23 = 32;

        if (i == c.Count() - 1 && !(v12 & 0xC0000000)) {
            do {
                v12 *= 4;
                v23 -= 2;
            } while (!(v12 & 0xC0000000));
        }

        if (v23) {
            for (uint32_t j = ((v23 - 1) >> 1) + 1; j > 0; j--) {
                Square(aa, aa, stack);
                Div(temp, aa, aa, d, stack);
                Square(aa, aa, stack);
                Div(temp, aa, aa, d, stack);

                if (v12 >> 30) {
                    auto& power = *bPower[(v12 >> 30) - 1];
                    MulMod(aa, aa, power, d, stack);
                }

                v12 *= 4;
            }
        }
    }

    stack.UnmakeDistinct(a, aa);
    stack.Free(allocCount);
}

void SetOne(BigBuffer& buffer) {
    buffer.SetCount(1);
    buffer[0] = 1;
}

void SetZero(BigBuffer& buffer) {
    buffer.Clear();
}

void Shl(BigBuffer& a, const BigBuffer& b, uint32_t shift) {
    auto v4 = shift >> 5;
    auto v14 = shift & 0x1F;
    auto v5 = b.Count() + v4 + 1;

    for (uint32_t i = v5 - 1, j = v5 - v4 - 1; i + 1 > 0; i--, j--) {
        uint32_t v9 = 0;

        if (i >= v4) {
            v9 = b[j] << v14;

            if (i > v4 && v14) {
                v9 += b[j - 1] >> (32 - v14);
            }
        }

        a[i] = v9;
    }

    a.SetCount(v5);
    a.Trim();
}

void Shr(BigBuffer& a, const BigBuffer& b, uint32_t shift) {
    auto v4 = shift >> 5;
    auto v9 = shift & 0x1F;

    uint32_t i = 0;
    for (i = 0; b.IsUsed(i + v4); i++) {
        auto v6 = b[i + v4] >> v9;

        if (v9) {
            v6 += b[i + v4 + 1] << (32 - v9);
        }

        a[i] = v6;
    }

    a.SetCount(i);
}

void Square(BigBuffer& a, const BigBuffer& b, BigStack& stack) {
    auto& aa = stack.MakeDistinct(a, &a == &b);
    aa.Clear();

    uint32_t i = 0;
    for (i = 0; b.IsUsed(i); i++) {
        uint64_t carry = 0;

        uint32_t j = 0;
        for (j = 0; j <= i; j++) {
            auto mul = b[i] * static_cast<uint64_t>(b[j]);
            auto add = mul + aa[i + j];

            if (j < i) {
                carry += mul;
            }

            aa[i + j] = ExtractLowPartLargeSum(carry, add);
        }

        aa[i + j] = ExtractLowPart(carry);
    }

    stack.UnmakeDistinct(a, aa);
}

void Sub(BigBuffer& a, const BigBuffer& b, const BigBuffer& c) {
    uint64_t borrow = 0;
    uint32_t i = 0;
    for (i = 0; b.IsUsed(i) || c.IsUsed(i); i++) {
        borrow += b[i] - static_cast<uint64_t>(c[i]);
        a[i] = ExtractLowPartSx(borrow);
    }

    a.SetCount(i);
    // This assert does not exist in retail WoW or Starcraft.
    //STORM_ASSERT(!borrow);
}

void Sub(BigBuffer& a, const BigBuffer& b, uint32_t c) {
    uint64_t borrow = 0;

    uint32_t i = 0;
    for (; b.IsUsed(i); i++) {
        borrow += b[i] - c;
        a[i] = ExtractLowPartSx(borrow);
    }

    a.SetCount(i);
    // This assert does not exist in retail WoW or Starcraft.
    //STORM_ASSERT(!borrow);
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
