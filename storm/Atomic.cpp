#include "storm/Atomic.hpp"

#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>
#endif

ATOMIC32 SInterlockedDecrement(ATOMIC32* ptr) {
#if defined(WHOA_SYSTEM_WIN)
    return InterlockedDecrement(ptr);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_WEB)
    return __sync_fetch_and_sub(ptr, 1) - 1;
#endif
}

ATOMIC32 SInterlockedIncrement(ATOMIC32* ptr) {
#if defined(WHOA_SYSTEM_WIN)
    return InterlockedIncrement(ptr);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_WEB)
    return __sync_fetch_and_add(ptr, 1) + 1;
#endif
}
