#ifndef STORM_THREAD_C_CRIT_SECT_HPP
#define STORM_THREAD_C_CRIT_SECT_HPP

#include <cstdint>

#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_WEB)
#include <pthread.h>
#endif

class CCritSect {
    public:
    // Member functions
    CCritSect();
    ~CCritSect();
    void Enter();
    void Enter(int32_t forWriting);
    void Leave();
    void Leave(int32_t fromWriting);

    private:
    // Member variables
#if defined(WHOA_SYSTEM_WIN)
    CRITICAL_SECTION m_critsect;
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_WEB)
    pthread_mutex_t m_critsect;
#endif
};

#endif
