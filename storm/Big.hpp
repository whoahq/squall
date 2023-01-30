#ifndef STORM_BIG_HPP
#define STORM_BIG_HPP

#include "storm/big/BigData.hpp"
#include <cstdint>

void SBigAdd(BigData* a, BigData* b, BigData* c);

void SBigDel(BigData* num);

void SBigFromBinary(BigData* num, const void* data, uint32_t bytes);

void SBigFromUnsigned(BigData* num, uint32_t val);

void SBigNew(BigData** num);

void SBigToBinaryBuffer(BigData* num, uint8_t* data, uint32_t maxBytes, uint32_t* bytes);

#endif
