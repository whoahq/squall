#include "storm/thread/CCritSect.hpp"

CCritSect::CCritSect() {
#if defined(WHOA_SYSTEM_WIN)
    InitializeCriticalSection(&this->m_critsect);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_mutex_init(&this->m_critsect, nullptr);
#endif
}

CCritSect::~CCritSect() {
#if defined(WHOA_SYSTEM_WIN)
    DeleteCriticalSection(&this->m_critsect);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_mutex_destroy(&this->m_critsect);
#endif
}

void CCritSect::Enter() {
#if defined(WHOA_SYSTEM_WIN)
    EnterCriticalSection(&this->m_critsect);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_mutex_lock(&this->m_critsect);
#endif
}

void CCritSect::Enter(int32_t forWriting) {
    // Overload to provide compatibility with CSRWLock
    this->Enter();
}

void CCritSect::Leave() {
#if defined(WHOA_SYSTEM_WIN)
    LeaveCriticalSection(&this->m_critsect);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_mutex_unlock(&this->m_critsect);
#endif
}

void CCritSect::Leave(int32_t fromWriting) {
    // Overload to provide compatibility with CSRWLock
    this->Leave();
}
