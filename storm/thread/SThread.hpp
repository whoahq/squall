#ifndef STORM_THREAD_S_THREAD_HPP
#define STORM_THREAD_S_THREAD_HPP

#include "thread/SSyncObject.hpp"
#include <cstdint>

class SThread : public SSyncObject {
    public:
    // Static functions
    static int32_t Create(uint32_t (*threadProc)(void*), void* param, SThread& thread, char* threadName, uint32_t a5);
};

#endif
