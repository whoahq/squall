#include "storm/thread/SCritSect.hpp"

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
