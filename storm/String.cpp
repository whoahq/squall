#include "storm/String.hpp"
#include "storm/Error.hpp"

size_t SStrCopy(char* dest, const char* source, size_t destsize) {
    STORM_ASSERT(dest);
    STORM_ASSERT(source);

    char* destbuf = dest;

    if (destsize == 0x7FFFFFFF) {
        while (*source) {
            *destbuf = *source;

            ++destbuf;
            ++source;
        }
    } else {
        if (*source) {
            while (destbuf < &dest[destsize - 1]) {
                *destbuf = *source;

                ++destbuf;
                ++source;

                if (!*source) {
                    *destbuf = '\0';
                    return static_cast<size_t>(destbuf - dest);
                }
            }
        }
    }

    *destbuf = '\0';
    return static_cast<size_t>(destbuf - dest);
}
