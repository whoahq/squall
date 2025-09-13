#include "storm/Memory.hpp"
#include "test/Test.hpp"

#include <cstring>
#include <numeric>
#include <vector>

TEST_CASE("operator new", "[memory]") {
    SECTION("allocates memory") {
        auto ptr = new int32_t;
        REQUIRE(ptr != nullptr);

        delete ptr;
    }
}

TEST_CASE("operator new[]", "[memory]") {
    SECTION("allocates memory") {
        auto ptr = new int32_t[10];
        REQUIRE(ptr != nullptr);

        delete[] ptr;
    }
}

TEST_CASE("SMemAlloc", "[memory]") {
    SECTION("allocates memory") {
        void* ptr = SMemAlloc(16, __FILE__, __LINE__);
        REQUIRE(ptr != nullptr);
        CHECK_NOTHROW(memset(ptr, 1, 16));
        SMemFree(ptr);
    }

    SECTION("allocates memory with null filename") {
        void* ptr = SMemAlloc(16, nullptr, 0);
        REQUIRE(ptr != nullptr);
        CHECK_NOTHROW(memset(ptr, 1, 16));
        SMemFree(ptr);
    }

    SECTION("allocates memory initialized to 0 with flag") {
        void* ptr = SMemAlloc(16, __FILE__, __LINE__, SMEM_FLAG_ZEROMEMORY);
        REQUIRE(ptr != nullptr);

        uint8_t* pArray = static_cast<uint8_t*>(ptr);
        CHECK(std::accumulate(pArray, pArray + 16, 0u) == 0);

        SMemFree(ptr);
    }

    SECTION("allocates a memory pointer even if the size is 0") {
        void* ptr = SMemAlloc(0, __FILE__, __LINE__);
        REQUIRE(ptr != nullptr);
        SMemFree(ptr);
    }
}

TEST_CASE("SMemCopy", "[memory]") {
    std::vector<uint8_t> data = { 1, 255, 128, 42, 69, 99, 13, 37 };

    SECTION("copies memory") {
        std::vector<uint8_t> dest(8);
        SMemCopy(dest.data(), data.data(), 8);

        CHECK_THAT(dest, Catch::Matchers::Equals(data));
    }

    SECTION("copies nothing if size is 0") {
        std::vector<uint8_t> dest(8);
        SMemCopy(dest.data(), data.data(), 0);

        CHECK(std::accumulate(dest.begin(), dest.end(), 0u) == 0);
    }

    SECTION("copies overlapping memory right") {
        std::vector<uint8_t> result = { 1, 255, 1, 255, 128, 42, 13, 37 };
        SMemCopy(data.data() + 2, data.data(), 4);

        CHECK_THAT(data, Catch::Matchers::Equals(result));
    }

    SECTION("copies overlapping memory left") {
        std::vector<uint8_t> result = { 1, 255, 69, 99, 13, 37, 13, 37 };
        SMemCopy(data.data() + 2, data.data() + 4, 4);

        CHECK_THAT(data, Catch::Matchers::Equals(result));
    }
}

TEST_CASE("SMemFill", "[memory]") {
    std::vector<uint8_t> data = { 1, 255, 128, 42, 69, 99, 13, 37 };

    SECTION("replaces bytes") {
        std::vector<uint8_t> result1(8, 255);
        SMemFill(data.data(), 8, 255);
        CHECK_THAT(data, Catch::Matchers::Equals(result1));

        std::vector<uint8_t> result2 = { 0, 0, 0, 0, 0, 255, 255, 255 };
        SMemFill(data.data(), 5, 0);
        CHECK_THAT(data, Catch::Matchers::Equals(result2));
    }

    SECTION("does nothing if size is 0") {
        std::vector<uint8_t> changedata = data;
        SMemFill(changedata.data(), 0, 255);

        CHECK_THAT(changedata, Catch::Matchers::Equals(data));
    }
}

TEST_CASE("SMemFree", "[memory]") {
    SECTION("does nothing on nullptr") {
        CHECK_NOTHROW(SMemFree(nullptr));
    }
}

