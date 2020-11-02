#include "storm/thread/CSRWLock.hpp"

void CSRWLock::Enter(int32_t forwriting) {
#ifdef PLATFORM_WIN
    // TODO
#endif

#if defined(PLATFORM_MAC) || defined(PLATFORM_LINUX)
    if (forwriting) {
        pthread_rwlock_wrlock(&this->m_lock);
    } else {
        pthread_rwlock_rdlock(&this->m_lock);
    }
#endif
}

void CSRWLock::Leave(int32_t fromwriting) {
#ifdef PLATFORM_WIN
    // TODO
#endif

#if defined(PLATFORM_MAC) || defined(PLATFORM_LINUX)
    pthread_rwlock_unlock(&this->m_lock);
#endif
}
