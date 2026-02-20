#include "test/Test.hpp"
#include "storm/Error.hpp"
#include "storm/Memory.hpp"
#include "storm/Option.hpp"

#include <climits>
#include <set>

#if defined(WHOA_FLAVOR_GENERIC) || defined(WHOA_FLAVOR_WOW)
// 8 is skipped
#define ALL_OPTIONS 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13
#define LAST_OPTION 13
#elif defined(WHOA_FLAVOR_D2)
#define ALL_OPTIONS 1, 2, 3, 4, 5
#define LAST_OPTION 5
#else
#define ALL_OPTIONS 1, 2, 3, 4, 5, 6
#define LAST_OPTION 6
#endif

const std::set<int32_t> boolean_options = { 9, 10, 11, 12, 13 };

TEST_CASE("StormSetOption", "[option]") {
    SECTION("sets options") {
        int32_t optname = GENERATE(ALL_OPTIONS);
        INFO(optname);

        int32_t optval[2] = { 256, 456 };
        int32_t resultval[2] = { 0, 0 };
        uint32_t resultlen = 8;
        if (optname == 6) {
            CHECK(StormSetOption(optname, &optval, 8) == 1);
            CHECK(StormGetOption(optname, &resultval, &resultlen) == 1);
            CHECK(resultval[0] == 256);
            CHECK(resultval[1] == 456);
        }
        else {
            CHECK(StormSetOption(optname, &optval, 4) == 1);
            CHECK(StormGetOption(optname, &resultval, &resultlen) == 1);
            if (boolean_options.count(optname) != 0) {
                CHECK(resultval[0] == 1);
            }
            else {
                CHECK(resultval[0] == 256);
            }
            CHECK(resultval[1] == 0);
        }
    }

    SECTION("fails if wave chunk size is not a power of 2") {
        uint32_t optval = GENERATE(0, 3, 5, 6, 7, 9, 10, 11, 12, 15, 20, 30, 62, 100, UINT_MAX);
        INFO(optval);
        CHECK_FALSE(StormSetOption(2, &optval, 4));
    }

    SECTION("succeeds if wave chunk size is a power of 2") {
        uint32_t optval = GENERATE(1, 2, 4, 8, 16, 32, UINT_MAX / 2 + 1);
        INFO(optval);
        CHECK(StormSetOption(2, &optval, 4) == 1);
    }

    SECTION("fails if len is not matching") {
        int32_t optname = GENERATE(ALL_OPTIONS);

        int32_t optval;
        if (optname == 6) {
            CHECK_FALSE(StormSetOption(optname, &optval, 7));
            CHECK_FALSE(StormSetOption(optname, &optval, 9));
        }
        else {
            CHECK_FALSE(StormSetOption(optname, &optval, 3));
            CHECK_FALSE(StormSetOption(optname, &optval, 5));
        }
        CHECK(SErrGetLastError() == ERROR_INVALID_PARAMETER);
    }

    SECTION("fails for any len not matching") {
        uint32_t optlen = GENERATE(0, 2, 1, 4, 7, 9, 10, 16);

        int32_t optval = 999;
        CHECK_FALSE(StormSetOption(6, &optval, optlen));
        CHECK(SErrGetLastError() == ERROR_INVALID_PARAMETER);
    }

// The DLL versions always succeeds invalid opcodes, different from WoW
#if defined(WHOA_BAD_STORM_OPTION_ALWAYS_SUCCEEDS)
    SECTION("succeeds if optname is invalid") {
        int32_t optname = GENERATE(0, -1, 8, LAST_OPTION + 1);

        int32_t optval = 999;
        CHECK(StormSetOption(optname, &optval, sizeof(optval)) == 1);
        CHECK(SErrGetLastError() == ERROR_SUCCESS);
    }
#else
    SECTION("fails if optname is invalid") {
        int32_t optname = GENERATE(0, -1, 8, LAST_OPTION + 1);

        int32_t optval = 999;
        CHECK_FALSE(StormSetOption(optname, &optval, sizeof(optval)));
        CHECK(SErrGetLastError() == ERROR_INVALID_PARAMETER);
    }
#endif
}

TEST_CASE("StormGetOption", "[option]") {
#if defined(WHOA_TRACKS_ALLOC_COUNTS)
    SECTION("gets alloc and free count") {
        int32_t optval[2];
        uint32_t optlen = sizeof(optval);
        StormGetOption(6, &optval, &optlen);

        int32_t allocated = optval[0];
        int32_t freed = optval[1];

        void* mem = STORM_ALLOC(32);

        StormGetOption(6, &optval, &optlen);
        CHECK(optval[0] == allocated + 1);
        CHECK(optval[1] == freed);

        mem = SMemReAlloc(mem, 0x10000, __FILE__, __LINE__);
        StormGetOption(6, &optval, &optlen);
        CHECK(optval[0] == allocated + 2);
        CHECK(optval[1] == freed + 1);

        STORM_FREE(mem);

        StormGetOption(6, &optval, &optlen);
        CHECK(optval[0] == allocated + 2);
        CHECK(optval[1] == freed + 2);
    }
#endif

    SECTION("fails if len is too small") {
        int32_t optname = GENERATE(ALL_OPTIONS);

        int32_t optval[2] = { 999, 999 };
        uint32_t optlen = 3;
        if (optname == 6) {
            optlen = 7;
            CHECK_FALSE(StormGetOption(optname, &optval, &optlen));
            CHECK(optval[0] == 999);
            CHECK(optval[1] == 999);
            CHECK(optlen == 7);
        }
        else {
            CHECK_FALSE(StormGetOption(optname, &optval, &optlen));
            CHECK(optval[0] == 999);
            CHECK(optval[1] == 999);
            CHECK(optlen == 3);
        }
        CHECK(SErrGetLastError() == ERROR_INVALID_PARAMETER);
    }

    SECTION("fails for any len too small") {
        uint32_t optlen = GENERATE(2, 1, 4, 7);

        int32_t optval = 999;
        CHECK_FALSE(StormGetOption(6, &optval, &optlen));
        CHECK(optval == 999);
        CHECK(SErrGetLastError() == ERROR_INVALID_PARAMETER);
    }

// The DLL versions always succeeds invalid opcodes, different from WoW
#if defined(WHOA_BAD_STORM_OPTION_ALWAYS_SUCCEEDS)
    SECTION("succeeds if optname is invalid") {
        int32_t optname = GENERATE(0, -1, 8, LAST_OPTION + 1);

        int32_t optval = 999;
        uint32_t optlen = sizeof(optval);
        CHECK(StormGetOption(optname, &optval, &optlen) == 1);
        CHECK(optval == 999);
        CHECK(optlen == 4);
        CHECK(SErrGetLastError() == ERROR_SUCCESS);
    }
#else
    SECTION("fails if optname is invalid") {
        int32_t optname = GENERATE(0, -1, 8, LAST_OPTION + 1);

        int32_t optval = 999;
        uint32_t optlen = sizeof(optval);
        CHECK_FALSE(StormGetOption(optname, &optval, &optlen));
        CHECK(optval == 999);
        CHECK(optlen == 4);
        CHECK(SErrGetLastError() == ERROR_INVALID_PARAMETER);
    }
#endif
}
