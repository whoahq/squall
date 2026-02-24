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

#if defined(WHOA_SYSTEM_WIN)
// Avoid bringing in all of Windows.h here
typedef struct HINSTANCE__* HINSTANCE;
#else
typedef void* HANDLE;
typedef void* LPOVERLAPPED;
typedef void* HINSTANCE;
#endif

int32_t STORMAPI StormDestroy();

HINSTANCE STORMAPI StormGetInstance();

#endif
