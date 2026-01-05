#include "storm/Error.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#if defined(WHOA_SYSTEM_WIN)
#include <Windows.h>
#endif

static uint32_t s_lasterror = ERROR_SUCCESS;
static int32_t s_suppress;
static int32_t s_displaying;

[[noreturn]] void STORMCDECL SErrDisplayAppFatal(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    printf("\n");
    va_end(args);

    exit(EXIT_FAILURE);
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

int32_t STORMAPI SErrIsDisplayingError() {
    return s_displaying;
}

void STORMAPI SErrPrepareAppFatal(const char* filename, int32_t linenumber) {
    // TODO
}

void STORMAPI SErrSetLastError(uint32_t errorcode) {
    s_lasterror = errorcode;
#if defined(WHOA_SYSTEM_WIN)
    SetLastError(errorcode);
#endif
}

uint32_t STORMAPI SErrGetLastError() {
    return s_lasterror;
}

void STORMAPI SErrSuppressErrors(int32_t suppress) {
    s_suppress = suppress;
}
