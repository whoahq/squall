#ifndef STORM_ERROR_HPP
#define STORM_ERROR_HPP

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


#ifdef _DEBUG
#ifndef ASSERTIONS_ENABLED
#define ASSERTIONS_ENABLED
#endif
#endif


#if defined(ASSERTIONS_ENABLED)
#define STORM_ASSERT(x)                                                                     \
    if (!(x)) {                                                                             \
        SErrDisplayError(STORM_ERROR_ASSERTION, __FILE__, __LINE__, #x, 0, 1, 0x11111111);  \
    }

#define STORM_ASSERT_FATAL(x)                    \
    if (!(x)) {                                  \
        SErrPrepareAppFatal(__FILE__, __LINE__); \
        SErrDisplayAppFatal(#x);                 \
    }
#else
#define STORM_ASSERT(x)
#define STORM_ASSERT_FATAL(x)
#endif


#if defined(NDEBUG)
#define STORM_VALIDATE_BEGIN    { bool __storm_result = true
#define STORM_VALIDATE(x)       __storm_result &= !!(x); STORM_ASSERT(x)
#define STORM_VALIDATE_END                          \
    if (!__storm_result) {                          \
        SErrSetLastError(ERROR_INVALID_PARAMETER);  \
        return 0;                                   \
    } }
#define STORM_VALIDATE_END_VOID                     \
    if (!__storm_result) {                          \
        SErrSetLastError(ERROR_INVALID_PARAMETER);  \
        return;                                     \
    } }
#else
#define STORM_VALIDATE_BEGIN        {
#define STORM_VALIDATE(x) STORM_ASSERT_FATAL(x)
#define STORM_VALIDATE_END          }
#define STORM_VALIDATE_END_VOID     }
#endif


#define STORM_ERROR_ASSERTION                0x85100000
#define STORM_ERROR_BAD_ARGUMENT             0x85100065
#define STORM_ERROR_GAME_ALREADY_STARTED     0x85100066
#define STORM_ERROR_GAME_FULL                0x85100067
#define STORM_ERROR_GAME_NOT_FOUND           0x85100068
#define STORM_ERROR_GAME_TERMINATED          0x85100069
#define STORM_ERROR_INVALID_PLAYER           0x8510006A
#define STORM_ERROR_NO_MESSAGES_WAITING      0x8510006B
#define STORM_ERROR_NOT_ARCHIVE              0x8510006C
#define STORM_ERROR_NOT_ENOUGH_ARGUMENTS     0x8510006D
#define STORM_ERROR_NOT_IMPLEMENTED          0x8510006E
#define STORM_ERROR_NOT_IN_ARCHIVE           0x8510006F
#define STORM_ERROR_NOT_IN_GAME              0x85100070
#define STORM_ERROR_NOT_INITIALIZED          0x85100071
#define STORM_ERROR_NOT_PLAYING              0x85100072
#define STORM_ERROR_NOT_REGISTERED           0x85100073
#define STORM_ERROR_REQUIRES_CODEC           0x85100074
#define STORM_ERROR_REQUIRES_DDRAW           0x85100075
#define STORM_ERROR_REQUIRES_DSOUND          0x85100076
#define STORM_ERROR_REQUIRES_UPGRADE         0x85100077
#define STORM_ERROR_STILL_ACTIVE             0x85100078
#define STORM_ERROR_VERSION_MISMATCH         0x85100079
#define STORM_ERROR_MEMORY_ALREADY_FREED     0x8510007A
#define STORM_ERROR_MEMORY_CORRUPT           0x8510007B
#define STORM_ERROR_MEMORY_INVALID_BLOCK     0x8510007C
#define STORM_ERROR_MEMORY_MANAGER_INACTIVE  0x8510007D
#define STORM_ERROR_MEMORY_NEVER_RELEASED    0x8510007E
#define STORM_ERROR_HANDLE_NEVER_RELEASED    0x8510007F
#define STORM_ERROR_ACCESS_OUT_OF_BOUNDS     0x85100080
#define STORM_ERROR_MEMORY_NULL_POINTER      0x85100081
#define STORM_ERROR_CDKEY_MISMATCH           0x85100082
#define STORM_ERROR_DATA_FILE_CORRUPT        0x85100083
#define STORM_ERROR_FATAL_EXCEPTION          0x85100084
#define STORM_ERROR_GAME_TYPE_UNAVAILABLE    0x85100085
#define STORM_ERROR_FATAL_CONDITION          0x85100086


#define SERR_LINECODE_FUNCTION  -1
#define SERR_LINECODE_OBJECT    -2
#define SERR_LINECODE_HANDLE    -3
#define SERR_LINECODE_FILE      -4
#define SERR_LINECODE_EXCEPTION -5  // exception handler


[[noreturn]] void SErrDisplayAppFatal(const char* format, ...);

int32_t SErrDisplayError(uint32_t errorcode, const char* filename, int32_t linenumber, const char* description, int32_t recoverable, uint32_t exitcode, uint32_t a7);

int32_t SErrDisplayErrorFmt(uint32_t errorcode, const char* filename, int32_t linenumber, int32_t recoverable, uint32_t exitcode, const char* format, ...);

void SErrPrepareAppFatal(const char* filename, int32_t linenumber);

void SErrSetLastError(uint32_t errorcode);

uint32_t SErrGetLastError();

void SErrSuppressErrors(uint32_t suppress);

#endif
