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
