#include "storm/Array.hpp"
#include "storm/String.hpp"
#include "test/Test.hpp"

struct TestArrayObject {
    uint32_t index = 0;
};

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

TEST_CASE("TSGrowableArray::Reserve", "[array]") {
    SECTION("reserves slot when array is empty") {
        TSGrowableArray<uint32_t> array;
        array.Reserve(1, 1);
        REQUIRE(array.Count() == 0);
    }

    SECTION("reserves slot when array has elements") {
        TSGrowableArray<TestArrayObject> array;

        auto elementA = array.New();
        elementA->index = 10;

        array.Reserve(1, 1);

        auto elementB = array.New();
        elementB->index = 20;

        REQUIRE(array.Count() == 2);
        REQUIRE(array[0].index == 10);
        REQUIRE(array[1].index == 20);
    }
}
