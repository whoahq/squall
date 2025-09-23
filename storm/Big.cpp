#include "storm/Big.hpp"
#include "storm/big/Ops.hpp"
#include "storm/Memory.hpp"
#include <cstring>

void STORMAPI SBigAdd(BigData* a, BigData* b, BigData* c) {
    Add(a->Primary(), b->Primary(), c->Primary());
}

void STORMAPI SBigAnd(BigData* a, BigData* b, BigData* c) {
    And(a->Primary(), b->Primary(), c->Primary());
}

void STORMAPI SBigBitLen(BigData* num, uint32_t* len) {
    auto& buffer = num->Primary();
    buffer.Trim();

    auto index = buffer.Count() - 1;
    auto high = buffer[index];

    uint32_t bitIndex;
    for (bitIndex = 31; bitIndex > 0; bitIndex--) {
        if (((1 << bitIndex) & high)) {
            break;
        }
    }

    *len = (index * 32) + bitIndex + 1;
}

int32_t STORMAPI SBigCompare(BigData* a, BigData* b) {
    return Compare(a->Primary(), b->Primary());
}

void STORMAPI SBigCopy(BigData* a, BigData* b) {
    a->m_primary = b->m_primary;
}

void STORMAPI SBigDec(BigData* a, BigData* b) {
    Sub(a->m_primary, b->m_primary, 1);
}

void STORMAPI SBigDel(BigData* num) {
    delete num;
}

void STORMAPI SBigDiv(BigData* a, BigData* b, BigData* c) {
    uint32_t allocCount = 0;
    BigBuffer& buf = a->Stack().Alloc(&allocCount);

    Div(a->Primary(), buf, b->Primary(), c->Primary(), a->Stack());

    a->Stack().Free(allocCount);
}

void STORMAPI SBigFromBinary(BigData* num, const void* data, uint32_t bytes) {
    FromBinary(num->Primary(), data, bytes);
}

void STORMAPI SBigFromStr(BigData* num, const char *str) {
    FromStr(num->Primary(), str);
}

void STORMAPI SBigFromUnsigned(BigData* num, uint32_t val) {
    FromUnsigned(num->Primary(), val);
}

void STORMAPI SBigInc(BigData* a, BigData* b) {
    Add(a->Primary(), b->Primary(), 1);
}

int32_t STORMAPI SBigIsEven(BigData* a) {
    return IsEven(a->Primary());
}

int32_t STORMAPI SBigIsOdd(BigData* a) {
    return IsOdd(a->Primary());
}

int32_t STORMAPI SBigIsOne(BigData* a) {
    return IsOne(a->Primary());
}

int32_t STORMAPI SBigIsZero(BigData* a) {
    return IsZero(a->Primary());
}

void STORMAPI SBigMod(BigData* a, BigData* b, BigData* c) {
    uint32_t allocCount = 0;
    auto& scratch = a->Stack().Alloc(&allocCount);

    Div(scratch, a->Primary(), b->Primary(), c->Primary(), a->Stack());

    a->Stack().Free(allocCount);
}

void STORMAPI SBigMul(BigData* a, BigData* b, BigData* c) {
    Mul(a->Primary(), b->Primary(), c->Primary(), a->Stack());
}

void STORMAPI SBigNew(BigData** num) {
    *num = STORM_NEW(BigData);
}

void STORMAPI SBigNot(BigData* a, BigData* b) {
    Not(a->Primary(), b->Primary());
}

void STORMAPI SBigOr(BigData* a, BigData* b, BigData* c) {
    Or(a->Primary(), b->Primary(), c->Primary());
}

void STORMAPI SBigPowMod(BigData* a, BigData* b, BigData* c, BigData* d) {
    PowMod(a->Primary(), b->Primary(), c->Primary(), d->Primary(), a->Stack());
}

void STORMAPI SBigShl(BigData* a, BigData* b, uint32_t shift) {
    Shl(a->Primary(), b->Primary(), shift);
}

void STORMAPI SBigShr(BigData* a, BigData* b, uint32_t shift) {
    Shr(a->Primary(), b->Primary(), shift);
}

void STORMAPI SBigSquare(BigData* a, BigData* b) {
    Square(a->Primary(), b->Primary(), a->Stack());
}

void STORMAPI SBigSub(BigData* a, BigData* b, BigData* c) {
    Sub(a->Primary(), b->Primary(), c->Primary());
}

void STORMAPI SBigToBinaryBuffer(BigData* num, uint8_t* data, uint32_t maxBytes, uint32_t* bytes) {
    auto& output = num->Output();
    ToBinary(output, num->Primary());

    uint32_t n = output.Count() < maxBytes ? output.Count() : maxBytes;
    memcpy(data, output.Ptr(), n);

    if (bytes) {
        *bytes = n;
    }
}

void STORMAPI SBigToUnsigned(BigData* num, uint32_t* val) {
    ToUnsigned(val, num->Primary());
}

void STORMAPI SBigXor(BigData* a, BigData* b, BigData* c) {
    Xor(a->Primary(), b->Primary(), c->Primary());
}
