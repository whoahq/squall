#ifndef STORM_ERROR_HPP
#define STORM_ERROR_HPP

#include <cstdint>

#if defined(WHOA_SYSTEM_WIN)
// This is necessary to prevent issues
// with std::max() or std::min() when including this header file
#define NOMINMAX
#include <Windows.h>
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
#define ERROR_SUCCESS           0x0
#define ERROR_INVALID_PARAMETER 0x57
#endif

#if defined(NDEBUG)
#define STORM_ASSERT(x)                          \
    (void)0
#else
#define STORM_ASSERT(x)                          \
    if (!(x)) {                                  \
        SErrPrepareAppFatal(__FILE__, __LINE__); \
        SErrDisplayAppFatal(#x);                 \
    }                                            \
    (void)0
#endif

#define STORM_VALIDATE(x, y, ...)                \
    if (!(x)) {                                  \
        SErrSetLastError(y);                     \
        return __VA_ARGS__;                      \
    }                                            \
    (void)0

[[noreturn]] void SErrDisplayAppFatal(const char* format, ...);

int32_t SErrDisplayError(uint32_t errorcode, const char* filename, int32_t linenumber, const char* description, int32_t recoverable, uint32_t exitcode, uint32_t a7);

int32_t SErrDisplayErrorFmt(uint32_t errorcode, const char* filename, int32_t linenumber, int32_t recoverable, uint32_t exitcode, const char* format, ...);

void SErrPrepareAppFatal(const char* filename, int32_t linenumber);

void SErrSetLastError(uint32_t errorcode);

uint32_t SErrGetLastError();

#endif
