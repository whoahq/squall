#ifndef STORM_THREAD_WIN_S_RW_LOCK_HPP
#define STORM_THREAD_WIN_S_RW_LOCK_HPP

#include <cstdint>

class SRWLock {
    public:
    // Types
    struct SUNNLOCK {
        int32_t m_state;
        int32_t m_event;
    };

    struct SURWLOCK {
        SRWLock::SUNNLOCK m_mutex;
        int32_t m_readerEvent;
    };

    // Static functions
    static void SUNNLockEnter(volatile SUNNLOCK* sunnlock);
    static void SUNNLockLeave(volatile SUNNLOCK* sunnlock);
    static void SURWLockInitialize(volatile SURWLOCK* surwlock);
    static void SURWLockDelete(volatile SURWLOCK* surwlock);
    static void SURWLockEnter(volatile SURWLOCK* surwlock, int32_t forwriting);
    static void SURWLockLeave(volatile SURWLOCK* surwlock, int32_t fromwriting);
};

#endif
