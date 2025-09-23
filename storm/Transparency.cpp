#include "storm/Transparency.hpp"
#include "storm/List.hpp"
#include "storm/region/Types.hpp"
#include "storm/Error.hpp"

#include <algorithm>
#include <climits>
#include <cstring>

#define COPY 0
#define SKIP 1
#define MAXSPANLENGTH 0xFC

// Replacing windows struct
struct WHOA_SIZE {
    int32_t cx, cy;
};

struct TRANS : TSLinkedNode<TRANS> {
    uint8_t* data;
    uint32_t dataalloc;
    uint32_t databytes;
    uint32_t instructionoffset;
    int32_t width;
    int32_t height;
    RECT boundrect;
};

WHOA_SIZE s_dirtysize = { 40, 30 };
int32_t s_dirtyxshift = 4;
int32_t s_dirtyxsize = 16;
int32_t s_dirtyyshift = 4;
int32_t s_dirtyysize = 16;

uint32_t* s_dirtyoffset;
uint8_t* s_savedata;
uint32_t s_savedataalloc;
STORM_LIST(TRANS) s_translist;

void ConvertBitmapToTransparency(uint8_t* bits, int32_t width, int32_t height, int32_t bitdepth, RECT* rect, RECT* boundrect, uint8_t colorkey, int32_t maskonly, uint8_t* data, uint32_t* databytes, uint32_t* instructionoffset) {
    int32_t size = 0;
    int32_t start = rect ? rect->left + width * rect->top : 0;
    int32_t cx = rect ? rect->right - rect->left : width;
    int32_t cy = rect ? rect->bottom - rect->top : height;

    int32_t adjust = width - cx;

    *boundrect = { INT_MAX, INT_MAX, 0, 0 };

    if (!maskonly) {
        uint8_t* source = &bits[start];
        for (int32_t y = 0; y < cy; y++) {
            bool found = false;
            for (int32_t x = 0; x < cx; x++) {
                if (*source != colorkey) {
                    size++;
                    if (data) *data++ = *source;

                    if (x < boundrect->left) boundrect->left = x;
                    if (x >= boundrect->right) boundrect->right = x + 1;
                    found = true;
                }
                source++;
            }

            if (found) {
                if (y < boundrect->top) boundrect->top = y;
                if (y >= boundrect->bottom) boundrect->bottom = y + 1;
            }

            source += adjust;
        }
    }

    if (boundrect->left > boundrect->right) boundrect->left = boundrect->right;
    if (boundrect->top > boundrect->bottom) boundrect->top = boundrect->bottom;

    if (size % 4 != 0) {
        if (data) data += 4 - (size % 4);
        size += 4 - (size % 4);
    }

    if (instructionoffset) *instructionoffset = size;

    uint8_t* source = &bits[start];
    while (cy--) {
        uint8_t copybytes = 0;
        uint8_t skipbytes = 0;
        int32_t copymode = SKIP;

        for (int32_t x = cx; x--;) {
            bool output = false;
            int32_t expectedmode = *source++ != colorkey ? SKIP : COPY;
            if (expectedmode == copymode) {
                if (copymode == COPY) {
                    copybytes++;
                }
                else {
                    skipbytes++;
                }
            }
            else if (copymode == COPY) {
                output = true;
                source--;
                x++;
            }
            else {
                copymode = COPY;
                skipbytes++;
            }

            if (output || copybytes == MAXSPANLENGTH || skipbytes == MAXSPANLENGTH || x == 0) {
                size += 2;
                if (data) {
                    *data++ = copybytes;
                    *data++ = skipbytes;
                }
                copybytes = 0;
                skipbytes = 0;
                copymode = SKIP;
            }
        }

        size += 2;
        if (data) {
            *data++ = 0;
            *data++ = 0;
        }

        source += adjust;
    }

    if (databytes) *databytes = size;
}

int32_t ConvertColorRefToColorData(uint32_t colorref) {
    if (colorref & 0x1000000) {
        return colorref & 0xFFFFFF;
    }
    return 0;
}

