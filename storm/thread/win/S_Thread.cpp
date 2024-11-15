#include "storm/thread/S_Thread.hpp"
#include "storm/Memory.hpp"

#ifndef WINAPI
#define WINAPI
#endif

DWORD WINAPI S_Thread::s_SLaunchThread(void* threadParam) {
    auto params = static_cast<SThreadParmBlock*>(threadParam);
    auto proc = params->threadProc;
    auto param = params->threadParam;

    SMemFree(threadParam);

    auto val = proc(param);

    // TODO
    // S_Thread::s_threadCrit.Enter();

    // TODO
    // - remove tracking in S_Thread::s_threads
    // - decrement num threads

    // TODO
    // S_Thread::s_threadCrit.Leave();

    return val;
}
