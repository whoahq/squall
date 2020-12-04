#include "storm/Atomic.hpp"

#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>
#endif

int32_t SInterlockedDecrement(int32_t* ptr) {
#if defined(WHOA_SYSTEM_WIN)
    return InterlockedDecrement(ptr);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    return __sync_fetch_and_sub(ptr, 1) - 1;
#endif
}

int32_t SInterlockedIncrement(int32_t* ptr) {
#if defined(WHOA_SYSTEM_WIN)
    return InterlockedIncrement(ptr);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    return __sync_fetch_and_add(ptr, 1) + 1;
#endif
}
