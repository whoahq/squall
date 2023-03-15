#include "storm/Thread.hpp"
#include "storm/Memory.hpp"
#include "storm/String.hpp"
#include "storm/thread/S_Thread.hpp"
#include <new>
#include <windows.h>

void* SCreateThread(uint32_t (*threadProc)(void*), void* threadParam, void* a3, SThread* syncObject, const char* threadName) {
    if (!threadName) {
        threadName = "";
    }

    // TODO
    // S_Thread::s_threadCrit.Enter();

    if (!S_Thread::s_numthreads) {
        auto& mainThread = S_Thread::s_threads[0];
        mainThread.suspended = 0;
        mainThread.live = 1;
        mainThread.threadId = GetCurrentThreadId();
        mainThread.threadH = nullptr;
        SStrCopy(mainThread.name, "main", sizeof(mainThread.name));

        S_Thread::s_numthreads++;

        // TODO
        /*
        if (StormGetOption(8, &v32, v33)) {
            auto& cdThread = S_Thread::s_threads[S_Thread::s_numthreads];
            cdThread.suspended = 0;
            cdThread.live = 1;
            cdThread.threadId = v32.cdThreadId;
            cdThread.threadH = v32.threadH;
            SStrCopy(cdThread.name, "CdThreadProc", sizeof(cdThread.name));

            S_Thread::s_numthreads++;
        }
        */
    }

    void* m = SMemAlloc(sizeof(SThreadParmBlock), __FILE__, __LINE__, 0x8);
    auto params = new (m) SThreadParmBlock();
    params->threadProc = threadProc;
    params->threadParam = threadParam;

    DWORD threadId;
    // TODO pass dwStackSize and dwCreationFlags through to CreateThread
    HANDLE threadH = CreateThread(nullptr, 0, S_Thread::s_SLaunchThread, params, 0x0, &threadId);
    params->threadH = threadH;

    auto& thread = S_Thread::s_threads[S_Thread::s_numthreads];
    thread.suspended = 0; // TODO set from dwCreationFlags
    thread.live = 1;
    thread.threadId = threadId;
    thread.threadH = threadH;

    if (threadName) {
        SStrCopy(thread.name, threadName, sizeof(thread.name));
    }

    S_Thread::s_numthreads++;

    // TODO
    // S_Thread::s_threadCrit.Leave();

    return threadH;
}
