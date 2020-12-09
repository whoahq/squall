#include "storm/Error.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

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

    printf("\n=========================================================\n");

    if (linenumber == -5) {
        printf("Exception Raised!\n\n");

        printf(" App:         %s\n", "GenericBlizzardApp");

        if (errorcode != 0x85100000) {
            printf(" Error Code:  0x%08X\n", errorcode);
        }

        // TODO output time

        printf(" Error:       %s\n\n", description);
    } else {
        printf("Assertion Failed!\n\n");

        printf(" App:         %s\n", "GenericBlizzardApp");
        printf(" File:        %s\n", filename);
        printf(" Line:        %d\n", linenumber);

        if (errorcode != 0x85100000) {
            printf(" Error Code:  0x%08X\n", errorcode);
        }

        // TODO output time

        printf(" Assertion:   %s\n", description);
    }

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
    // TODO
}
