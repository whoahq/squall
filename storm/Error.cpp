#include "storm/Error.hpp"
#include "storm/thread/SCritSect.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#if defined(WHOA_SYSTEM_WIN)
#include <Windows.h>
#define StormGetThreadId() GetCurrentThreadId()
#else
#include <unistd.h>
#define StormGetThreadId() getpid()
#endif

struct APPFATINFO {
    const char* filename;
    int32_t linenumber;
    uint32_t threadId;
};

static APPFATINFO s_appFatInfo;

SCritSect s_critsect;

static uint32_t s_lasterror = ERROR_SUCCESS;
static int32_t s_suppress;
static int32_t s_displaying;

[[noreturn]] void SErrDisplayAppFatalCustomV(uint32_t errorcode, const char* format, va_list va) {
    const char* filename = nullptr;
    int32_t linenumber = 0;
    s_critsect.Enter();
    if (s_appFatInfo.threadId == StormGetThreadId()) {
        filename = s_appFatInfo.filename;
        linenumber = s_appFatInfo.linenumber;
        s_appFatInfo = {};
    }
    s_critsect.Leave();

    char buffer[2048];
    std::vsnprintf(buffer, sizeof(buffer) - 1, format, va);
    buffer[sizeof(buffer) - 1] = '\0';

#ifdef WHOA_DISPLAY_ERR_EXTRA_ARG
    SErrDisplayError(errorcode, filename, linenumber, buffer, 0, EXIT_FAILURE, 2);
#else
    SErrDisplayError(errorcode, filename, linenumber, buffer, 0, EXIT_FAILURE);
#endif
    std::exit(EXIT_FAILURE);
}

[[noreturn]] void STORMCDECL SErrDisplayAppFatal(const char* format, ...) {
    va_list args;
    va_start(args, format);
    SErrDisplayAppFatalCustomV(STORM_ERROR_FATAL_CONDITION, format, args);
}

#ifdef WHOA_DISPLAY_ERR_EXTRA_ARG
int32_t STORMAPI SErrDisplayError(uint32_t errorcode, const char* filename, int32_t linenumber, const char* description, int32_t recoverable, uint32_t exitcode, uint32_t a7) {
#else
int32_t STORMAPI SErrDisplayError(uint32_t errorcode, const char* filename, int32_t linenumber, const char* description, int32_t recoverable, uint32_t exitcode) {
#endif
    if (s_suppress || s_displaying) return 0;
    s_displaying = 1;

    // TODO

    printf("\n=========================================================\n");

    if (linenumber == SERR_LINECODE_EXCEPTION) {
        printf("Exception Raised!\n\n");

        printf(" App:         %s\n", "GenericBlizzardApp");

        if (errorcode != STORM_ERROR_ASSERTION) {
            printf(" Error Code:  0x%08X\n", errorcode);
        }

        // TODO output time

        printf(" Error:       %s\n\n", description);
    } else {
        printf("Assertion Failed!\n\n");

        printf(" App:         %s\n", "GenericBlizzardApp");
        printf(" File:        %s\n", filename);
        printf(" Line:        %d\n", linenumber);

        if (errorcode != STORM_ERROR_ASSERTION) {
            printf(" Error Code:  0x%08X\n", errorcode);
        }

        // TODO output time

        printf(" Assertion:   %s\n", description);
    }

    s_displaying = 0;
    if (!recoverable) {
        SErrSuppressErrors(1);
        std::exit(exitcode);
    }
    return 1;
}

int32_t STORMCDECL SErrDisplayErrorFmt(uint32_t errorcode, const char* filename, int32_t linenumber, int32_t recoverable, uint32_t exitcode, const char* format, ...) {
    char buffer[2048];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer) - 1, format, args);
    buffer[sizeof(buffer) - 1] = '\0';
    va_end(args);

    return SErrDisplayError(errorcode, filename, linenumber, buffer, recoverable, exitcode);
}

int32_t STORMAPI SErrGetErrorStr(uint32_t errorcode, char* buffer, uint32_t bufferchars) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(buffer);
    STORM_VALIDATE(bufferchars);
    STORM_VALIDATE_END;

    buffer[0] = '\0';
#if defined(WHOA_SYSTEM_WIN)
    // half-measure
    return FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, errorcode, LANG_USER_DEFAULT, buffer, bufferchars, nullptr);
#else
    return 0;
#endif
}

uint32_t STORMAPI SErrGetLastError() {
    return s_lasterror;
}

int32_t STORMAPI SErrIsDisplayingError() {
    return s_displaying;
}

void STORMAPI SErrPrepareAppFatal(const char* filename, int32_t linenumber) {
    s_critsect.Enter();
    s_appFatInfo.filename = filename;
    s_appFatInfo.linenumber = linenumber;
    s_appFatInfo.threadId = StormGetThreadId();
    s_critsect.Leave();
}

void STORMAPI SErrSetLastError(uint32_t errorcode) {
    s_lasterror = errorcode;
#if defined(WHOA_SYSTEM_WIN)
    SetLastError(errorcode);
#endif
}

void STORMAPI SErrSuppressErrors(int32_t suppress) {
    s_suppress = suppress;
}