HSTRANS CreateTransparencyRecord(HSTRANS baseptr) {
    HSTRANS transptr = s_translist.NewNode(STORM_LIST_TAIL, 0, 0);
    if (baseptr) {
        transptr->dataalloc = baseptr->dataalloc;
        transptr->databytes = baseptr->databytes;
        transptr->instructionoffset = baseptr->instructionoffset;
        transptr->width = baseptr->width;
        transptr->height = baseptr->height;
        transptr->boundrect = baseptr->boundrect;
    }
    return transptr;
}

int32_t DetermineShift(int32_t value, int32_t* shift) {
    int32_t bits = 0, curr = 1;
    while (curr < value) {
        bits++;
        curr <<= 1;
    }

    *shift = bits;
    return curr == value;
}

int32_t InternalCreateTransparency(uint8_t* bits, int32_t width, int32_t height, int32_t bitdepth, RECT* rect, uint32_t colorkey, int32_t maskonly, HSTRANS* handle) {
    if (bitdepth != 8) return 0;

    uint8_t paletteindex = ConvertColorRefToColorData(colorkey);
    uint32_t transbytes = 0;
    uint32_t instructionoffset = 0;
    RECT boundrect;

    ConvertBitmapToTransparency(bits, width, height, bitdepth, rect, &boundrect, paletteindex, maskonly, nullptr, &transbytes, &instructionoffset);

    uint8_t* transdata = static_cast<uint8_t*>(STORM_ALLOC(transbytes));
    ConvertBitmapToTransparency(bits, width, height, bitdepth, rect, &boundrect, paletteindex, maskonly, transdata, nullptr, nullptr);

    HSTRANS newptr = CreateTransparencyRecord(nullptr);
    newptr->boundrect = boundrect;
    newptr->data = transdata;
    newptr->dataalloc = transbytes;
    newptr->databytes = transbytes;
    newptr->instructionoffset = instructionoffset;
    newptr->width = rect ? rect->right - rect->left : width;
    newptr->height = rect ? rect->bottom - rect->top : height;

    *handle = newptr;
    return 1;
}

void InternalDrawTransparency(HSTRANS trans, uint8_t* dest, int32_t destadjust) {
    uint8_t* sourceinst = &trans->data[trans->instructionoffset];
    uint8_t* sourcedata = trans->data;
    int32_t cy = trans->height;
    while (cy--) {
        uint8_t copybytes = *sourceinst++;
        uint8_t skipbytes = *sourceinst++;
        while (copybytes || skipbytes) {
            while(copybytes) {
                *dest++ = *sourcedata++;
                copybytes--;
            }
            dest += skipbytes;

            copybytes = *sourceinst++;
            skipbytes = *sourceinst++;
        }
        dest += destadjust;
    }
}

void InternalDrawTransparencyFromSource(HSTRANS trans, uint8_t* dest, uint8_t* source, int32_t destadjust, int32_t sourceadjust) {
    uint8_t* sourceinst = &trans->data[trans->instructionoffset];
    for (int y = 0; y < trans->height; y++) {
        uint8_t copybytes = *(sourceinst++);
        uint8_t skipbytes = *(sourceinst++);
        while (copybytes || skipbytes) {
            std::memcpy(dest, source, copybytes);
            source += copybytes;
            dest += copybytes;

            dest += skipbytes;
            source += skipbytes;

            copybytes = *(sourceinst++);
            skipbytes = *(sourceinst++);
        }
        dest += destadjust;
        source += sourceadjust;
    }
}

int32_t STORMAPI STransBlt(uint8_t* dest, int32_t destx, int32_t desty, int32_t destpitch, HSTRANS transparency) {
    STORM_ASSERT(dest);
    STORM_ASSERT(destpitch > 0);
    STORM_ASSERT(transparency);
    STORM_ASSERT(transparency->instructionoffset);

    InternalDrawTransparency(transparency, &dest[destpitch * desty + destx], destpitch - transparency->width);
    return 1;
}

int32_t STORMAPI STransBltUsingMask(uint8_t* dest, uint8_t* source, int32_t destpitch, int32_t sourcepitch, HSTRANS mask) {
    STORM_ASSERT(dest);
    STORM_ASSERT(source);
    STORM_ASSERT(destpitch > 0);
    STORM_ASSERT(mask);

    int32_t sourceadjust = sourcepitch ? sourcepitch - mask->width : 0;
    int32_t destadjust = destpitch - mask->width;
    InternalDrawTransparencyFromSource(mask, dest, source, destadjust, sourceadjust);
    return 1;
}

