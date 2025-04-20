#ifndef STORM_REGION_RGN_HPP
#define STORM_REGION_RGN_HPP

#include "storm/region/Types.hpp"
#include "storm/Array.hpp"
#include "storm/Hash.hpp"
#include <cstdint>

// region flgs
#define SF_NONE      0x00
#define SF_ADDING    0x00000001
#define SF_OVERLAPS  0x00000002
#define SF_TEMPMASK  0x00000003
#define SF_PARAMONLY 0x00010000

struct SOURCE {
    RECTF rect;
    void* param;
    int32_t sequence;
    uint32_t flags;
};

struct FOUNDPARAM {
    void* param;
    int32_t sequence;
};

class RGN : public TSHashObject<RGN, HASHKEY_NONE> {
    public:
    // Member variables
    TSGrowableArray<SOURCE> source;
    TSGrowableArray<RECTF> combined;
    TSGrowableArray<FOUNDPARAM> foundparams;
    RECTF foundparamsrect;
    int32_t sequence;
    int32_t dirty;
};

#endif
