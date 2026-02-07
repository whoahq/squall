#include "storm/thread/SEvent.hpp"

SEvent::SEvent(int32_t manualReset, int32_t initialValue)
    : SSyncObject() {
#if defined(WHOA_SYSTEM_WIN)
    this->m_opaqueData = CreateEventA(nullptr, manualReset, initialValue, nullptr);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_WEB)
    this->int0 = 2 - (manualReset >= 1);
    this->m_value1 = initialValue;

    pthread_cond_init(&this->m_cond, nullptr);
#endif
}

int32_t SEvent::Reset() {
#if defined(WHOA_SYSTEM_WIN)
    return ResetEvent(this->m_opaqueData);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_WEB)
    pthread_mutex_lock(&this->m_mutex);
    this->m_value1 = 0;
    pthread_mutex_unlock(&this->m_mutex);

    return 1;
#endif
}

int32_t SEvent::Set() {
#if defined(WHOA_SYSTEM_WIN)
    return SetEvent(this->m_opaqueData);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_WEB)
    pthread_mutex_lock(&this->m_mutex);

    this->m_value1 = 1;
    pthread_cond_signal(&this->m_cond);

    pthread_mutex_unlock(&this->m_mutex);

    return 1;
#endif
}
