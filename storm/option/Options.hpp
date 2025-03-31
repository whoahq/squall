#ifndef STORM_OPTION_OPTIONS_HPP
#define STORM_OPTION_OPTIONS_HPP

#include <cstdint>

struct STORMOPTIONS {
    int32_t  smemleaksilentwarning;
    int32_t  serrleaksilentwarning;
    uint32_t wavechunksize;
    int32_t  opt0C;
    int32_t  opt10;
    int32_t  opt14;
    int32_t  opt18;
    int32_t  opt1C;
    int32_t  sregunicode;
    int32_t  opt24;
    int32_t  opt28;
};

extern STORMOPTIONS g_opt;

extern uint32_t s_alloccount;
extern uint32_t s_freecount;

#endif
