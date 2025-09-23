#ifndef STORM_REGION_HPP
#define STORM_REGION_HPP

#include "storm/region/Types.hpp"
#include "Core.hpp"

void STORMAPI SRgnClear(HSRGN handle);

void STORMAPI SRgnCombineRectf(HSRGN handle, const RECTF* rect, void* param, int32_t combineMode);

void STORMAPI SRgnCombineRecti(HSRGN handle, const RECT* rect, void* param, int32_t combineMode);

void STORMAPI SRgnCreate(HSRGN* handlePtr, uint32_t reserved = 0);

void STORMAPI SRgnDelete(HSRGN handle);

void STORMAPI SRgnDuplicate(HSRGN origHandle, HSRGN* handle, uint32_t reserved = 0);

void STORMAPI SRgnGetBoundingRectf(HSRGN handle, RECTF* rect);

void STORMAPI SRgnGetBoundingRecti(HSRGN handle, RECT* rect);

void STORMAPI SRgnGetRectParamsf(HSRGN handle, const RECTF* rect, uint32_t* numParams, void** buffer);

void STORMAPI SRgnGetRectParamsi(HSRGN handle, const RECT* rect, uint32_t* numParams, void** buffer);

void STORMAPI SRgnGetRectsf(HSRGN handle, uint32_t* numRects, RECTF* buffer);

void STORMAPI SRgnGetRectsi(HSRGN handle, uint32_t* numRects, RECT* buffer);

int32_t STORMAPI SRgnIsPointInRegionf(HSRGN handle, float x, float y);

int32_t STORMAPI SRgnIsPointInRegioni(HSRGN handle, int32_t x, int32_t y);

int32_t STORMAPI SRgnIsRectInRegionf(HSRGN handle, const RECTF* rect);

int32_t STORMAPI SRgnIsRectInRegioni(HSRGN handle, const RECT* rect);

void STORMAPI SRgnOffsetf(HSRGN handle, float xoffset, float yoffset);

void STORMAPI SRgnOffseti(HSRGN handle, int32_t xoffset, int32_t yoffset);

#endif
