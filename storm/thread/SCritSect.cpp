#include "thread/SCritSect.hpp"

void SCritSect::Enter() {
#if defined(PLATFORM_WIN)
    EnterCriticalSection(&this->m_opaqueData);
#endif

#if defined(PLATFORM_MAC) || defined(PLATFORM_LINUX)
    pthread_mutex_lock(&this->m_mutex);
#endif
}

void SCritSect::Leave() {
#if defined(PLATFORM_WIN)
    LeaveCriticalSection(&this->m_opaqueData);
#endif

#if defined(PLATFORM_MAC) || defined(PLATFORM_LINUX)
    pthread_mutex_unlock(&this->m_mutex);
#endif
}