int32_t STORMAPI STransCreateE(uint8_t* bits, int32_t width, int32_t height, int32_t bitdepth, RECT* rect, uint32_t colorkey, HSTRANS* handle) {
    if (handle) *handle = nullptr;

    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(bits);
    STORM_VALIDATE(width);
    STORM_VALIDATE(height);
    STORM_VALIDATE(handle);
    STORM_VALIDATE_END;

    return InternalCreateTransparency(bits, width, height, bitdepth, rect, colorkey, 0, handle);
}

int32_t STORMAPI STransCreateI(uint8_t* bits, int32_t width, int32_t height, int32_t bitdepth, RECT* rect, uint32_t colorkey, HSTRANS* handle) {
    RECT exclrect;
    RECT* exclrectptr = rect;
    if (rect) {
        exclrect = { rect->left, rect->top, rect->right + 1, rect->bottom + 1 };
        exclrectptr = &exclrect;
    }
    return STransCreateE(bits, width, height, bitdepth, exclrectptr, colorkey, handle);
}

int32_t STORMAPI STransCreateMaskE(uint8_t* bits, int32_t width, int32_t height, int32_t bitdepth, RECT* rect, uint32_t colorkey, HSTRANS* handle) {
    if (handle) *handle = nullptr;

    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(bits);
    STORM_VALIDATE(width);
    STORM_VALIDATE(height);
    STORM_VALIDATE(handle);
    STORM_VALIDATE_END;

    return InternalCreateTransparency(bits, width, height, bitdepth, rect, colorkey, 1, handle);
}

int32_t STORMAPI STransCreateMaskI(uint8_t* bits, int32_t width, int32_t height, int32_t bitdepth, RECT* rect, uint32_t colorkey, HSTRANS* handle) {
    RECT exclrect;
    RECT* exclrectptr = rect;
    if (rect) {
        exclrect = { rect->left, rect->top, rect->right + 1, rect->bottom + 1 };
        exclrectptr = &exclrect;
    }
    return STransCreateMaskE(bits, width, height, bitdepth, exclrectptr, colorkey, handle);
}

int32_t STORMAPI STransDelete(HSTRANS handle) {
    if (handle->data) {
        if (s_savedata) {
            STORM_FREE(s_savedata);
        }

        s_savedata = handle->data;
        s_savedataalloc = handle->dataalloc;

        handle->data = nullptr;
        handle->dataalloc = 0;
    }

    s_translist.DeleteNode(handle);
    return 1;
}

int32_t STORMAPI STransDestroy() {
    while (TRANS* curr = s_translist.Head()) {
        //SErrReportResourceLeak("HSTRANS");
        STransDelete(curr);
    }

    if (s_dirtyoffset) {
        STORM_FREE(s_dirtyoffset);
        s_dirtyoffset = nullptr;
    }

    if (s_savedata) {
        STORM_FREE(s_savedata);
        s_savedata = nullptr;
        s_savedataalloc = 0;
    }
    return 1;
}

int32_t STORMAPI STransDuplicate(HSTRANS source, HSTRANS* handle) {
    if (handle) {
        *handle = nullptr;
    }

    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(source);
    STORM_VALIDATE(handle);
    STORM_VALIDATE_END;

    uint8_t* data = static_cast<uint8_t*>(STORM_ALLOC(source->dataalloc));
    std::memcpy(data, source->data, source->databytes);

    HSTRANS newtrans = CreateTransparencyRecord(source);
    newtrans->data = data;

    *handle = newtrans;
    return 1;
}

