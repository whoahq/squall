#ifndef STORM_REGION_TYPES_HPP
#define STORM_REGION_TYPES_HPP

#include "storm/Handle.hpp"
#include <cstdint>
#include <limits>

DECLARE_STORM_HANDLE(HSRGN);
DECLARE_STORM_HANDLE(HLOCKEDRGN);

struct RECTF {
    float left;
    float bottom;
    float right;
    float top;
};

// NOTE: This is a made up type to replace WINAPI's RECT.
// Implementation requires RECT and RECTF to be the same size but WINAPI uses `long`,
// which can change size in 64-bit.
struct RECTI {
    int32_t left, top, right, bottom;
};

// Combine modes
#define SRGN_AND 1
#define SRGN_OR 2
#define SRGN_XOR 3
#define SRGN_DIFF 4
#define SRGN_COPY 5
#define SRGN_PARAMONLY 6

#endif
