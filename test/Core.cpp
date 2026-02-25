#include "test/Test.hpp"
#include "EventTest.hpp"
#include "storm/Core.hpp"
#include "storm/Region.hpp"

#if defined(WHOA_DESTROY_HAS_RETVAL)
#define CHECK_DESTROY(x) CHECK(x == 1);
#else
#define CHECK_DESTROY(x) x;
#endif

TEST_CASE("StormDestroy", "[core]") {

#if defined(WHOA_DESTROY_HAS_RETVAL)
    SECTION("always returns 1") {
        CHECK(StormDestroy() == 1);
    }
#endif

    SECTION("SEvt") {
        EventHandlerTest test;

        SECTION("destroys all event handlers") {
            SEvtRegisterHandler(1, 1, 1, 0, &TestEventHandler1);
            CHECK(SEvtDispatch(1, 1, 1, nullptr) == 1);
            CHECK(test.NumCalls() == 1);

            CHECK(SEvtDispatch(1, 1, 1, nullptr) == 1);
            CHECK(test.NumCalls() == 2);

            CHECK_DESTROY(StormDestroy());

            // Can't increment calls since the handler was destroyed
            CHECK(SEvtDispatch(1, 1, 1, nullptr) == 0);
            CHECK(test.NumCalls() == 2);
        }

        SECTION("doesn't destroy break data") {
            // not ideal but it's official behaviour
            SEvtBreakHandlerChain(nullptr);

            CHECK_DESTROY(StormDestroy());

            SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler1);
            CHECK(SEvtDispatch(0, 0, 0, nullptr) == 0);
            CHECK(SEvtDispatch(0, 0, 0, nullptr) == 1);
        }
    }

    SECTION("SRgn") {
        SECTION("destroys region handles") {
            HSRGN rgn = nullptr;
            SRgnCreate(&rgn);
            REQUIRE(rgn != nullptr);    // valid handle

            CHECK_DESTROY(StormDestroy());

            // fails to duplicate because handle is invalid
            HSRGN newrgn;
            SRgnDuplicate(rgn, &newrgn);
            CHECK(newrgn == nullptr);
        }
    }
}
