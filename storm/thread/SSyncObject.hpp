#ifndef STORM_THREAD_S_SYNC_OBJECT_HPP
#define STORM_THREAD_S_SYNC_OBJECT_HPP

#include <cstdint>

#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
#include <pthread.h>
#endif

class SSyncObject {
    public:
    // Member variables
#if defined(WHOA_SYSTEM_WIN)
    HANDLE m_opaqueData = nullptr;
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    int32_t int0 = 6;
    int32_t m_value1;
    int32_t m_value2;
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
#endif

    // Member functions
    SSyncObject();
    void Close(void);
    bool Valid();
    uint32_t Wait(uint32_t timeoutMs);
};

#endif
