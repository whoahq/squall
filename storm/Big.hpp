#ifndef STORM_BIG_HPP
#define STORM_BIG_HPP

#include "storm/big/BigData.hpp"
#include <cstdint>

void SBigAdd(BigData* a, BigData* b, BigData* c);

void SBigAnd(BigData* a, BigData* b, BigData* c);

void SBigBitLen(BigData* num, uint32_t* len);

int32_t SBigCompare(BigData* a, BigData* b);

void SBigCopy(BigData* a, BigData* b);

void SBigDec(BigData* a, BigData* b);

void SBigDel(BigData* num);

void SBigDiv(BigData* a, BigData* b, BigData* c);

void SBigFromBinary(BigData* num, const void* data, uint32_t bytes);

void SBigFromStr(BigData* num, const char* str);

void SBigFromUnsigned(BigData* num, uint32_t val);

void SBigInc(BigData* a, BigData* b);

int32_t SBigIsEven(BigData* a);

int32_t SBigIsOdd(BigData* a);

int32_t SBigIsOne(BigData* a);

int32_t SBigIsZero(BigData* a);

void SBigMod(BigData* a, BigData* b, BigData* c);

void SBigMul(BigData* a, BigData* b, BigData* c);

void SBigNew(BigData** num);

void SBigNot(BigData* a, BigData* b);

void SBigOr(BigData* a, BigData* b, BigData* c);

void SBigPowMod(BigData* a, BigData* b, BigData* c, BigData* d);

void SBigShl(BigData* a, BigData* b, uint32_t shift);

void SBigShr(BigData* a, BigData* b, uint32_t shift);

void SBigSquare(BigData* a, BigData* b);

void SBigSub(BigData* a, BigData* b, BigData* c);

void SBigToBinaryBuffer(BigData* num, uint8_t* data, uint32_t maxBytes, uint32_t* bytes);

void SBigXor(BigData* a, BigData* b, BigData* c);

void SBigToUnsigned(BigData* num, uint32_t* val);

#endif
