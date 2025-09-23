#ifndef STORM_BIG_HPP
#define STORM_BIG_HPP

#include "storm/big/BigData.hpp"
#include "Core.hpp"

void STORMAPI SBigAdd(BigData* a, BigData* b, BigData* c);

void STORMAPI SBigAnd(BigData* a, BigData* b, BigData* c);

void STORMAPI SBigBitLen(BigData* num, uint32_t* len);

int32_t STORMAPI SBigCompare(BigData* a, BigData* b);

void STORMAPI SBigCopy(BigData* a, BigData* b);

void STORMAPI SBigDec(BigData* a, BigData* b);

void STORMAPI SBigDel(BigData* num);

void STORMAPI SBigDiv(BigData* a, BigData* b, BigData* c);

void STORMAPI SBigFromBinary(BigData* num, const void* data, uint32_t bytes);

void STORMAPI SBigFromStr(BigData* num, const char* str);

void STORMAPI SBigFromUnsigned(BigData* num, uint32_t val);

void STORMAPI SBigInc(BigData* a, BigData* b);

int32_t STORMAPI SBigIsEven(BigData* a);

int32_t STORMAPI SBigIsOdd(BigData* a);

int32_t STORMAPI SBigIsOne(BigData* a);

int32_t STORMAPI SBigIsZero(BigData* a);

void STORMAPI SBigMod(BigData* a, BigData* b, BigData* c);

void STORMAPI SBigMul(BigData* a, BigData* b, BigData* c);

void STORMAPI SBigNew(BigData** num);

void STORMAPI SBigNot(BigData* a, BigData* b);

void STORMAPI SBigOr(BigData* a, BigData* b, BigData* c);

void STORMAPI SBigPowMod(BigData* a, BigData* b, BigData* c, BigData* d);

void STORMAPI SBigShl(BigData* a, BigData* b, uint32_t shift);

void STORMAPI SBigShr(BigData* a, BigData* b, uint32_t shift);

void STORMAPI SBigSquare(BigData* a, BigData* b);

void STORMAPI SBigSub(BigData* a, BigData* b, BigData* c);

void STORMAPI SBigToBinaryBuffer(BigData* num, uint8_t* data, uint32_t maxBytes, uint32_t* bytes);

void STORMAPI SBigXor(BigData* a, BigData* b, BigData* c);

void STORMAPI SBigToUnsigned(BigData* num, uint32_t* val);

#endif
