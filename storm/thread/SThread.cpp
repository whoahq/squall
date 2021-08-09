#include "storm/thread/SThread.hpp"
#include "storm/Thread.hpp"

int32_t SThread::Create(uint32_t (*threadProc)(void*), void* param, SThread& thread, char* threadName, uint32_t a5) {
#if defined(WHOA_SYSTEM_WIN)
    // TODO implement
    return 0;
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    thread.int0 = 5;
    thread.m_value1 = 0;
    pthread_cond_init(&thread.m_cond, nullptr);

    uint32_t v8;
    return SCreateThread(threadProc, param, &v8, &thread, nullptr) != 0;
#endif
}
