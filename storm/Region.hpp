#ifndef STORM_REGION_HPP
#define STORM_REGION_HPP

#include "storm/region/Types.hpp"
#include <cstdint>

void SRgnClear(HSRGN handle);

void SRgnCombineRectf(HSRGN handle, RECTF* rect, void* param, int32_t combineMode);

void SRgnCreate(HSRGN* handlePtr, uint32_t reserved);

void SRgnDelete(HSRGN handle);

void SRgnGetBoundingRectf(HSRGN handle, RECTF* rect);

void SRgnGetRectsf(HSRGN handle, uint32_t* numrects, RECTF* buffer);

#endif
