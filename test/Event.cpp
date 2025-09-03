#include "EventTest.h"


static void STORMAPI TestBreakEventHandlerSelf(void* data) {
    EventHandlerTest::RegisterCall(10, data);

    CHECK(SEvtBreakHandlerChain(data) == 1);
}

static void STORMAPI TestBreakEventHandlerOther(void* data) {
    EventHandlerTest::RegisterCall(10, data);

    int bunk = 0;
    CHECK(SEvtBreakHandlerChain(&bunk) == 1);
}

TEST_CASE("SEvtBreakHandlerChain", "[event]") {
    EventHandlerTest test;

    SECTION("can use nullptr as data") {
        CHECK(SEvtBreakHandlerChain(nullptr) == 1);

        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler1);
        CHECK(SEvtDispatch(7357, 1, 0, nullptr) == 0);
    }

    SECTION("causes SEvtDispatch to break early if one of many data matches") {
        int data1 = 42, data2 = 1337, data3 = 0xFFFFFFFF;

        CHECK(SEvtBreakHandlerChain(nullptr) == 1);
        CHECK(SEvtBreakHandlerChain(&data1) == 1);
        CHECK(SEvtBreakHandlerChain(&data2) == 1);
        CHECK(SEvtBreakHandlerChain(&data3) == 1);

        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler1);
        CHECK(SEvtDispatch(7357, 1, 0, &data2) == 0);
    }

    SECTION("doesn't break SEvtDispatch if no data matches") {
        int data1 = 42, data2 = 1337, data3 = 0xFFFFFFFF;

        CHECK(SEvtBreakHandlerChain(nullptr) == 1);
        CHECK(SEvtBreakHandlerChain(&data1) == 1);
        CHECK(SEvtBreakHandlerChain(&data2) == 1);
        CHECK(SEvtBreakHandlerChain(&data3) == 1);

        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler1);
        CHECK(SEvtDispatch(7357, 1, 0, &test) == 1);
    }

    SECTION("deduplicates multiple same-data breaks") {
        CHECK(SEvtBreakHandlerChain(&test) == 1);
        CHECK(SEvtBreakHandlerChain(&test) == 1);
        CHECK(SEvtBreakHandlerChain(&test) == 1);

        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler1);
        CHECK(SEvtDispatch(7357, 1, 0, &test) == 0);
        CHECK(SEvtDispatch(7357, 1, 0, &test) == 1);
    }

    SECTION("causes a dispatch to stop during handling") {
        SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler1);
        SEvtRegisterHandler(0, 0, 0, 0, &TestBreakEventHandlerSelf);
        SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler2);

        CHECK(SEvtDispatch(0, 0, 0, nullptr) == 1);
        CHECK(test.NumCalls() == 2);
        // Calls are reverse order, TestEventHandler1 doesn't get called
        CHECK_THAT(test.CallResult(), MatchesCall({ 2, nullptr }));
        CHECK_THAT(test.CallResult(), MatchesCall({ 10, nullptr }));
    }

    SECTION("doesn't cause a dispatch to stop during handling if data differs") {
        SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler1);
        SEvtRegisterHandler(0, 0, 0, 0, &TestBreakEventHandlerOther);
        SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler2);

        CHECK(SEvtDispatch(0, 0, 0, nullptr) == 1);
        CHECK(test.NumCalls() == 3);
        // Calls are reverse order, TestEventHandler1 doesn't get called
        CHECK_THAT(test.CallResult(), MatchesCall({ 2, nullptr }));
        CHECK_THAT(test.CallResult(), MatchesCall({ 10, nullptr }));
        CHECK_THAT(test.CallResult(), MatchesCall({ 1, nullptr }));
    }
}

