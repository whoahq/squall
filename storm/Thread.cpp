#include "storm/Thread.hpp"

#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
#include <pthread.h>
#endif

uintptr_t SGetCurrentThreadId() {
#if defined(WHOA_SYSTEM_WIN)
    return GetCurrentThreadId();
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    return reinterpret_cast<uintptr_t>(pthread_self());
#endif
}
