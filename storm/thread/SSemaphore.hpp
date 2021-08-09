#ifndef STORM_THREAD_S_SEMAPHORE_HPP
#define STORM_THREAD_S_SEMAPHORE_HPP

#include "storm/thread/SSyncObject.hpp"
#include <cstdint>

class SSemaphore : public SSyncObject {
    public:
    // Member functions
    SSemaphore(uint32_t initialCount, uint32_t maximumCount);
};

#endif
