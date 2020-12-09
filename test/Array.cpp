#include "storm/Array.hpp"
#include "storm/String.hpp"
#include "test/Test.hpp"

TEST_CASE("TSBaseArray", "[array]") {
    SECTION("constructs correctly") {
        TSBaseArray<uint32_t> array;
        REQUIRE(array.Count() == 0);
    }
}

TEST_CASE("TSBaseArray::MemFileName", "[array]") {
    SECTION("returns a non-empty string") {
        TSBaseArray<uint32_t> array;
        REQUIRE(SStrLen(array.MemFileName()) > 0);
    }
}

TEST_CASE("TSBaseArray::MemLineNo", "[array]") {
    SECTION("returns a negative number") {
        TSBaseArray<uint32_t> array;
        REQUIRE(array.MemLineNo() < 0);
    }
}

TEST_CASE("TSFixedArray", "[array]") {
    SECTION("constructs correctly") {
        TSFixedArray<uint32_t> array;
        REQUIRE(array.Count() == 0);
    }
}

TEST_CASE("TSGrowableArray", "[array]") {
    SECTION("constructs correctly") {
        TSGrowableArray<uint32_t> array;
        REQUIRE(array.Count() == 0);
    }
}