TEST_CASE("SEvtDestroy", "[event]") {
    EventHandlerTest test;

    SECTION("always returns 1") {
        CHECK(SEvtDestroy() == 1);
    }

    SECTION("destroys all event handlers") {
        SEvtRegisterHandler(1, 1, 1, 0, &TestEventHandler1);
        CHECK(SEvtDispatch(1, 1, 1, nullptr) == 1);
        CHECK(test.NumCalls() == 1);

        CHECK(SEvtDispatch(1, 1, 1, nullptr) == 1);
        CHECK(test.NumCalls() == 2);

        CHECK(SEvtDestroy() == 1);

        // Can't increment calls since the handler was destroyed
        CHECK(SEvtDispatch(1, 1, 1, nullptr) == 0);
        CHECK(test.NumCalls() == 2);
    }

    SECTION("doesn't destroy break data") {
        // not ideal but it's official behaviour
        SEvtBreakHandlerChain(nullptr);

        CHECK(SEvtDestroy() == 1);

        SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler1);
        CHECK(SEvtDispatch(0, 0, 0, nullptr) == 0);
        CHECK(SEvtDispatch(0, 0, 0, nullptr) == 1);
    }
}

static void STORMAPI TestNestedDispatchEventHandler(void* data) {
    EventHandlerTest::RegisterCall(20, data);

    CHECK(SEvtDispatch(1337, 420, 69, nullptr) == 1);
}

