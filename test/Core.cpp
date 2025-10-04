#include "test/Test.hpp"
#include "EventTest.hpp"
#include "storm/Core.hpp"


TEST_CASE("StormDestroy", "[core]") {
    SECTION("always returns 1") {
        CHECK(StormDestroy() == 1);
    }

    SECTION("SEvt") {
        EventHandlerTest test;

        SECTION("destroys all event handlers") {
            SEvtRegisterHandler(1, 1, 1, 0, &TestEventHandler1);
            CHECK(SEvtDispatch(1, 1, 1, nullptr) == 1);
            CHECK(test.NumCalls() == 1);

            CHECK(SEvtDispatch(1, 1, 1, nullptr) == 1);
            CHECK(test.NumCalls() == 2);

            CHECK(StormDestroy() == 1);

            // Can't increment calls since the handler was destroyed
            CHECK(SEvtDispatch(1, 1, 1, nullptr) == 0);
            CHECK(test.NumCalls() == 2);
        }

        SECTION("doesn't destroy break data") {
            // not ideal but it's official behaviour
            SEvtBreakHandlerChain(nullptr);

            CHECK(StormDestroy() == 1);

            SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler1);
            CHECK(SEvtDispatch(0, 0, 0, nullptr) == 0);
            CHECK(SEvtDispatch(0, 0, 0, nullptr) == 1);
        }
    }
}
