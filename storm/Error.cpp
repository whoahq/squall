#include "storm/Error.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <string>

#if defined(WHOA_SYSTEM_WIN)
#include <Windows.h>
#endif

static uint32_t s_lasterror = ERROR_SUCCESS;

[[noreturn]] void SErrDisplayAppFatal(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    printf("\n");
    va_end(args);

    exit(EXIT_FAILURE);
}

int32_t SErrDisplayError(uint32_t errorcode, const char* filename, int32_t linenumber, const char* description, int32_t recoverable, uint32_t exitcode, uint32_t a7) {
    // TODO

#if defined(WHOA_SYSTEM_WIN)
    #define S_ERR_MAX 1024

    char buffer[S_ERR_MAX];
    memset(buffer, 0, S_ERR_MAX);
    int32_t offset = 0;

    #define S_ERR_DISPLAY(...) \
        if (offset < S_ERR_MAX) { \
            offset += std::snprintf(&buffer[offset], S_ERR_MAX-offset, __VA_ARGS__); \
        }
#elif
    #define S_ERR_DISPLAY printf

    S_ERR_DISPLAY("\n=========================================================\n");
#endif

    if (linenumber == -5) {
        S_ERR_DISPLAY("Exception Raised!\n\n");

        S_ERR_DISPLAY(" App:         %s\n", "GenericBlizzardApp");

        if (errorcode != 0x85100000) {
            S_ERR_DISPLAY(" Error Code:  0x%08X\n", errorcode);
        }

        // TODO output time

        S_ERR_DISPLAY(" Error:       %s\n\n", description);
    } else {
        S_ERR_DISPLAY("Assertion Failed!\n\n");

        S_ERR_DISPLAY(" App:         %s\n", "GenericBlizzardApp");
        S_ERR_DISPLAY(" File:        %s\n", filename);
        S_ERR_DISPLAY(" Line:        %d\n", linenumber);

        if (errorcode != 0x85100000) {
            S_ERR_DISPLAY(" Error Code:  0x%08X\n", errorcode);
        }

        // TODO output time

        S_ERR_DISPLAY(" Assertion:   %s\n", description);
    }

#if defined(WHOA_SYSTEM_WIN)
    // Display message in Visual Studio
    OutputDebugStringA(buffer);

    S_ERR_DISPLAY("\n Ctrl-C to copy error to clipboard\n");

    const char* caption = "Fatal Error";
    if (recoverable) {
        caption = "Error";
    }

    MessageBoxA(nullptr, buffer, caption, MB_ICONERROR);

    #undef S_ERR_MAX
#endif

#undef S_ERR_DISPLAY

    if (recoverable) {
        return 1;
    } else {
        exit(exitcode);
    }
}

int32_t SErrDisplayErrorFmt(uint32_t errorcode, const char* filename, int32_t linenumber, int32_t recoverable, uint32_t exitcode, const char* format, ...) {
    char buffer[2048];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer) - 1, format, args);
    buffer[sizeof(buffer) - 1] = '\0';
    va_end(args);

    return SErrDisplayError(errorcode, filename, linenumber, buffer, recoverable, exitcode, 1);
}

void SErrPrepareAppFatal(const char* filename, int32_t linenumber) {
    // TODO
}

void SErrSetLastError(uint32_t errorcode) {
    s_lasterror = errorcode;

    #if defined(WHOA_SYSTEM_WIN)
        SetLastError(errorcode);
    #endif
}

uint32_t SErrGetLastError() {
    return s_lasterror;
}
