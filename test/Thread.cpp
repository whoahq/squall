#include "storm/Thread.hpp"
#include "test/Test.hpp"

uint32_t threadProc(void* param) {
    return 0;
};

TEST_CASE("SGetCurrentThreadId", "[thread]") {
    SECTION("returns thread id") {
        uintptr_t threadId = SGetCurrentThreadId();
        CHECK(threadId > 0);
    }
}

TEST_CASE("SCritSect::Enter", "[thread]") {
    SECTION("locks critical section") {
        SCritSect critSect;
        critSect.Enter();
        SUCCEED();
    }
}

TEST_CASE("SCritSect::Leave", "[thread]") {
    SECTION("unlocks critical section") {
        SCritSect critSect;
        critSect.Enter();
        critSect.Leave();
        SUCCEED();
    }
}

TEST_CASE("SThread::Create", "[thread]") {
    SECTION("creates new thread") {
        SThread thread;
        char* threadName = const_cast<char*>("TestThread");
        REQUIRE(SThread::Create(threadProc, nullptr, thread, threadName, 0) != 0);
    }
}
