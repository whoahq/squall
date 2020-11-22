#include "storm/Unicode.hpp"

uint32_t SUniSGetUTF8(const uint8_t* strptr, int32_t* chars) {
    if (chars) {
        *chars = 0;
    }

    if (!strptr || !*strptr) {
        return -1;
    }

    if (chars) {
        *chars = *chars + 1;
    }

    uint32_t value = *strptr;
    int32_t extra;

    if ((value & 0xFE) == 0xFC) {
        value &= 0x01;
        extra = 5;
    } else if ((value & 0xFC) == 0xF8) {
        value &= 0x03;
        extra = 4;
    } else if ((value & 0xF8) == 0xF0) {
        value &= 0x07;
        extra = 3;
    } else if ((value & 0xF0) == 0xE0) {
        value &= 0x0F;
        extra = 2;
    } else if ((value & 0xE0) == 0xC0) {
        value &= 0x1F;
        extra = 1;
    } else if ((value & 0x80) == 0x00) {
        return value;
    } else {
        return 0x80000000;
    }

    const uint8_t* next = strptr + 1;

    for (int32_t i = 0; i < extra; i++, next++) {
        if (!*next) {
            return -1;
        }

        if (chars) {
            *chars = *chars + 1;
        }

        if ((*next & 0xC0) != 0x80) {
            return 0x80000000;
        }

        value = (value << 6) | (*next & 0x3F);
    }

    return value;
}

void SUniSPutUTF8(uint32_t c, char* strptr) {
    if (!strptr) {
        return;
    }

    auto curstr = strptr;
    auto v3 = c;
    char v4, v5, v6, v7;

    if (c >= 0x80) {
        if (c >= 0x800) {
            if (c >= 0x10000) {
                if (c >= 0x200000) {
                    if (c >= 0x400000) {
                        if (c >= 0x80000000) {
                            *curstr = 0;
                            return;
                        }

                        *strptr = (c >> 30) | 0xFC;
                        curstr = strptr + 1;
                        // TODO this seems likely to be bitwise right shift 24, not 8
                        v7 = ((c >> 8) & 0x3F) | 0x80;
                    } else {
                        // TODO this seems likely to be bitwise right shift 24, not 8
                        v7 = (c >> 8) | 0xF8;
                    }

                    *curstr++ = v7;
                    v6 = ((c >> 18) & 0x3F) | 0x80;
                } else {
                    v6 = (c >> 18) | 0xF0;
                }

                *curstr++ = v6;
                v5 = ((c >> 12) & 0x3F) | 0x80;
            } else {
                v5 = (c >> 12) | 0xE0;
            }

            *curstr++ = v5;
            v4 = ((c >> 6) & 0x3F) | 0x80;
        } else {
            v4 = (c >> 6) | 0xC0;
        }

        *curstr++ = v4;
        v3 = (c & 0x3F) | 0x80;
    }

    *curstr++ = v3;
    *curstr = '\0';
}
