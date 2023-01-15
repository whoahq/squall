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

TEST_CASE("TSFixedArray::Top", "[array]") {
    SECTION("returns nullptr when array has no elements") {
        TSFixedArray<uint32_t> array;
        array.SetCount(0);
        REQUIRE(array.Top() == nullptr);
    }

    SECTION("returns first element when array has 1 element") {
        TSFixedArray<uint32_t> array;
        array.SetCount(1);
        array[0] = 1000;
        REQUIRE(*array.Top() == 1000);
    }

    SECTION("returns last element when array has 2 elements") {
        TSFixedArray<uint32_t> array;
        array.SetCount(2);
        array[0] = 1000;
        array[1] = 1001;
        REQUIRE(*array.Top() == 1001);
    }
}

TEST_CASE("TSGrowableArray", "[array]") {
    SECTION("constructs correctly") {
        TSGrowableArray<uint32_t> array;
        REQUIRE(array.Count() == 0);
    }
}
