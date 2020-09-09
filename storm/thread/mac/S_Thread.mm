#include "thread/S_Thread.hpp"
#include "Memory.hpp"

uint32_t S_Thread::s_SLaunchThread(void* threadParam) {
    // TODO
    // SClearFP(1);

    auto params = static_cast<SThreadParmBlock*>(threadParam);

    // TODO
    // S_Thread::s_threadCrit.Enter();

    // TODO
    // - something involving pthread_self()

    // TODO
    // S_Thread::s_threadCrit.Leave();

    // TODO
    // SErrAddFrameText("Calling thread proc %lx\n", params->threadProc);
    params->threadProc(params->threadParam);

    // TODO
    // S_Thread::s_threadCrit.Enter();

    // TODO
    // - remove tracking in S_Thread::s_threads
    // - decrement num threads

    // TODO
    // S_Thread::s_threadCrit.Leave();

    if (params->syncObject) {
        pthread_mutex_lock(&params->syncObject->m_mutex);
        params->syncObject->m_value = 1;
        pthread_mutex_unlock(&params->syncObject->m_mutex);
        pthread_cond_signal(&params->syncObject->m_cond);
    }

    SMemFree(threadParam);

    return 0;
}