TEST_CASE("SEvtDispatch", "[event]") {
    EventHandlerTest test;

    SECTION("sends data to an event handler") {
        SEvtRegisterHandler(1337, 42, 5, 0, &TestEventHandler1);

        int data = 5;
        CHECK(SEvtDispatch(1337, 42, 5, &data) == 1);

        REQUIRE(test.NumCalls() == 1);
        CHECK_THAT(test.CallResult(), MatchesCall({ 1, &data }));
    }

    SECTION("can use 0 as valid handler ids") {
        SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler1);

        int data = 5;
        CHECK(SEvtDispatch(0, 0, 0, &data) == 1);

        REQUIRE(test.NumCalls() == 1);
        CHECK_THAT(test.CallResult(), MatchesCall({ 1, &data }));
    }

    SECTION("can pass nullptr to a handler") {
        SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler1);

        CHECK(SEvtDispatch(0, 0, 0, nullptr) == 1);

        REQUIRE(test.NumCalls() == 1);
        CHECK_THAT(test.CallResult(), MatchesCall({ 1, nullptr }));
    }

    SECTION("sends data to multiple event handlers") {
        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler1);
        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler2);
        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler3);

        int data = 1;
        CHECK(SEvtDispatch(7357, 1, 0, &data) == 1);

        REQUIRE(test.NumCalls() == 3);
        CHECK_THAT(test.CallResult(), MatchesCall({ 3, &data }));
        CHECK_THAT(test.CallResult(), MatchesCall({ 2, &data }));
        CHECK_THAT(test.CallResult(), MatchesCall({ 1, &data }));
    }

    SECTION("sends data to multiple event handlers with reverse ordering") {
        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler3);
        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler1);
        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler2);

        int data = 1;
        CHECK(SEvtDispatch(7357, 1, 0, &data) == 1);

        REQUIRE(test.NumCalls() == 3);
        CHECK_THAT(test.CallResult(), MatchesCall({ 2, &data }));
        CHECK_THAT(test.CallResult(), MatchesCall({ 1, &data }));
        CHECK_THAT(test.CallResult(), MatchesCall({ 3, &data }));
    }

    SECTION("sends data to duplicate handlers") {
        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler1);
        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler1);
        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler1);

        int data = 1;
        CHECK(SEvtDispatch(7357, 1, 0, &data) == 1);

        REQUIRE(test.NumCalls() == 3);
        CHECK_THAT(test.CallResult(), MatchesCall({ 1, &data }));
        CHECK_THAT(test.CallResult(), MatchesCall({ 1, &data }));
        CHECK_THAT(test.CallResult(), MatchesCall({ 1, &data }));
    }

    SECTION("does nothing if there are no handlers in the type") {
        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler1);

        CHECK(SEvtDispatch(42, 1, 0, nullptr) == 0);
        CHECK(test.NumCalls() == 0);
    }

    SECTION("does nothing if there are no handlers that match the subtype") {
        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler1);

        CHECK(SEvtDispatch(7357, 2, 0, nullptr) == 0);
        CHECK(test.NumCalls() == 0);
    }

    SECTION("does nothing if there are no handlers that match the id") {
        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler1);

        CHECK(SEvtDispatch(7357, 1, 1, nullptr) == 0);
        CHECK(test.NumCalls() == 0);
    }

    SECTION("breaks the next dispatch if data matches any in break chain") {
        int data = 42;
        SEvtBreakHandlerChain(&data);

        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler1);

        // doesn't call because data matches the break chain
        CHECK(SEvtDispatch(7357, 1, 0, &data) == 0);
        CHECK(test.NumCalls() == 0);

        // calls because the break already occurred
        CHECK(SEvtDispatch(7357, 1, 0, &data) == 1);
        CHECK(test.NumCalls() == 1);
    }

    SECTION("calls handlers if data doesn't match any in break chain") {
        int data = 42;
        int data2 = 1337;
        SEvtBreakHandlerChain(&data);
        
        SEvtRegisterHandler(7357, 1, 0, 0, &TestEventHandler1);

        CHECK(SEvtDispatch(7357, 1, 0, &data2) == 1);
        CHECK(test.NumCalls() == 1);
    }

    SECTION("finds the correct handler among many") {
        SEvtRegisterHandler(9000, 0, 0, 0, &TestEventHandler1);
        SEvtRegisterHandler(9000, 1, 0, 0, &TestEventHandler1);
        SEvtRegisterHandler(9000, 1, 1, 0, &TestEventHandler1);
        SEvtRegisterHandler(420, 0, 0, 0, &TestEventHandler1);
        SEvtRegisterHandler(420, 1, 0, 0, &TestEventHandler1);
        SEvtRegisterHandler(420, 1, 1, 0, &TestEventHandler2);
        SEvtRegisterHandler(420, 1, 2, 0, &TestEventHandler1);
        SEvtRegisterHandler(42, 0, 0, 0, &TestEventHandler1);

        CHECK(SEvtDispatch(420, 1, 1, nullptr) == 1);

        CHECK(test.NumCalls() == 1);
        CHECK_THAT(test.CallResult(), MatchesCall({2, nullptr}));
    }

    SECTION("can use a non-pointer as data") {
        SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler1);

        CHECK(SEvtDispatch(0, 0, 0, reinterpret_cast<void*>(42)) == 1);
        CHECK_THAT(test.CallResult(), MatchesCall({1, reinterpret_cast<void*>(42)}));
    }

    SECTION("broken handler chain still results in success if at least 1 handler called") {
        SEvtRegisterHandler(0, 0, 0, 0, &TestBreakEventHandlerOther);

        CHECK(SEvtDispatch(0, 0, 0, nullptr) == 1);
        CHECK(test.NumCalls() == 1);
    }

    SECTION("can be called from handlers") {
        SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler1);
        SEvtRegisterHandler(1337, 420, 69, 0, &TestEventHandler1);
        SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler2);
        SEvtRegisterHandler(0, 0, 0, 0, &TestNestedDispatchEventHandler);
        SEvtRegisterHandler(1337, 420, 69, 0, &TestEventHandler3);
        SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler4);

        CHECK(SEvtDispatch(0, 0, 0, &test) == 1);
        CHECK(test.NumCalls() == 6);
        CHECK_THAT(test.CallResult(), MatchesCall({ 4, &test }));
        CHECK_THAT(test.CallResult(), MatchesCall({ 20, &test }));
        CHECK_THAT(test.CallResult(), MatchesCall({ 3, nullptr }));
        CHECK_THAT(test.CallResult(), MatchesCall({ 1, nullptr }));
        CHECK_THAT(test.CallResult(), MatchesCall({ 2, &test }));
        CHECK_THAT(test.CallResult(), MatchesCall({ 1, &test }));
    }
}