int32_t STORMAPI STransInvertMask(HSTRANS sourcemask, HSTRANS* handle) {
    if (handle) {
        *handle = nullptr;
    }

    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(sourcemask);
    STORM_VALIDATE(handle);
    STORM_VALIDATE_END;

    uint32_t dataalloc = sourcemask->databytes - sourcemask->instructionoffset + 2 * sourcemask->height;
    uint8_t* data = static_cast<uint8_t*>(STORM_ALLOC(dataalloc));
    uint32_t bytes = 0;

    uint8_t* source = &sourcemask->data[sourcemask->instructionoffset];
    uint8_t* dest = data;

    for (int32_t cy = sourcemask->height; cy--;) {
        uint8_t copybytes = 0, skipbytes = 0;
        do {
            skipbytes = *source++;
            if (copybytes || skipbytes) {
                *dest++ = copybytes;
                *dest++ = skipbytes;
                bytes += 2;
            }
            copybytes = *source++;
        } while (copybytes || skipbytes);

        *dest++ = 0;
        *dest++ = 0;
        bytes += 2;
    }

    HSTRANS newptr = CreateTransparencyRecord(sourcemask);
    newptr->data = data;
    newptr->dataalloc = dataalloc;
    newptr->databytes = bytes;
    newptr->instructionoffset = 0;

    *handle = newptr;
    return 1;
}

int32_t STORMAPI STransIsPixelInMask(HSTRANS mask, int32_t offsetx, int32_t offsety) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(mask);
    STORM_VALIDATE_END;

    if (offsetx < 0 || offsety < 0 || offsetx >= mask->width || offsety >= mask->height) {
        return 0;
    }

    uint8_t copybytes, skipbytes;
    uint8_t* instptr = &mask->data[mask->instructionoffset];
    while(offsety--) {
        do {
            copybytes = *instptr++;
            skipbytes = *instptr++;
        } while (copybytes || skipbytes);
    }

    copybytes = *instptr++;
    skipbytes = *instptr++;
    while(copybytes || skipbytes) {
        if (copybytes > offsetx) return 1;
        offsetx -= copybytes;

        if (skipbytes > offsetx) return 0;
        offsetx -= skipbytes;

        copybytes = *instptr++;
        skipbytes = *instptr++;
    }
    return 0;
}

int32_t STORMAPI STransSetDirtyArrayInfo(int32_t screencx, int32_t screency, int32_t cellcx, int32_t cellcy) {
    if (s_dirtyoffset) {
        STORM_FREE(s_dirtyoffset);
        s_dirtyoffset = nullptr;
    }

    if (!DetermineShift(cellcx, &s_dirtyxshift)) {
        return 0;
    }

    if (!DetermineShift(cellcy, &s_dirtyyshift)) {
        return 0;
    }

    s_dirtysize = {
        (screencx + (1 << s_dirtyxshift) - 1) >> s_dirtyxshift,
        (screency + (1 << s_dirtyyshift) - 1) >> s_dirtyyshift,
    };

    s_dirtyxsize = cellcx;
    s_dirtyysize = cellcy;

    s_dirtyoffset = static_cast<uint32_t*>(STORM_ALLOC(sizeof(s_dirtyoffset[0]) * s_dirtysize.cy));

    uint32_t offset = 0;
    for (int32_t i = 0; i < s_dirtysize.cy; i++) {
        s_dirtyoffset[i] = offset;
        offset += s_dirtysize.cy;
    }
    return 1;
}

int32_t STORMAPI STransUpdateDirtyArray(uint8_t* dirtyarray, uint8_t dirtyvalue, int32_t destx, int32_t desty, HSTRANS transparency, int32_t tracecontour) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(dirtyarray);
    STORM_VALIDATE(dirtyvalue);
    STORM_VALIDATE(transparency);
    STORM_VALIDATE_END;

    if (!s_dirtyoffset) return 0;
    if (transparency->width <= 0 || transparency->height <= 0) return 0;
    if (tracecontour) return 0;

    int32_t lastx = (transparency->boundrect.right + destx) >> s_dirtyxshift;
    int32_t lasty = (transparency->boundrect.bottom + desty) >> s_dirtyyshift;
    int32_t firstx = (transparency->boundrect.left + destx) >> s_dirtyxshift;
    int32_t firsty = (transparency->boundrect.top + desty) >> s_dirtyyshift;

    for (int32_t y = desty; y < lasty; y++) {
        for (int32_t x = destx; x < lastx; x++) {
            dirtyarray[s_dirtyoffset[y] + x] |= dirtyvalue;
        }
    }
    return 1;
}
