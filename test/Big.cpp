#include "storm/big/Ops.hpp"
#include "test/Test.hpp"

TEST_CASE("ExtractLowPart", "[big]") {
    SECTION("extracts low part of 0") {
        uint64_t value = 0;
        auto low = ExtractLowPart(value);

        REQUIRE(low == 0);
        REQUIRE(value == 0);
    }

    SECTION("extracts low part of 0x12345678") {
        uint64_t value = 0x12345678;
        auto low = ExtractLowPart(value);

        REQUIRE(low == 0x12345678);
        REQUIRE(value == 0);
    }

    SECTION("extracts low part of 0xAAAABBBBCCCCDDDD") {
        uint64_t value = 0xAAAABBBBCCCCDDDD;
        auto low = ExtractLowPart(value);

        REQUIRE(low == 0xCCCCDDDD);
        REQUIRE(value == 0xAAAABBBB);
    }
}

TEST_CASE("ExtractLowPartSx", "[big]") {
    SECTION("extracts low part of 0") {
        uint64_t value = 0;
        auto low = ExtractLowPartSx(value);

        REQUIRE(low == 0);
        REQUIRE(value == 0);
    }

    SECTION("extracts low part of 0x12345678") {
        uint64_t value = 0x12345678;
        auto low = ExtractLowPartSx(value);

        REQUIRE(low == 0x12345678);
        REQUIRE(value == 0);
    }

    SECTION("extracts low part of 0xAAAABBBBCCCCDDDD") {
        uint64_t value = 0xAAAABBBBCCCCDDDD;
        auto low = ExtractLowPartSx(value);

        REQUIRE(low == 0xCCCCDDDD);
        REQUIRE(value == 0xFFFFFFFFAAAABBBB);
    }
}

TEST_CASE("MakeLarge", "[big]") {
    SECTION("creates uint64_t out of 0xAABBCCDD and 0x11223344") {
        uint64_t value = MakeLarge(0xAABBCCDD, 0x11223344);

        REQUIRE(value == 0x11223344AABBCCDD);
    }

    SECTION("creates uint64_t out of 0 and 0x11223344") {
        uint64_t value = MakeLarge(0x00000000, 0x11223344);

        REQUIRE(value == 0x1122334400000000);
    }
}