TEST_CASE("SMemFree full args", "[memory]") {
    SECTION("does nothing on nullptr") {
        CHECK_NOTHROW(SMemFree(nullptr, __FILE__, __LINE__));
    }

    SECTION("can take a null filename") {
        void* ptr = STORM_ALLOC(10);
        CHECK_NOTHROW(SMemFree(ptr, nullptr, 0));
    }
}

TEST_CASE("SMemMove", "[memory]") {
    std::vector<uint8_t> data = { 1, 255, 128, 42, 69, 99, 13, 37 };

    SECTION("copies memory") {
        std::vector<uint8_t> dest(8);
        SMemMove(dest.data(), data.data(), 8);

        CHECK_THAT(dest, Catch::Matchers::Equals(data));
    }

    SECTION("copies nothing if size is 0") {
        std::vector<uint8_t> dest(8);
        SMemMove(dest.data(), data.data(), 0);

        CHECK(std::accumulate(dest.begin(), dest.end(), 0u) == 0);
    }

    SECTION("copies overlapping memory right") {
        std::vector<uint8_t> result = { 1, 255, 1, 255, 128, 42, 13, 37 };
        SMemMove(data.data() + 2, data.data(), 4);

        CHECK_THAT(data, Catch::Matchers::Equals(result));
    }

    SECTION("copies overlapping memory left") {
        std::vector<uint8_t> result = { 1, 255, 69, 99, 13, 37, 13, 37 };
        SMemMove(data.data() + 2, data.data() + 4, 4);

        CHECK_THAT(data, Catch::Matchers::Equals(result));
    }
}

TEST_CASE("SMemReAlloc", "[memory]") {
    SECTION("allocates memory") {
        void* ptr = SMemReAlloc(nullptr, 16, __FILE__, __LINE__);
        REQUIRE(ptr != nullptr);
        CHECK_NOTHROW(memset(ptr, 1, 16));
        SMemFree(ptr);
    }

    SECTION("allocates memory with null filename") {
        void* ptr = SMemReAlloc(nullptr, 16, nullptr, 0);
        REQUIRE(ptr != nullptr);
        CHECK_NOTHROW(memset(ptr, 1, 16));
        SMemFree(ptr);
    }

    SECTION("allocates memory initialized to 0 with flag") {
        void* ptr = SMemReAlloc(nullptr, 16, __FILE__, __LINE__, SMEM_FLAG_ZEROMEMORY);
        REQUIRE(ptr != nullptr);

        uint8_t* pArray = static_cast<uint8_t*>(ptr);
        CHECK(std::accumulate(pArray, pArray + 16, 0u) == 0);

        SMemFree(ptr);
    }

    SECTION("allocates a memory pointer even if the size is 0") {
        void* ptr = SMemReAlloc(nullptr, 0, __FILE__, __LINE__);
        REQUIRE(ptr != nullptr);
        SMemFree(ptr);
    }

    SECTION("reallocates memory") {
        void* ptr = SMemAlloc(16, __FILE__, __LINE__);
        REQUIRE(ptr != nullptr);

        *static_cast<uint32_t*>(ptr) = 123456;

        // 1MB
        void* ptr2 = SMemReAlloc(ptr, 1024 * 1024, __FILE__, __LINE__);
        REQUIRE(ptr2 != nullptr);

        CHECK(*static_cast<uint32_t*>(ptr2) == 123456);
        CHECK_NOTHROW(memset(ptr2, 1, 1024 * 1024));

        SMemFree(ptr2);
    }
}

TEST_CASE("SMemZero", "[memory]") {
    std::vector<uint8_t> data = { 1, 255, 128, 42, 69, 99, 13, 37 };

    SECTION("zeroes out memory") {
        std::vector<uint8_t> result = { 0, 0, 0, 0, 0, 99, 13, 37 };
        SMemZero(data.data(), 5);

        CHECK_THAT(data, Catch::Matchers::Equals(result));
    }

    SECTION("does nothing if size is 0") {
        std::vector<uint8_t> change = data;
        SMemZero(change.data(), 0);

        CHECK_THAT(change, Catch::Matchers::Equals(data));
    }
}
