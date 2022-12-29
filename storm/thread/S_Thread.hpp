#ifndef STORM_THREAD_S__THREAD_HPP
#define STORM_THREAD_S__THREAD_HPP

#include "storm/thread/SThread.hpp"
#include <cstdint>
#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>
#endif

typedef SThread SyncObjectData;

struct SThreadParmBlock {
    uint32_t (*threadProc)(void*);
    void* threadParam;
    uint32_t threadID;
#if defined(WHOA_SYSTEM_WIN)
    HANDLE threadH;
#endif
#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    SyncObjectData* syncObject;
#endif
};

class S_Thread {
    public:
    // Types
    struct SThreadTrack {
        int32_t suspended;
        int32_t live;
        uint32_t threadId;
#if defined(WHOA_SYSTEM_WIN)
        HANDLE threadH;
#endif
        char name[16];
    };

    // Static variables
    static int32_t s_numthreads;
    static int32_t s_maxthreads;
    static uint32_t s_threadID;
    static SThreadTrack s_threads[1024];

    // Static functions
#if defined(WHOA_SYSTEM_WIN)
    static DWORD s_SLaunchThread(void* threadParam);
#endif

#if defined(WHOA_SYSTEM_MAC)
    static uint32_t s_SLaunchThread(void* threadParam);
#endif

#if defined(WHOA_SYSTEM_LINUX)
    static void* s_SLaunchThread(void* threadParam);
#endif
};

#endif
