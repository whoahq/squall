#include "storm/Atomic.hpp"
#include "test/Test.hpp"

TEST_CASE("SInterlockedDecrement", "[atomic]") {
    SECTION("decrements value") {
        int32_t value = 1;
        auto decremented = SInterlockedDecrement(&value);
        CHECK(value == 0);
        CHECK(decremented == 0);
    }
}

TEST_CASE("SInterlockedIncrement", "[atomic]") {
    SECTION("increments value") {
        int32_t value = { 1 };
        auto incremented = SInterlockedIncrement(&value);
        CHECK(value == 2);
        CHECK(incremented == 2);
    }
}
