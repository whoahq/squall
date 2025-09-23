#ifndef STORM_TRANSPARENCY_HPP
#define STORM_TRANSPARENCY_HPP

#include <cstdint>
#include "storm/Core.hpp"
#include "storm/region/Types.hpp"

#define STRANS_CF_INTERSECT 1
#define STRANS_CF_INVERTSECOND 2
#define STRANS_CF_SUBTRACT 3

#define STRANS_COLORKEY_VALID 0x1000000
#define STRANS_COLORKEY_MASK 0xFFFFFF
#define STRANS_COLORKEY(x) (((x) & STRANS_COLORKEY_MASK) | STRANS_COLORKEY_VALID)

struct TRANS;
typedef TRANS* HSTRANS;

int32_t STORMAPI STransBlt(uint8_t* dest, int32_t destx, int32_t desty, int32_t destpitch, HSTRANS transparency);

int32_t STORMAPI STransBltUsingMask(uint8_t* dest, uint8_t* source, int32_t destpitch, int32_t sourcepitch, HSTRANS mask);

int32_t STORMAPI STransCombineMasks(HSTRANS basemask, HSTRANS secondmask, int32_t offsetx, int32_t offsety, uint32_t flags, HSTRANS* handle);

// Exclusive rect - [left,right), [top,bottom)
int32_t STORMAPI STransCreateE(uint8_t* bits, int32_t width, int32_t height, int32_t bitdepth, RECT* rect, uint32_t colorkey, HSTRANS* handle);

// Inclusive rect - [left,right], [top,bottom]
int32_t STORMAPI STransCreateI(uint8_t* bits, int32_t width, int32_t height, int32_t bitdepth, RECT* rect, uint32_t colorkey, HSTRANS* handle);

int32_t STORMAPI STransCreateMaskE(uint8_t* bits, int32_t width, int32_t height, int32_t bitdepth, RECT* rect, uint32_t colorkey, HSTRANS* handle);

int32_t STORMAPI STransCreateMaskI(uint8_t* bits, int32_t width, int32_t height, int32_t bitdepth, RECT* rect, uint32_t colorkey, HSTRANS* handle);

int32_t STORMAPI STransDelete(HSTRANS handle);

int32_t STORMAPI STransDestroy();

int32_t STORMAPI STransDuplicate(HSTRANS source, HSTRANS* handle);

int32_t STORMAPI STransIntersectDirtyArray(HSTRANS sourcemask, uint8_t* dirtyarray, uint8_t dirtyarraymask, HSTRANS* handle);

int32_t STORMAPI STransInvertMask(HSTRANS sourcemask, HSTRANS* handle);

int32_t STORMAPI STransIsPixelInMask(HSTRANS mask, int32_t offsetx, int32_t offsety);

int32_t STORMAPI STransSetDirtyArrayInfo(int32_t screencx, int32_t screency, int32_t cellcx, int32_t cellcy);

int32_t STORMAPI STransUpdateDirtyArray(uint8_t* dirtyarray, uint8_t dirtyvalue, int32_t destx, int32_t desty, HSTRANS transparency, int32_t tracecontour = 0);

#endif
