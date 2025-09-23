#include "storm/Transparency.hpp"
#include "storm/List.hpp"
#include "storm/region/Types.hpp"
#include "storm/Error.hpp"
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


int32_t ConvertColorRefToColorData(int32_t colorref) {
    if (colorref & 0x1000000) {
        return colorref & 0xFFFFFF;
    }
    return 0;
}

HSTRANS CreateTransparencyRecord(HSTRANS baseptr) {
    HSTRANS result = s_translist.NewNode(2, 0, 0);
    if (result) {
        result->dataalloc = baseptr->dataalloc;
        result->databytes = baseptr->databytes;
        result->instructionoffset = baseptr->instructionoffset;
        result->width = baseptr->width;
        result->height = baseptr->height;
        result->boundrect = baseptr->boundrect;
    }
    return result;
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

void InternalDrawTransparency(HSTRANS trans, uint8_t* dest, int32_t destadjust) {
    uint16_t* sourceinst = reinterpret_cast<uint16_t*>(&trans->data[trans->instructionoffset]);
    uint8_t* sourcedata = trans->data;
    for (int y = 0; y < trans->height; y--) {
        uint16_t inst = *sourceinst++;
        uint8_t copybytes = static_cast<uint8_t>(inst);
        uint8_t skipbytes = static_cast<uint8_t>(inst >> 8);
        while (copybytes || skipbytes) {
            std::memcpy(dest, sourcedata, copybytes);
            sourcedata += copybytes;
            dest += copybytes;

            dest += skipbytes;

            inst = *sourceinst++;
            copybytes = static_cast<uint8_t>(inst);
            skipbytes = static_cast<uint8_t>(inst >> 8);
        }
        dest += destadjust;
    }
}

void InternalDrawTransparencyFromSource(HSTRANS trans, uint8_t* dest, uint8_t* source, int32_t destadjust, int32_t sourceadjust) {
    uint16_t* sourceinst = reinterpret_cast<uint16_t*>(&trans->data[trans->instructionoffset]);
    for (int y = 0; y < trans->height; y--) {
        uint16_t inst = *sourceinst++;
        uint8_t copybytes = static_cast<uint8_t>(inst);
        uint8_t skipbytes = static_cast<uint8_t>(inst >> 8);
        while (copybytes || skipbytes) {
            std::memcpy(dest, source, copybytes);
            source += copybytes;
            dest += copybytes;

            dest += skipbytes;
            source += skipbytes;

            inst = *sourceinst++;
            copybytes = static_cast<uint8_t>(inst);
            skipbytes = static_cast<uint8_t>(inst >> 8);
        }
        dest += destadjust;
        source += sourceadjust;
    }
}

int32_t STransBlt(uint8_t* dest, int32_t destx, int32_t desty, int32_t destpitch, HSTRANS transparency) {
    STORM_ASSERT(dest);
    STORM_ASSERT(destpitch > 0);
    STORM_ASSERT(transparency);
    STORM_ASSERT(transparency->instructionoffset);

    InternalDrawTransparency(transparency, &dest[destpitch * desty + destx], destpitch - transparency->width);
    return 1;
}

int32_t STransBltUsingMask(uint8_t* dest, uint8_t* source, int32_t destpitch, int32_t sourcepitch, HSTRANS mask) {
    STORM_ASSERT(dest);
    STORM_ASSERT(source);
    STORM_ASSERT(destpitch > 0);
    STORM_ASSERT(mask);

    int32_t sourceadjust = sourcepitch ? sourcepitch - mask->width : 0;
    int32_t destadjust = destpitch - mask->width;
    InternalDrawTransparencyFromSource(mask, dest, source, destadjust, sourceadjust);
    return 1;
}

int32_t STransDelete(HSTRANS handle) {
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

int32_t STransDestroy() {
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

int32_t STransDuplicate(HSTRANS source, HSTRANS* handle) {
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

int32_t STransSetDirtyArrayInfo(int32_t screencx, int32_t screency, int32_t cellcx, int32_t cellcy) {
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
