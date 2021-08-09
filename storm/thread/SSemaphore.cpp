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
