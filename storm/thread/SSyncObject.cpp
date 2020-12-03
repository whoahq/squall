#include "storm/thread/SSyncObject.hpp"

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
#include <cerrno>
#include <sys/time.h>
#include <unistd.h>
#endif

SSyncObject::SSyncObject() {
#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_mutex_init(&this->m_mutex, 0);
#endif
}

uint32_t SSyncObject::Wait(uint32_t timeoutMs) {
#if defined(WHOA_SYSTEM_WIN)
    return WaitForSingleObject(this->m_opaqueData, timeoutMs);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    if (this->m_int0 == 6) {
        // WAIT_FAILED
        return 0xFFFFFFFF;
    }

    if (timeoutMs != 0xFFFFFFFF) {
        timeval v10;
        timespec v11;

        if (timeoutMs) {
            gettimeofday(&v10, nullptr);

            v11.tv_sec = v10.tv_sec + timeoutMs / 1000;
            uint32_t v7 = 1000 * v10.tv_usec + 1000000 * (timeoutMs % 1000);
            v11.tv_nsec = v7;

            if (v7 > 999999999) {
                v11.tv_sec = v10.tv_sec + timeoutMs / 1000 + 1;
                v11.tv_nsec = v7 - 1000000000;
            }
        } else {
            v11.tv_sec = 0;
            v11.tv_nsec = 0;
        }

        while (true) {
            int32_t v3 = pthread_mutex_trylock(&this->m_mutex);

            if (!v3) {
                break;
            }

            if (v3 != EBUSY) {
                if (v3 != ETIMEDOUT) {
                    // WAIT_FAILED
                    return 0xFFFFFFFF;
                } else {
                    // WAIT_TIMEOUT
                    return 0x00000102;
                }
            }

            gettimeofday(&v10, nullptr);

            if (v10.tv_sec > v11.tv_sec || (v10.tv_sec == v11.tv_sec && 1000 * v10.tv_usec >= v11.tv_nsec)) {
                // WAIT_TIMEOUT
                return 0x00000102;
            }

            usleep(0);
        }

        if (this->m_int0 == 3) {
            // WAIT_OBJECT_0
            return 0;
        }

        int32_t v4;

        while (true) {
            v4 = this->m_value;

            if (v4) {
                break;
            }

            int32_t v5 = pthread_cond_timedwait(&this->m_cond, &this->m_mutex, &v11);

            if (v5) {
                pthread_mutex_unlock(&this->m_mutex);

                if (v5 == ETIMEDOUT) {
                    // WAIT_TIMEOUT
                    return 0x00000102;
                } else {
                    // WAIT_FAILED
                    return 0xFFFFFFFF;
                }
            }
        }

        if (this->m_int0 == 2) {
            this->m_value = 0;
        } else if (this->m_int0 == 4) {
            this->m_value = v4 - 1;
        }

        pthread_mutex_unlock(&this->m_mutex);

        // WAIT_OBJECT_0
        return 0;
    }

    pthread_mutex_lock(&this->m_mutex);

    if (this->m_int0 == 3) {
        // WAIT_OBJECT_0
        return 0;
    }

    while (!this->m_value) {
        pthread_cond_wait(&this->m_cond, &this->m_mutex);
    }

    if (this->m_int0 == 2) {
        this->m_value = 0;
    } else if (this->m_int0 == 4) {
        this->m_value = this->m_value - 1;
    }

    pthread_mutex_unlock(&this->m_mutex);

    // WAIT_OBJECT_0
    return 0;
#endif
}
