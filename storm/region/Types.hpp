#ifndef STORM_REGION_TYPES_HPP
#define STORM_REGION_TYPES_HPP

#include "storm/Handle.hpp"
#include <cstdint>

DECLARE_HANDLE(HSRGN);

DECLARE_HANDLE(HLOCKEDRGN);

struct RECTF {
    float left;
    float bottom;
    float right;
    float top;
};

#endif
