#include "storm/Thread.hpp"
#include "test/Test.hpp"

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
