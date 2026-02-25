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

#if !defined(WHOA_SYSTEM_WIN)
typedef void* HANDLE;
typedef void* LPOVERLAPPED;
#endif

#if defined(WHOA_DESTROY_HAS_RETVAL)
#define STORM_DESTROY_RETTYPE int32_t
#else
#define STORM_DESTROY_RETTYPE void
#endif

STORM_DESTROY_RETTYPE STORMAPI StormDestroy();

#endif
