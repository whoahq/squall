#include "storm/thread/SCritSect.hpp"

SCritSect::SCritSect() {
#if defined(WHOA_SYSTEM_WIN)
    InitializeCriticalSection(&this->m_opaqueData);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(&this->m_mutex, &attr);
#endif
}

SCritSect::~SCritSect() {
#if defined(WHOA_SYSTEM_WIN)
    DeleteCriticalSection(&this->m_opaqueData);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_mutex_destroy(&this->m_mutex);
#endif
}

void SCritSect::Enter() {
#if defined(WHOA_SYSTEM_WIN)
    EnterCriticalSection(&this->m_opaqueData);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_mutex_lock(&this->m_mutex);
#endif
}

void SCritSect::Leave() {
#if defined(WHOA_SYSTEM_WIN)
    LeaveCriticalSection(&this->m_opaqueData);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_mutex_unlock(&this->m_mutex);
#endif
}