TEST_CASE("SEvtPopState", "[event]") {
    EventHandlerTest test;

    SECTION("fails if there are no handlers") {
        CHECK(SEvtPopState(0, 0) == 0);
    }

    SECTION("unregisters type if it is already top level") {
        SEvtRegisterHandler(1337, 420, 69, 0, &TestEventHandler1);

        CHECK(SEvtPopState(1337, 420) == 1);
        CHECK(SEvtUnregisterType(1337, 420) == 0);
    }

    SECTION("popped state can receive callbacks again") {
        SEvtRegisterHandler(1337, 420, 69, 0, &TestEventHandler1);
        CHECK(SEvtPushState(1337, 420) == 1);

        CHECK(SEvtDispatch(1337, 420, 69, nullptr) == 0);
        CHECK(test.NumCalls() == 0);

        CHECK(SEvtPopState(1337, 420) == 0);

        CHECK(SEvtDispatch(1337, 420, 69, nullptr) == 1);
        CHECK(test.NumCalls() == 1);
    }

    SECTION("popped state can be unregistered again") {
        SEvtRegisterHandler(1337, 420, 69, 0, &TestEventHandler1);
        CHECK(SEvtPushState(1337, 420) == 1);

        CHECK(SEvtUnregisterHandler(1337, 420, 69, &TestEventHandler1) == 0);
        CHECK(SEvtPopState(1337, 420) == 0);
        CHECK(SEvtUnregisterHandler(1337, 420, 69, &TestEventHandler1) == 1);
    }
}

TEST_CASE("SEvtPushState", "[event]") {
    EventHandlerTest test;

    SECTION("fails if there are no handlers") {
        CHECK(SEvtPushState(0, 0) == 0);
    }

    SECTION("fails if the type does not match any registered types") {
        SEvtRegisterHandler(0, 0, 1, 0, &TestEventHandler1);
        SEvtRegisterHandler(10, 0, 1, 0, &TestEventHandler1);
        SEvtRegisterHandler(0, 9, 1, 0, &TestEventHandler1);

        CHECK(SEvtPushState(10, 9) == 0);
    }

    SECTION("succeeds if the handler type exists") {
        SEvtRegisterHandler(0, 0, 1, 0, &TestEventHandler1);
        CHECK(SEvtPushState(0, 0) == 1);
    }

    SECTION("pushed state won't receive callbacks") {
        SEvtRegisterHandler(1337, 420, 69, 0, &TestEventHandler1);
        CHECK(SEvtPushState(1337, 420) == 1);

        CHECK(SEvtDispatch(1337, 420, 69, nullptr) == 0);
        CHECK(test.NumCalls() == 0);
    }

    SECTION("pushed state applies to all ids") {
        SEvtRegisterHandler(0, 0, 9, 0, &TestEventHandler1);
        SEvtRegisterHandler(0, 0, 8, 0, &TestEventHandler1);
        SEvtRegisterHandler(0, 0, 66, 0, &TestEventHandler1);

        CHECK(SEvtPushState(0, 0) == 1);

        CHECK(SEvtDispatch(0, 0, 9, nullptr) == 0);
        CHECK(SEvtDispatch(0, 0, 8, nullptr) == 0);
        CHECK(SEvtDispatch(0, 0, 66, nullptr) == 0);
        CHECK(test.NumCalls() == 0);
    }

    SECTION("pushed state can't be unregistered") {
        SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler1);
        SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler2);

        CHECK(SEvtPushState(0, 0) == 1);
        CHECK(SEvtUnregisterHandler(0, 0, 0, &TestEventHandler1) == 0);
    }
}

static void STORMAPI TestRegisterEventHandler(void* data) {
    EventHandlerTest::RegisterCall(30, data);

    CHECK(SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler4) == 1);
}

