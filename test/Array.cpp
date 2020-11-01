#include "storm/Array.hpp"
#include "test/Test.hpp"

TEST_CASE("TSBaseArray", "[list]") {
    SECTION("constructs correctly") {
        TSBaseArray<uint32_t> array;
        REQUIRE(array.Count() == 0);
    }
}

TEST_CASE("TSFixedArray", "[list]") {
    SECTION("constructs correctly") {
        TSFixedArray<uint32_t> array;
        REQUIRE(array.Count() == 0);
    }
}

TEST_CASE("TSGrowableArray", "[list]") {
    SECTION("constructs correctly") {
        TSGrowableArray<uint32_t> array;
        REQUIRE(array.Count() == 0);
    }
}
