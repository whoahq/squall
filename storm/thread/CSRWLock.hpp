#ifndef STORM_THREAD_CS_RW_LOCK_HPP
#define STORM_THREAD_CS_RW_LOCK_HPP

#include <cstdint>

#if defined(WHOA_PLATFORM_MAC) || defined(WHOA_PLATFORM_LINUX)
#include <pthread.h>
#endif

class CSRWLock {
    public:
    // Member variables
#ifdef WHOA_PLATFORM_WIN
    char m_opaqueData[12];
#endif

#if defined(WHOA_PLATFORM_MAC) || defined(WHOA_PLATFORM_LINUX)
    pthread_rwlock_t m_lock;
#endif

    // Member functions
    void Enter(int32_t forwriting);
    void Leave(int32_t fromwriting);
};

#endif
