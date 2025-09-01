#include "storm/Memory.hpp"
#include "test/Test.hpp"

#include <cstring>
#include <numeric>


TEST_CASE("SMemAlloc", "[memory]") {
    SECTION("allocates memory") {
        void* ptr = SMemAlloc(16, __FILE__, __LINE__);
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

TEST_CASE("SMemFree", "[memory]") {
    SECTION("does nothing on nullptr") {
        CHECK_NOTHROW(SMemFree(nullptr));
    }
}

TEST_CASE("SMemFree full args", "[memory]") {
    SECTION("does nothing on nullptr") {
        CHECK_NOTHROW(SMemFree(nullptr, __FILE__, __LINE__));
    }
}

TEST_CASE("SMemReAlloc", "[memory]") {
    SECTION("allocates memory") {
        void* ptr = SMemReAlloc(nullptr, 16, __FILE__, __LINE__);
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
