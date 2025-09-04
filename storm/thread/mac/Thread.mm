#include "storm/Thread.hpp"
#include "storm/Memory.hpp"
#include "storm/String.hpp"
#include "storm/thread/S_Thread.hpp"
#include "storm/thread/mac/SThreadRunner.h"
#include <new>

void* SCreateThread(uint32_t (*threadProc)(void*), void* threadParam, void* a3, SThread* syncObject, const char* threadName) {
    if (!threadName) {
        threadName = "";
    }

    // TODO
    // S_Thread::s_threadCrit.Enter();

    if (!S_Thread::s_numthreads) {
        uint32_t threadId = S_Thread::s_threadID++;

        auto& mainThread = S_Thread::s_threads[0];
        mainThread.suspended = 0;
        mainThread.live = 1;
        mainThread.threadId = threadId;
        SStrCopy(mainThread.name, "main", sizeof(mainThread.name));

        S_Thread::s_numthreads++;

        // TODO
        /*
        if (StormGetOption(8, &v32, v33)) {
            auto& cdThread = S_Thread::s_threads[S_Thread::s_numthreads];
            cdThread.suspended = 0;
            cdThread.live = 1;
            cdThread.threadId = v32;
            SStrCopy(cdThread.name, "CdThreadProc", sizeof(cdThread.name));

            S_Thread::s_numthreads++;
        }
        */
    }

    uint32_t threadId = S_Thread::s_threadID++;

    auto params = STORM_NEW_ZERO(SThreadParmBlock);
    params->threadProc = threadProc;
    params->threadParam = threadParam;
    params->threadID = threadId;
    params->syncObject = syncObject;

    if (syncObject) {
        syncObject->m_value1 = 0;
        pthread_cond_init(&syncObject->m_cond, nullptr);
        pthread_mutex_init(&syncObject->m_mutex, nullptr);
    }

    // Launch thread
    auto threadRunner = [[SThreadRunner alloc] init:params];
    [NSThread
        detachNewThreadSelector:@selector(run)
                       toTarget:threadRunner
                     withObject:nil];

    auto& thread = S_Thread::s_threads[S_Thread::s_numthreads];
    thread.suspended = 0;
    thread.live = 1;
    thread.threadId = threadId;
    SStrCopy(thread.name, threadName, sizeof(thread.name));

    S_Thread::s_numthreads++;

    // TODO
    // S_Thread::s_threadCrit.Leave();

    return reinterpret_cast<void*>(threadId);
}
