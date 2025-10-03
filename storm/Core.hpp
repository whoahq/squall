#ifndef STORM_CORE_HPP
#define STORM_CORE_HPP

#include <cstdint>

#ifndef STORMAPI
#if defined(_MSC_VER)
    #define STORMAPI __stdcall
    #define STORMCDECL __cdecl
#else
    #define STORMAPI
    #define STORMCDECL
#endif
#endif

int32_t STORMAPI StormDestroy();

#endif
