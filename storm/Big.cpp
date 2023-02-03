#include "storm/Big.hpp"
#include "storm/big/Ops.hpp"
#include "storm/Memory.hpp"
#include <cstring>

void SBigAdd(BigData* a, BigData* b, BigData* c) {
    Add(a->Primary(), b->Primary(), c->Primary());
}

void SBigBitLen(BigData* num, uint32_t* len) {
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

int32_t SBigCompare(BigData* a, BigData* b) {
    return Compare(a->Primary(), b->Primary());
}

void SBigDel(BigData* num) {
    delete num;
}

void SBigFromBinary(BigData* num, const void* data, uint32_t bytes) {
    FromBinary(num->Primary(), data, bytes);
}

void SBigFromUnsigned(BigData* num, uint32_t val) {
    FromUnsigned(num->Primary(), val);
}

void SBigNew(BigData** num) {
    auto m = SMemAlloc(sizeof(BigData), __FILE__, __LINE__, 0x0);
    *num = new (m) BigData();
}

void SBigMul(BigData* a, BigData* b, BigData* c) {
    Mul(a->Primary(), b->Primary(), c->Primary(), a->Stack());
}

void SBigShr(BigData* a, BigData* b, uint32_t shift) {
    Shr(a->Primary(), b->Primary(), shift);
}

void SBigSquare(BigData* a, BigData* b) {
    Square(a->Primary(), b->Primary(), a->Stack());
}

void SBigSub(BigData* a, BigData* b, BigData* c) {
    Sub(a->Primary(), b->Primary(), c->Primary());
}

void SBigToBinaryBuffer(BigData* num, uint8_t* data, uint32_t maxBytes, uint32_t* bytes) {
    auto& output = num->Output();
    ToBinary(output, num->Primary());

    uint32_t n = output.Count() < maxBytes ? output.Count() : maxBytes;
    memcpy(data, output.Ptr(), n);

    if (bytes) {
        *bytes = n;
    }
}
