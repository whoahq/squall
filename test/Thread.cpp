#include "storm/Thread.hpp"
#include "storm/Memory.hpp"
#include "test/Test.hpp"

uint32_t threadProc(void* param) {
    return 0;
};

TEST_CASE("CCritSect::Enter", "[thread]") {
    SECTION("locks critical section") {
        CCritSect critSect;
        critSect.Enter();
        SUCCEED();
    }
}

TEST_CASE("CCritSect::Leave", "[thread]") {
    SECTION("unlocks critical section") {
        CCritSect critSect;
        critSect.Enter();
        critSect.Leave();
        SUCCEED();
    }
}

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
        auto threadName = const_cast<char*>("TestThread");
        auto threadParam = STORM_ALLOC(16);
        REQUIRE(SThread::Create(threadProc, threadParam, thread, threadName, 0) != 0);

        STORM_FREE(threadParam);
    }
}
