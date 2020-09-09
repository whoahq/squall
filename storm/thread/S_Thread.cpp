#include "thread/S_Thread.hpp"

int32_t S_Thread::s_numthreads;

int32_t S_Thread::s_maxthreads = 1024;

uint32_t S_Thread::s_threadID;

S_Thread::SThreadTrack S_Thread::s_threads[1024];