TEST_CASE("SEvtRegisterHandler", "[event]") {
    EventHandlerTest test;

    SECTION("registers an event handler") {
        CHECK(SEvtRegisterHandler(7357, 1, 2, 0, &TestEventHandler1) == 1);
        // true if found and removed
        CHECK(SEvtUnregisterHandler(7357, 1, 2, &TestEventHandler1) == 1);
    }

    SECTION("registers event handlers with different types") {
        CHECK(SEvtRegisterHandler(7357, 1, 2, 0, &TestEventHandler1) == 1);
        CHECK(SEvtRegisterHandler(69, 1, 2, 0, &TestEventHandler1) == 1);

        CHECK(SEvtUnregisterHandler(69, 1, 2, &TestEventHandler1) == 1);
        CHECK(SEvtRegisterHandler(777, 1, 2, 0, &TestEventHandler1) == 1);

        CHECK(SEvtUnregisterHandler(7357, 1, 2, &TestEventHandler1) == 1);
        CHECK(SEvtUnregisterHandler(777, 1, 2, &TestEventHandler1) == 1);
    }

    SECTION("adds a new handler while being processed") {
        CHECK(SEvtRegisterHandler(0, 0, 0, 0, &TestEventHandler1) == 1);
        CHECK(SEvtRegisterHandler(0, 0, 0, 0, &TestRegisterEventHandler) == 1);

        SEvtDispatch(0, 0, 0, nullptr);
        CHECK(test.NumCalls() == 2);
        CHECK_THAT(test.CallResult(), MatchesCall({30}));
        CHECK_THAT(test.CallResult(), MatchesCall({1}));

        SEvtDispatch(0, 0, 0, nullptr);
        CHECK(test.NumCalls() == 3);
        CHECK_THAT(test.CallResult(), MatchesCall({4}));
        CHECK_THAT(test.CallResult(), MatchesCall({30}));
        CHECK_THAT(test.CallResult(), MatchesCall({1}));

        SEvtDispatch(0, 0, 0, nullptr);
        CHECK(test.NumCalls() == 4);
        CHECK_THAT(test.CallResult(), MatchesCall({4}));
        CHECK_THAT(test.CallResult(), MatchesCall({4}));
        CHECK_THAT(test.CallResult(), MatchesCall({30}));
        CHECK_THAT(test.CallResult(), MatchesCall({1}));
    }
}

static void STORMAPI TestUnregisterEventHandler(void* data) {
    EventHandlerTest::RegisterCall(30, data);

    CHECK(SEvtUnregisterHandler(0, 0, 0, &TestUnregisterEventHandler) == 1);
}

TEST_CASE("SEvtUnregisterHandler", "[event]") {
    EventHandlerTest test;

    SECTION("does nothing if handler doesn't exist") {
        CHECK(SEvtUnregisterHandler(7357, 2, 1, &TestEventHandler1) == 0);
        CHECK(SEvtUnregisterHandler(7357, 2, 1, nullptr) == 0);
    }

    SECTION("removes multiple handler functions if handler is null") {
        SEvtRegisterHandler(7357, 1, 2, 0, &TestEventHandler1);
        SEvtRegisterHandler(7357, 1, 2, 0, &TestEventHandler2);

        CHECK(SEvtUnregisterHandler(7357, 1, 2, nullptr) == 1);

        // Make sure nothing gets called
        CHECK(SEvtDispatch(7357, 1, 2, nullptr) == 0);
        CHECK(test.NumCalls() == 0);
    }

    SECTION("removes a specific event handler") {
        SEvtRegisterHandler(7357, 1, 2, 0, &TestEventHandler1);
        SEvtRegisterHandler(7357, 1, 2, 0, &TestEventHandler2);

        CHECK(SEvtUnregisterHandler(7357, 1, 2, &TestEventHandler1) == 1);

        // Make sure the other handler wasn't removed by checking if it is called
        CHECK(SEvtDispatch(7357, 1, 2, nullptr) == 1);
        CHECK_THAT(test.CallResult(), MatchesCall({ 2, nullptr }));
    }

    SECTION("removes nothing if only type differs") {
        SEvtRegisterHandler(7357, 1, 2, 0, &TestEventHandler1);

        CHECK(SEvtUnregisterHandler(42, 1, 2, nullptr) == 0);

        CHECK(SEvtDispatch(7357, 1, 2, nullptr) == 1);
        CHECK(test.NumCalls() == 1);
    }

    SECTION("removes nothing if only subtype differs") {
        SEvtRegisterHandler(7357, 1, 2, 0, &TestEventHandler1);

        CHECK(SEvtUnregisterHandler(7357, 2, 2, nullptr) == 0);

        CHECK(SEvtDispatch(7357, 1, 2, nullptr) == 1);
        CHECK(test.NumCalls() == 1);
    }

    SECTION("removes nothing if only id differs") {
        SEvtRegisterHandler(7357, 1, 2, 0, &TestEventHandler1);

        CHECK(SEvtUnregisterHandler(7357, 1, 3, nullptr) == 0);

        CHECK(SEvtDispatch(7357, 1, 2, nullptr) == 1);
        CHECK(test.NumCalls() == 1);
    }

    SECTION("allows a handler to remove itself while being called") {
        SEvtRegisterHandler(0, 0, 0, 0, &TestUnregisterEventHandler);

        CHECK(SEvtDispatch(0, 0, 0, nullptr) == 1);
        CHECK(test.NumCalls() == 1);

        CHECK(SEvtDispatch(0, 0, 0, nullptr) == 0);
    }

    SECTION("stops calls once a handler removes itself") {
        SEvtRegisterHandler(0, 0, 0, 0, &TestUnregisterEventHandler);
        SEvtRegisterHandler(0, 0, 0, 0, &TestUnregisterEventHandler);
        SEvtRegisterHandler(0, 0, 0, 0, &TestUnregisterEventHandler);

        CHECK(SEvtDispatch(0, 0, 0, nullptr) == 1);
        CHECK(test.NumCalls() == 1);
        CHECK(SEvtDispatch(0, 0, 0, nullptr) == 0);
    }
}

