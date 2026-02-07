#include "storm/thread/CSRWLock.hpp"

CSRWLock::CSRWLock() {
#if defined(WHOA_SYSTEM_WIN)
    // TODO
    SRWLock::SURWLockInitialize(&this->m_opaqueData);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_WEB)
    pthread_rwlock_init(&this->m_lock, nullptr);
#endif
}

CSRWLock::~CSRWLock() {
#if defined(WHOA_SYSTEM_WIN)
    SRWLock::SURWLockDelete(&this->m_opaqueData);
    // TODO
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_WEB)
    pthread_rwlock_destroy(&this->m_lock);
#endif
}

void CSRWLock::Enter(int32_t forwriting) {
#if defined(WHOA_SYSTEM_WIN)
    SRWLock::SURWLockEnter(&this->m_opaqueData, forwriting);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_WEB)
    if (forwriting) {
        pthread_rwlock_wrlock(&this->m_lock);
    } else {
        pthread_rwlock_rdlock(&this->m_lock);
    }
#endif
}

void CSRWLock::Leave(int32_t fromwriting) {
#if defined(WHOA_SYSTEM_WIN)
    SRWLock::SURWLockLeave(&this->m_opaqueData, fromwriting);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_WEB)
    pthread_rwlock_unlock(&this->m_lock);
#endif
}
