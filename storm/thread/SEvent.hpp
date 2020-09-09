#ifndef STORM_THREAD_S_EVENT_HPP
#define STORM_THREAD_S_EVENT_HPP

#include "thread/SSyncObject.hpp"
#include <cstdint>

class SEvent : public SSyncObject {
    public:
    // Member functions
    SEvent(int32_t manualReset, int32_t);
    int32_t Reset(void);
    int32_t Set(void);
};

#endif
