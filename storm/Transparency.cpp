#include "storm/Transparency.hpp"
#include "storm/List.hpp"
#include "storm/Error.hpp"

#include <algorithm>
#include <climits>
#include <cstring>

#define COPY 0
#define SKIP 1
#define NUM_OPS 2

#define MAXSPANLENGTH 0xFC
#define TRANS_CHUNKSIZE 4096

struct TRANS : TSLinkedNode<TRANS> {
    uint8_t* data;
    uint32_t dataalloc;
    uint32_t databytes;
    uint32_t instructionoffset;
    int32_t width;
    int32_t height;
    RECT boundrect;
};

// Replacing windows struct
struct WHOA_SIZE {
    int32_t cx, cy;
};

struct BUFFER {
    uint8_t* data;
    uint32_t bytesalloc;
    uint32_t bytesused;
    uint32_t chunksize;
};

// Defaults from SC 1.17's storm.dll
WHOA_SIZE s_dirtysize = { 40, 30 };
int32_t s_dirtyxshift = 4;
int32_t s_dirtyxsize = 16;
int32_t s_dirtyyshift = 4;
int32_t s_dirtyysize = 16;

uint32_t* s_dirtyoffset;
uint8_t* s_savedata;
uint32_t s_savedataalloc;
STORM_LIST(TRANS) s_translist;

void BufferCreate(BUFFER* buffer) {
    buffer->chunksize = TRANS_CHUNKSIZE;
    buffer->bytesused = 0;
    if (s_savedata) {
        buffer->data = s_savedata;
        buffer->bytesalloc = s_savedataalloc;
        s_savedata = nullptr;
        s_savedataalloc = 0;
    }
    else {
        buffer->bytesalloc = TRANS_CHUNKSIZE;
        buffer->data = static_cast<uint8_t*>(STORM_ALLOC(buffer->bytesalloc));
    }
}

void BufferReserve(BUFFER* buffer, uint32_t bytes, uint8_t** adjptr1, uint8_t** adjptr2) {
    uint32_t newalloc = buffer->bytesalloc;
    while (newalloc < bytes + buffer->bytesused) {
        newalloc += buffer->chunksize;
    }

    if (newalloc != buffer->bytesalloc) {
        uint8_t* newdata = static_cast<uint8_t*>(STORM_ALLOC(newalloc));
        SMemCopy(newdata, buffer->data, buffer->bytesused);
        STORM_FREE(buffer->data);

        if (adjptr1 && *adjptr1) {
            *adjptr1 = &newdata[*adjptr1 - buffer->data];
        }
        if (adjptr2 && *adjptr2) {
            *adjptr2 = &newdata[*adjptr2 - buffer->data];
        }
        buffer->bytesalloc = newalloc;
        buffer->data = newdata;
    }
}

