#include "test/Test.hpp"
#include "storm/Bitmap.hpp"

TEST_CASE("SBmpAllocLoadImage", "[bitmap]") {
}

TEST_CASE("SBmpDecodeImage", "[bitmap]") {
    SECTION("does nothing for invalid format") {
        uint8_t data = 5;
        CHECK_FALSE(SBmpDecodeImage(99, &data, 1, nullptr, nullptr, 0));
    }

    SECTION("resets numeric output arguments") {
        uint8_t data = 5;

        uint32_t width = 100, height = 100, bitdepth = 100;
        CHECK_FALSE(SBmpDecodeImage(99, &data, 1, nullptr, nullptr, 0, &width, &height, &bitdepth));

        CHECK(width == 0);
        CHECK(height == 0);
        CHECK(bitdepth == 0);
    }
}

TEST_CASE("SBmpLoadImage", "[bitmap]") {
}

#if !defined(WHOA_STORMDLL_VERSION) || WHOA_STORMDLL_VERSION >= 2016
TEST_CASE("SBmpGetPitchForAlignment", "[bitmap]") {
}

TEST_CASE("SBmpPadImage", "[bitmap]") {
}
#endif
