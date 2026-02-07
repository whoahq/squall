#include "storm/thread/S_Thread.hpp"
#include "storm/Memory.hpp"

void* S_Thread::s_SLaunchThread(void* threadParam) {
    // TODO

    auto params = static_cast<SThreadParmBlock*>(threadParam);

    // TODO

    params->threadProc(params->threadParam);

    // TODO

    if (params->syncObject) {
        pthread_mutex_lock(&params->syncObject->m_mutex);
        params->syncObject->m_value1 = 1;
        pthread_mutex_unlock(&params->syncObject->m_mutex);

        pthread_cond_signal(&params->syncObject->m_cond);
    }

    STORM_FREE(threadParam);

    return nullptr;
}
