#include "storm/Memory.hpp"
#include "test/Test.hpp"

TEST_CASE("SMemAlloc", "[memory]") {
    SECTION("allocates memory") {
        void* ptr = SMemAlloc(16, __FILE__, __LINE__, 0x0);
        REQUIRE(ptr != nullptr);
        SMemFree(ptr);
    }
}
