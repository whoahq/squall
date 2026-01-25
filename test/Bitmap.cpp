#include "test/Test.hpp"
#include "storm/Bitmap.hpp"
#include <sstream>

TEST_CASE("SBmpAllocLoadImage", "[bitmap]") {
    SECTION("loads image from MPQ") {
    }

    SECTION("loads image from filesystem") {
    }

    SECTION("fails when image not found") {
    }

    SECTION("populates output args") {
    }

    SECTION("clears output args when format not supported") {
    }

    SECTION("calls custom allocation function when provided") {
    }
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

    SECTION("decodes a PCX image") {
    }

    SECTION("populates output args") {
    }

    SECTION("retrieves palette for 8 bit images") {
    }

    SECTION("ignores palette if bit depth is not 8") {
    }
}

TEST_CASE("SBmpLoadImage", "[bitmap]") {
    SECTION("loads image from MPQ") {
    }

    SECTION("loads image from filesystem") {
    }

    SECTION("fails when image not found") {
    }

    SECTION("populates output args") {
    }

    SECTION("clears output args when format not supported") {
    }

    SECTION("retrieves args even if buffer unspecified") {
    }
}

struct AlignTestCase {
    int32_t width, align, result;
};

std::string GetAlignTestcaseStr(const AlignTestCase &input) {
    std::ostringstream ss;
    ss << "SBmpGetPitchForAlignment(" << input.width << ", " << input.align << ")";
    return ss.str();
}

#if !defined(WHOA_STORMDLL_VERSION) || WHOA_STORMDLL_VERSION >= 2016
TEST_CASE("SBmpGetPitchForAlignment", "[bitmap]") {
    SECTION("returns width if divisible by alignment") {
        auto input = GENERATE(
            AlignTestCase { 0, 1, 0 },
            AlignTestCase { 1, 1, 1 },
            AlignTestCase { 2, 1, 2 },
            AlignTestCase { 99, 1, 99 },
            AlignTestCase { -1, 1, -1 },
            AlignTestCase { 2, 2, 2 },
            AlignTestCase { -20, 4, -20 },
            AlignTestCase { 12, 4, 12 },
            AlignTestCase { 16, 8, 16 }
        );
        INFO(GetAlignTestcaseStr(input));
        CHECK(SBmpGetPitchForAlignment(input.width, input.align) == input.result);
    }

    SECTION("returns width rounded up to alignment") {
        auto input = GENERATE(
            AlignTestCase { 1, 2, 2 },
            AlignTestCase { 1, 4, 4 },
            AlignTestCase { -1, 4, 4 },
            AlignTestCase { -3, 4, 4 },
            AlignTestCase { 18, 4, 20 },
            AlignTestCase { 16, 5, 20 },
            AlignTestCase { -53, 100, 100 },
            AlignTestCase { 152, 100, 200 }
        );
        INFO(GetAlignTestcaseStr(input));
        CHECK(SBmpGetPitchForAlignment(input.width, input.align) == input.result);
    }
}

TEST_CASE("SBmpPadImage", "[bitmap]") {
    SECTION("does nothing if width is aligned") {
    }

    SECTION("pads rows if width is not aligned") {
    }
}
#endif
