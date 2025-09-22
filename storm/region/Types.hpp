#ifndef STORM_REGION_TYPES_HPP
#define STORM_REGION_TYPES_HPP

#include "storm/Handle.hpp"
#include <cstdint>
#include <limits>

DECLARE_STORM_HANDLE(HSRGN);
DECLARE_STORM_HANDLE(HLOCKEDRGN);

#if defined(WHOA_RECT_USES_SCREEN_COORDINATES)
struct RECTF {
    float left, top, right, bottom;
};
#else
struct RECTF {
    float left, bottom, right, top;
};
#endif

#if defined(WHOA_SYSTEM_WIN)
// NOTE: WINAPI's RECT uses `long`.
#include <Windows.h>
#else
struct RECT {
    int32_t left, top, right, bottom;
};
#endif

// Combine modes
#define SRGN_AND 1
#define SRGN_OR 2
#define SRGN_XOR 3
#define SRGN_DIFF 4
#define SRGN_COPY 5
#define SRGN_PARAMONLY 6

#endif
