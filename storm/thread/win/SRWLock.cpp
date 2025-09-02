#include "storm/thread/win/SRWLock.hpp"

void SRWLock::SUNNLockEnter(volatile SUNNLOCK* sunnlock) {
    // TODO
}

void SRWLock::SUNNLockLeave(volatile SUNNLOCK* sunnlock) {
    // TODO
}

void SRWLock::SURWLockInitialize(volatile SRWLock::SURWLOCK* surwlock) {
    // TODO
}

void SRWLock::SURWLockDelete(volatile SRWLock::SURWLOCK* surwlock) {
    // TODO
}

void SRWLock::SURWLockEnter(volatile SURWLOCK* surwlock, int32_t forwriting) {
    // TODO
}

void SRWLock::SURWLockLeave(volatile SURWLOCK* surwlock, int32_t fromwriting) {
    // TODO
}
