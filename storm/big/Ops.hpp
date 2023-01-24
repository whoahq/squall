#ifndef STORM_BIG_OPS_HPP
#define STORM_BIG_OPS_HPP

#include <cstdint>

uint32_t ExtractLowPart(uint64_t& value);

uint32_t ExtractLowPartSx(uint64_t& value);

uint64_t MakeLarge(uint32_t low, uint32_t high);

#endif
