#ifndef STORM_TRANSPARENCY_HPP
#define STORM_TRANSPARENCY_HPP

#include <cstdint>
#include "storm/Handle.hpp"

struct TRANS;
typedef TRANS* HSTRANS;

int32_t STransBlt(uint8_t* dest, int32_t destx, int32_t desty, int32_t destpitch, HSTRANS transparency);

int32_t STransBltUsingMask(uint8_t* dest, uint8_t* source, int32_t destpitch, int32_t sourcepitch, HSTRANS mask);

int32_t STransDelete(HSTRANS handle);

int32_t STransDestroy();

int32_t STransDuplicate(HSTRANS source, HSTRANS* handle);

int32_t STransSetDirtyArrayInfo(int32_t screencx, int32_t screency, int32_t cellcx, int32_t cellcy);

#endif
