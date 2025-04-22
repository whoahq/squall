#ifndef STORM_REGION_HPP
#define STORM_REGION_HPP

#include "storm/region/Types.hpp"
#include <cstdint>

void SRgnClear(HSRGN handle);

void SRgnCombineRectf(HSRGN handle, RECTF* rect, void* param, int32_t combineMode);

void SRgnCreate(HSRGN* handlePtr, uint32_t reserved = 0);

void SRgnDelete(HSRGN handle);

void SRgnDuplicate(HSRGN origHandle, HSRGN* handle, uint32_t reserved = 0);

void SRgnGetBoundingRectf(HSRGN handle, RECTF* rect);

void SRgnGetRectParamsf(HSRGN handle, RECTF* rect, uint32_t* numParams, void** buffer);

void SRgnGetRectsf(HSRGN handle, uint32_t* numRects, RECTF* buffer);

int32_t SRgnIsPointInRegionf(HSRGN handle, float x, float y);

int32_t SRgnIsRectInRegionf(HSRGN handle, const RECTF* rect);

void SRgnOffsetf(HSRGN handle, float xoffset, float yoffset);

#endif
