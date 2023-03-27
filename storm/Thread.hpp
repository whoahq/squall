#ifndef STORM_THREAD_HPP
#define STORM_THREAD_HPP

#include "storm/thread/CCritSect.hpp"
#include "storm/thread/CSRWLock.hpp"
#include "storm/thread/SCritSect.hpp"
#include "storm/thread/SEvent.hpp"
#include "storm/thread/SSemaphore.hpp"
#include "storm/thread/SSyncObject.hpp"
#include "storm/thread/SThread.hpp"
#include <cstdint>

void* SCreateThread(uint32_t (*threadProc)(void*), void* threadParam, void* a3, SThread* syncObject, const char* threadName);

uintptr_t SGetCurrentThreadId();

#endif
