#ifndef STORM_ERROR_HPP
#define STORM_ERROR_HPP

#include "storm/error/Macros.hpp"
#include "storm/error/Types.hpp"
#include <cstdint>

#if defined(WHOA_SYSTEM_WIN)
#include <WinError.h>
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
#define ERROR_SUCCESS           0x0
#define ERROR_INVALID_HANDLE    0x6
#define ERROR_NOT_ENOUGH_MEMORY 0x8
#define ERROR_INVALID_PARAMETER 0x57
#endif

[[noreturn]] void SErrDisplayAppFatal(const char* format, ...);

int32_t SErrDisplayError(uint32_t errorcode, const char* filename, int32_t linenumber, const char* description, int32_t recoverable, uint32_t exitcode, uint32_t a7);

int32_t SErrDisplayErrorFmt(uint32_t errorcode, const char* filename, int32_t linenumber, int32_t recoverable, uint32_t exitcode, const char* format, ...);

void SErrPrepareAppFatal(const char* filename, int32_t linenumber);

void SErrSetLastError(uint32_t errorcode);

uint32_t SErrGetLastError();

void SErrSuppressErrors(uint32_t suppress);

#endif
