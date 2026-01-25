#ifndef STORM_BITMAP_HPP
#define STORM_BITMAP_HPP

#include <cstdint>
#include "Core.hpp"

#define SBMP_IMAGETYPE_AUTO 0
#define SBMP_IMAGETYPE_BMP 1
#define SBMP_IMAGETYPE_PCX 2
#define SBMP_IMAGETYPE_TGA 3

struct STORM_PALETTEENTRY {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t flags;
};

typedef void* (STORMAPI* SBMPALLOCPROC)(uint32_t);

int32_t STORMAPI SBmpAllocLoadImage(const char* filename, STORM_PALETTEENTRY* paletteentries, uint8_t** returnedbuffer, uint32_t* width = nullptr, uint32_t* height = nullptr, uint32_t* bitdepth = nullptr, uint32_t requestedbitdepth = 0, SBMPALLOCPROC allocproc = nullptr);

int32_t STORMAPI SBmpDecodeImage(int32_t imagetype, uint8_t* imagedata, uint32_t imagebytes, STORM_PALETTEENTRY* paletteentries, uint8_t* bitmapbits, uint32_t buffersize, uint32_t* width = nullptr, uint32_t* height = nullptr, uint32_t* bitdepth = nullptr);

int32_t STORMAPI SBmpGetPitchForAlignment(int32_t width, int32_t align);

int32_t STORMAPI SBmpLoadImage(const char* filename, STORM_PALETTEENTRY* paletteentries, uint8_t* bitmapbits, uint32_t buffersize, uint32_t* width = nullptr, uint32_t* height = nullptr, uint32_t* bitdepth = nullptr);

int32_t STORMAPI SBmpPadImage(uint8_t* imagedata, uint32_t imagebytes, int32_t width, int32_t height, int32_t alignment, int32_t* out_newwidth);

#endif
