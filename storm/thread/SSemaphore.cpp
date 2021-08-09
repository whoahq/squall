#include "storm/thread/SSemaphore.hpp"

SSemaphore::SSemaphore(uint32_t initialCount, uint32_t maximumCount)
    : SSyncObject() {
#if defined(WHOA_SYSTEM_WIN)
    this->m_opaqueData = CreateSemaphoreA(nullptr, initialCount, maximumCount, nullptr);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    this->m_value1 = initialCount;
    this->m_value2 = maximumCount;
    this->int0 = 4;
    pthread_cond_init(&this->m_cond, 0);
#endif
}

int32_t SSemaphore::Signal(uint32_t count) {
#if defined(WHOA_SYSTEM_WIN)
    return ReleaseSemaphore(this->m_opaqueData, count, nullptr);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_mutex_lock(&this->m_mutex);

    if (this->m_value1 + count > this->m_value2) {
        pthread_mutex_unlock(&this->m_mutex);

        return 0;
    }

    this->m_value1 += count;

    if (count <= 1) {
        pthread_cond_signal(&this->m_cond);
    } else {
        pthread_cond_broadcast(&this->m_cond);
    }

    pthread_mutex_unlock(&this->m_mutex);

    return 1;
#endif
}
