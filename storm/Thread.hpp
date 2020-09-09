#ifndef STORM_THREAD_HPP
#define STORM_THREAD_HPP

#include "thread/SCritSect.hpp"
#include "thread/SEvent.hpp"
#include "thread/SSyncObject.hpp"
#include "thread/SThread.hpp"
#include <cstdint>

int32_t SCreateThread(uint32_t (*threadProc)(void*), void* threadParam, void* a3, SThread* syncObject, const char* threadName);

#endif
