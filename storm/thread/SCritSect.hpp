#ifndef STORM_THREAD_S_CRIT_SECT_HPP
#define STORM_THREAD_S_CRIT_SECT_HPP

#if defined(PLATFORM_MAC) || defined(PLATFORM_LINUX)
#include <pthread.h>
#endif

class SCritSect {
    public:
    // Member variables
#if defined(PLATFORM_WIN)
    CRITICAL_SECTION m_opaqueData;
#endif

#if defined(PLATFORM_MAC) || defined(PLATFORM_LINUX)
    pthread_mutex_t m_mutex;
#endif

    // Member functions
    void Enter(void);
    void Leave(void);
};

#endif