void ConvertBitmapToTransparency(uint8_t* bits, int32_t width, int32_t height, int32_t bitdepth, RECT* rect, RECT* boundrect, uint8_t colorkey, int32_t maskonly, uint8_t* data, uint32_t* databytes, uint32_t* instructionoffset) {
    uint32_t size = 0;
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
        bool copymode = true;

        for (int32_t x = cx; x--;) {
            bool output = false;
            bool expectedcopymode = *source++ != colorkey;
            if (expectedcopymode == copymode) {
                if (copymode) {
                    copybytes++;
                }
                else {
                    skipbytes++;
                }
            }
            else if (copymode) {
                copymode = false;
                skipbytes++;
            }
            else {
                output = true;
                source--;
                x++;
            }

            if (output || copybytes == MAXSPANLENGTH || skipbytes == MAXSPANLENGTH || x == 0) {
                size += 2;
                if (data) {
                    *data++ = copybytes;
                    *data++ = skipbytes;
                }
                copybytes = 0;
                skipbytes = 0;
                copymode = true;
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
    if (colorref & STRANS_COLORKEY_VALID) {
        return colorref & STRANS_COLORKEY_MASK;
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
    for (int32_t y = 0; y < trans->height; y++) {
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

int32_t STORMAPI STransCombineMasks(HSTRANS basemask, HSTRANS secondmask, int32_t offsetx, int32_t offsety, uint32_t flags, HSTRANS* handle) {
    if (handle) *handle = nullptr;

    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(basemask);
    STORM_VALIDATE(secondmask);
    STORM_VALIDATE(handle);
    STORM_VALIDATE_END;

    bool intersect = (flags & STRANS_CF_INTERSECT) != 0;
    bool invertsecond = (flags & STRANS_CF_INVERTSECOND) != 0;

    bool usespan[NUM_OPS][NUM_OPS][NUM_OPS];
    for (int32_t spantype0 = 0; spantype0 < NUM_OPS; spantype0++) {
        for (int32_t spantypedest = 0; spantypedest < NUM_OPS; spantypedest++) {
            if (intersect) {
                usespan[spantype0][spantypedest][invertsecond ? SKIP : COPY] = (spantype0 == spantypedest);
                usespan[spantype0][spantypedest][invertsecond ? COPY : SKIP] = (spantypedest == SKIP);
            }
            else {
                usespan[spantype0][spantypedest][invertsecond ? SKIP : COPY] = (spantypedest == COPY);
                usespan[spantype0][spantypedest][invertsecond ? COPY : SKIP] = (spantype0 == spantypedest);
            }
        }
    }

    uint8_t* baseptr = &basemask->data[basemask->instructionoffset];
    uint8_t* secondptr = &secondmask->data[secondmask->instructionoffset];

    uint8_t copybytes, skipbytes;

    for (int32_t i = offsety; i < 0; i++) {
        do {
            copybytes = *secondptr++;
            skipbytes = *secondptr++;
        } while (copybytes || skipbytes);
    }

    BUFFER buffer;
    BufferCreate(&buffer);
    uint8_t* dest = buffer.data;
    for (int32_t line = 0; line < basemask->height; line++) {
        BufferReserve(&buffer, 2 * (basemask->width + 1), &dest, nullptr);

        if (line >= offsety && line <= offsety + secondmask->height - 1) {
            int32_t span[2][NUM_OPS] = {};
            span[1][SKIP] = std::max(0, offsetx);
            bool hitend = false;

            if (offsetx < 0) {
                int32_t bytesleft = -offsetx;
                while (bytesleft) {
                    span[1][COPY] = *secondptr++;
                    span[1][SKIP] = *secondptr++;

                    if (span[1][COPY] == 0 && span[1][SKIP] == 0) {
                        span[1][SKIP] = INT_MAX;
                        hitend = true;
                        break;
                    }

                    for (int32_t adjustremaining = 0; adjustremaining < NUM_OPS; adjustremaining++) {
                        int32_t adjustment = std::min(bytesleft, span[1][adjustremaining]);
                        span[1][adjustremaining] -= adjustment;
                        bytesleft -= adjustment;
                    }
                }
            }

            while(1) {
                span[0][COPY] = *baseptr++;
                span[0][SKIP] = *baseptr++;
                if (span[0][COPY] == 0 && span[0][SKIP] == 0) break;

                for (int32_t j = 0; j < NUM_OPS; j++) {
                    while(span[0][j]) {
                        if (span[1][COPY] == 0 && span[1][SKIP] == 0) {
                            span[1][COPY] = *secondptr++;
                            span[1][SKIP] = *secondptr++;
                            if (span[1][COPY] == 0 && span[1][SKIP] == 0) {
                                span[1][SKIP] = INT_MAX;
                                hitend = true;
                            }
                        }

                        uint8_t inst[NUM_OPS];
                        for (int32_t k = 0; k < NUM_OPS; k++) {
                            int32_t spanlength = 0;
                            if (usespan[j][k][COPY]) {
                                spanlength = span[1][COPY];
                            }

                            if (usespan[j][k][SKIP] && (usespan[j][k][COPY] || !span[1][COPY])) {
                                spanlength += span[1][SKIP];
                            }

                            spanlength = std::min(span[0][j], spanlength);
                            inst[k] = spanlength;
                            span[0][j] -= spanlength;
                            for (int32_t m = 0; m < NUM_OPS; m++) {
                                int32_t v11 = std::min(span[1][m], spanlength);
                                span[1][m] -= v11;
                                spanlength -= v11;
                            }
                        }

                        if (inst[COPY] || inst[SKIP]) {
                            *dest++ = inst[COPY];
                            *dest++ = inst[SKIP];
                            buffer.bytesused += 2;
                        }
                    }
                }
            }

            *dest++ = 0;
            *dest++ = 0;
            buffer.bytesused += 2;

            if (!hitend) {
                do {
                    copybytes = *secondptr++;
                    skipbytes = *secondptr++;
                } while (copybytes || skipbytes);
            }
        }
        else if (intersect == invertsecond) {
            do {
                buffer.bytesused += 2;
                copybytes = *baseptr++;
                skipbytes = *baseptr++;

                *dest++ = copybytes;
                *dest++ = skipbytes;
            } while (copybytes || skipbytes);
        }
        else {
            int32_t width = basemask->width;
            while (width) {
                int32_t skipspan = std::min(width, MAXSPANLENGTH);
                *dest++ = 0;
                *dest++ = skipspan;
                buffer.bytesused += 2;

                width -= skipspan;
            }
            *dest++ = 0;
            *dest++ = 0;
            buffer.bytesused += 2;

            do {
                copybytes = *baseptr++;
                skipbytes = *baseptr++;
            } while (copybytes || skipbytes);
        }
    }

    HSTRANS newptr = CreateTransparencyRecord(basemask);
    newptr->data = buffer.data;
    newptr->dataalloc = buffer.bytesalloc;
    newptr->databytes = buffer.bytesused;
    newptr->instructionoffset = 0;
    *handle = newptr;
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
    if (handle) *handle = nullptr;
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(source);
    STORM_VALIDATE(handle);
    STORM_VALIDATE_END;

    uint8_t* data = static_cast<uint8_t*>(STORM_ALLOC(source->dataalloc));
    SMemCopy(data, source->data, source->databytes);

    HSTRANS newtrans = CreateTransparencyRecord(source);
    newtrans->data = data;

    *handle = newtrans;
    return 1;
}

int32_t STORMAPI STransIntersectDirtyArray(HSTRANS sourcemask, uint8_t* dirtyarray, uint8_t dirtyarraymask, HSTRANS* handle) {
    if (handle) *handle = nullptr;
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(sourcemask);
    STORM_VALIDATE(dirtyarray);
    STORM_VALIDATE(dirtyarraymask);
    STORM_VALIDATE(handle);
    STORM_VALIDATE_END;

    if (s_dirtyoffset == 0) return 0;

    BUFFER buffer;
    BufferCreate(&buffer);

    uint8_t* source = &sourcemask->data[sourcemask->instructionoffset];
    uint8_t* dest = buffer.data;
    uint8_t* lastsource = source;
    uint8_t* lastdest = dest;

    for (int32_t y = 0; y < sourcemask->height; y++) {
        BufferReserve(&buffer, 2 * (sourcemask->width + 1), &dest, &lastdest);

        // wtf?
        if (((s_dirtyysize - 1) & y) != 0 && !memcmp(source, lastsource, source - lastsource)) {
            ptrdiff_t sourcebytes = source - lastsource;
            ptrdiff_t destbytes = dest - lastdest;
            memcpy(dest, lastdest, dest - lastdest);

            lastsource = source;
            lastdest = dest;
            source += sourcebytes;
            dest += destbytes;
            buffer.bytesused += static_cast<uint32_t>(destbytes);
        }
        else {
            uint8_t copybytes, skipbytes;

            lastsource = source;
            lastdest = dest;
            uint8_t* dirty = &dirtyarray[s_dirtyoffset[y >> s_dirtyyshift]];
            uint32_t xoffset = 0;
            do {
                copybytes = *source++;
                skipbytes = *source++;

                if (copybytes) {
                    uint8_t bytesleft = copybytes;
                    uint8_t length = 0;
                    bool copymode = true;

                    while (bytesleft != 0 || length != 0) {
                        if (bytesleft != 0 && ((dirtyarraymask & *dirty) != 0) == copymode) {
                            uint8_t cellleft = s_dirtyxsize - xoffset;
                            if (bytesleft >= cellleft) {
                                length += cellleft;
                                bytesleft -= cellleft;
                                xoffset = 0;
                                dirty++;
                            }
                            else {
                                length += bytesleft;
                                xoffset += bytesleft;
                                bytesleft = 0;
                            }
                        }
                        else {
                            *dest++ = length;
                            buffer.bytesused++;
                            length = 0;
                            copymode = !copymode;
                        }
                    }

                    if (skipbytes || !copymode) {
                        if (copymode) {
                            *dest++ = 0;
                            buffer.bytesused++;
                        }
                        *dest++ = skipbytes;
                        buffer.bytesused++;
                    }
                }
                else {
                    *dest++ = 0;
                    *dest++ = skipbytes;
                    buffer.bytesused += 2;
                }

                if (skipbytes) {
                    xoffset += skipbytes;
                    dirty += xoffset >> s_dirtyxshift;
                    xoffset &= s_dirtyxsize - 1;
                }

            } while (copybytes || skipbytes);
        }
    }

    HSTRANS newptr = CreateTransparencyRecord(sourcemask);
    newptr->data = buffer.data;
    newptr->dataalloc = buffer.bytesalloc;
    newptr->databytes = buffer.bytesused;
    newptr->instructionoffset = 0;
    *handle = newptr;
    return 1;
}

int32_t STORMAPI STransInvertMask(HSTRANS sourcemask, HSTRANS* handle) {
    if (handle) *handle = nullptr;
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
        offset += s_dirtysize.cx;
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

    for (int32_t y = firsty; y < lasty; y++) {
        for (int32_t x = firstx; x < lastx; x++) {
            dirtyarray[s_dirtyoffset[y] + x] |= dirtyvalue;
        }
    }
    return 1;
}