static void STORMAPI TestUnregisterTypeHandler(void* data) {
    EventHandlerTest::RegisterCall(30, data);

    CHECK(SEvtUnregisterType(0, 0) == 1);
}

TEST_CASE("SEvtUnregisterType", "[event]") {
    EventHandlerTest test;

    SECTION("removes all event handlers in matching type and subtype") {
        SEvtRegisterHandler(7357, 1, 2, 0, &TestEventHandler1);
        SEvtRegisterHandler(7357, 1, 2, 0, &TestEventHandler2);

        CHECK(SEvtUnregisterType(7357, 1) == 1);

        CHECK(SEvtDispatch(7357, 1, 2, nullptr) == 0);
        CHECK(test.NumCalls() == 0);
    }

    SECTION("removes nothing if only type matches") {
        SEvtRegisterHandler(7357, 2, 2, 0, &TestEventHandler1);

        CHECK(SEvtUnregisterType(7357, 1) == 0);

        CHECK(SEvtDispatch(7357, 2, 2, nullptr) == 1);
        CHECK(test.NumCalls() == 1);
    }

    SECTION("removes nothing if only subtype matches") {
        SEvtRegisterHandler(7357, 1, 2, 0, &TestEventHandler1);

        CHECK(SEvtUnregisterType(42, 1) == 0);

        CHECK(SEvtDispatch(7357, 1, 2, nullptr) == 1);
        CHECK(test.NumCalls() == 1);
    }

    SECTION("removes only matching handlers among many") {
        SEvtRegisterHandler(123, 1, 2, 0, &TestEventHandler1);
        SEvtRegisterHandler(123, 2, 2, 0, &TestEventHandler2);
        SEvtRegisterHandler(123, 2, 3, 0, &TestEventHandler3);
        SEvtRegisterHandler(123, 1, 1, 0, &TestEventHandler4);
        SEvtRegisterHandler(123, 2, 1, 0, &TestEventHandler1);

        CHECK(SEvtUnregisterType(123, 2) == 1);

        CHECK(SEvtDispatch(123, 2, 1, nullptr) == 0);
        CHECK(SEvtDispatch(123, 2, 2, nullptr) == 0);
        CHECK(SEvtDispatch(123, 2, 3, nullptr) == 0);

        CHECK(SEvtDispatch(123, 1, 1, nullptr) == 1);
        CHECK(test.NumCalls() == 1);
        CHECK(SEvtDispatch(123, 1, 2, nullptr) == 1);
        CHECK(test.NumCalls() == 2);
    }

    SECTION("allows a handler to remove itself while being called") {
        SEvtRegisterHandler(0, 0, 0, 0, &TestUnregisterTypeHandler);

        CHECK(SEvtDispatch(0, 0, 0, nullptr) == 1);
        CHECK(test.NumCalls() == 1);

        CHECK(SEvtDispatch(0, 0, 0, nullptr) == 0);
    }

    SECTION("stops calls once a handler removes itself") {
        SEvtRegisterHandler(0, 0, 0, 0, &TestUnregisterTypeHandler);
        SEvtRegisterHandler(0, 0, 0, 0, &TestUnregisterTypeHandler);
        SEvtRegisterHandler(0, 0, 0, 0, &TestUnregisterTypeHandler);

        CHECK(SEvtDispatch(0, 0, 0, nullptr) == 1);
        CHECK(test.NumCalls() == 1);
        CHECK(SEvtDispatch(0, 0, 0, nullptr) == 0);
    }
}
