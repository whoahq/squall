#ifndef STORM_ERROR_HPP
#define STORM_ERROR_HPP

#include "storm/error/Macros.hpp"
#include "storm/error/Types.hpp"
#include "Core.hpp"

#if defined(WHOA_SYSTEM_WIN)
#include <WinError.h>
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
#define ERROR_SUCCESS           0x0
#define ERROR_INVALID_HANDLE    0x6
#define ERROR_NOT_ENOUGH_MEMORY 0x8
#define ERROR_INVALID_PARAMETER 0x57
#endif

[[noreturn]] void STORMCDECL SErrDisplayAppFatal(const char* format, ...);


#ifdef WHOA_DISPLAY_ERR_EXTRA_ARG
int32_t STORMAPI SErrDisplayError(uint32_t errorcode, const char* filename, int32_t linenumber, const char* description, int32_t recoverable, uint32_t exitcode, uint32_t a7 = 0x11111111);
#else
int32_t STORMAPI SErrDisplayError(uint32_t errorcode, const char* filename, int32_t linenumber, const char* description, int32_t recoverable, uint32_t exitcode);
#endif

int32_t STORMCDECL SErrDisplayErrorFmt(uint32_t errorcode, const char* filename, int32_t linenumber, int32_t recoverable, uint32_t exitcode, const char* format, ...);

int32_t STORMAPI SErrIsDisplayingError();

void STORMAPI SErrPrepareAppFatal(const char* filename, int32_t linenumber);

void STORMAPI SErrSetLastError(uint32_t errorcode);

uint32_t STORMAPI SErrGetLastError();

void STORMAPI SErrSuppressErrors(int32_t suppress);

#endif
