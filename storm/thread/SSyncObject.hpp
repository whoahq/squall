#ifndef STORM_THREAD_S_SYNC_OBJECT_HPP
#define STORM_THREAD_S_SYNC_OBJECT_HPP

#include <cstdint>

#if defined(PLATFORM_MAC) || defined(PLATFORM_LINUX)
#include <pthread.h>
#endif

class SSyncObject {
    public:
    // Member variables
#if defined(PLATFORM_WIN)
    HANDLE m_opaqueData = nullptr;
#endif

#if defined(PLATFORM_MAC) || defined(PLATFORM_LINUX)
    int32_t m_int0 = 6;
    int32_t m_value;
    pthread_cond_t m_cond;
    pthread_mutex_t m_mutex;
#endif

    // Member functions
    SSyncObject();
    void Close(void);
    bool Valid(void);
    uint32_t Wait(uint32_t timeoutMs);
};

#endif
