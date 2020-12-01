#ifndef STORM_THREAD_S_CRIT_SECT_HPP
#define STORM_THREAD_S_CRIT_SECT_HPP

#if defined(WHOA_PLATFORM_WIN)
#include <windows.h>
#endif

#if defined(WHOA_PLATFORM_MAC) || defined(WHOA_PLATFORM_LINUX)
#include <pthread.h>
#endif

class SCritSect {
    public:
    // Member variables
#if defined(WHOA_PLATFORM_WIN)
    CRITICAL_SECTION m_opaqueData;
#endif

#if defined(WHOA_PLATFORM_MAC) || defined(WHOA_PLATFORM_LINUX)
    pthread_mutex_t m_mutex;
#endif

    // Member functions
    void Enter(void);
    void Leave(void);
};

#endif
