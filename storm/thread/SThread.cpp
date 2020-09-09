#include "thread/SThread.hpp"
#include "Thread.hpp"

int32_t SThread::Create(uint32_t (*threadProc)(void*), void* param, SThread& thread, char* threadName, uint32_t a5) {
#if defined(PLATFORM_WIN)
    // TODO implement
#endif

#if defined(PLATFORM_MAC) || defined(PLATFORM_LINUX)
    thread.m_int0 = 5;
    thread.m_value = 0;
    pthread_cond_init(&thread.m_cond, nullptr);

    uint32_t v8;
    return SCreateThread(threadProc, param, &v8, &thread, nullptr) != 0;
#endif
}
