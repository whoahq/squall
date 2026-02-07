#ifndef STORM_THREAD_S_CRIT_SECT_HPP
#define STORM_THREAD_S_CRIT_SECT_HPP

#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_WEB)
#include <pthread.h>
#endif

class SCritSect {
    public:
    // Member variables
#if defined(WHOA_SYSTEM_WIN)
    CRITICAL_SECTION m_opaqueData;
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_WEB)
    pthread_mutex_t m_mutex;
#endif

    // Member functions
    SCritSect();
    ~SCritSect();
    void Enter();
    void Leave();
};

#endif
