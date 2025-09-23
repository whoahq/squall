#include "test/Test.hpp"
#include "storm/Transparency.hpp"
#include <string>
#include <sstream>
#include <iostream>

const uint8_t OriginalImageBits[] = {
"...................................................................................................."
"......;&&&&&&&&&&;............................;&&&&&&&&&&+.........................................."
".......xxxx+..:x:xXXXXXXX:..;XXx....;XXXXXx:.+&&&;....::;XXXXXx:....:xXX;..:XXXXXXXX:XXXXXXX;;+:...."
"........+&&&&:.:&&&xxXxxxx:;+X&&$:+xxxxxx&&&+$&&&:...:+xxXxxx&&&+..;+x&&&:+xxXxxxx;:;+xxx+&&&:......"
".........:$&&&x....;&&&+..x&&;x&&;.x&&$.:&&$:$&&&:.....x&&$.:&&$:.;&&+;&&$.:&&&xxx...:&&&x.........."
"...........+&&&&:..;&&&+.;&&&&$&&&:x&&$x&&X..x&&&x.....x&&$x&&X...$&&&&$&&x:&&&x:....:&&&x.........."
".......x$;..:$&&&+.:&&&::$&&...;&&$x&&$.x&&&x.+&&&$:...x&&$.x&&&xx&&x...$&&+&&$:.....:&&&;.........."
".....x&&&&&&&&&&&X..;x:........................:x&&&&x:...............................;x:..........."
"...................................................................................................."
};

const uint8_t OriginalMaskBits[] = {
"##################..........................##############.........................................."
"##################..........................##############.........................................."
"##############..............................#########..............................................."
"##############..............................#########..............................................."
"##################..........................#########..............................................."
"##################...........................########..............................................."
"##################...........................########..............................................."
"##################...........................##########............................................."
"##################...........................##########............................................."
};

const uint8_t OriginalMaskResultBits[] = {
"...................................................................................................."
"......;&&&&&&&&&&;............................;&&&&&&&&&&+.........................................."
".......xxxx+.................................+&&&;.................................................."
"........+&&&&:..............................+$&&&:.................................................."
".........:$&&&x.............................:$&&&:.................................................."
"...........+&&&&:............................x&&&x.................................................."
".......x$;..:$&&&+............................+&&&$:................................................"
".....x&&&&&&&&&&&X.............................:x&&&&x:............................................."
"...................................................................................................."
};

const uint8_t OriginalInvertedMaskResultBits[] = {
"...................................................................................................."
"...................................................................................................."
"..............:x:xXXXXXXX:..;XXx....;XXXXXx:..........::;XXXXXx:....:xXX;..:XXXXXXXX:XXXXXXX;;+:...."
"...............:&&&xxXxxxx:;+X&&$:+xxxxxx&&&.........:+xxXxxx&&&+..;+x&&&:+xxXxxxx;:;+xxx+&&&:......"
"...................;&&&+..x&&;x&&;.x&&$.:&&$...........x&&$.:&&$:.;&&+;&&$.:&&&xxx...:&&&x.........."
"...................;&&&+.;&&&&$&&&:x&&$x&&X............x&&$x&&X...$&&&&$&&x:&&&x:....:&&&x.........."
"...................:&&&::$&&...;&&$x&&$.x&&&x..........x&&$.x&&&xx&&x...$&&+&&$:.....:&&&;.........."
"....................;x:...............................................................;x:..........."
"...................................................................................................."
};

const uint32_t ImageWidth = 100;
const uint32_t ImageHeight = 9;
const uint32_t ImageBytes = ImageWidth * ImageHeight;

std::vector<uint8_t> MakeTestCopy(const uint8_t* data) {
    return std::vector<uint8_t>(data, data + ImageBytes);
}

std::string StringifyResult(const uint8_t* data) {
    std::ostringstream ss;
    for (int y = 0; y < ImageHeight; y++) {
        ss.write(reinterpret_cast<const char*>(data), ImageWidth);
        data += ImageWidth;
        ss << '\n';
    }
    return ss.str();
}

std::string StringifyResult(const std::vector<uint8_t>& data) {
    return StringifyResult(data.data());
}

class TransTest {
public:
    std::vector<uint8_t> ImageBits = MakeTestCopy(OriginalImageBits);
    std::vector<uint8_t> MaskBits = MakeTestCopy(OriginalMaskBits);
    std::vector<uint8_t> MaskResultBits = MakeTestCopy(OriginalMaskResultBits);
    std::vector<uint8_t> InvertedMaskResultBits = MakeTestCopy(OriginalInvertedMaskResultBits);

    ~TransTest() {
        StormDestroy();
    }
};

TEST_CASE("STransBlt", "[transparency]") {
    TransTest t;
    HSTRANS trans;

    SECTION("reproduces trans mask with no colorkey") {
        REQUIRE(STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &trans) == 1);

        std::vector<uint8_t> result = t.ImageBits;
        CHECK(STransBlt(result.data(), 0, 0, ImageWidth, trans) == 1);
        CHECK(result == t.MaskBits);
    }

    SECTION("produces cutout with colorkey") {
        REQUIRE(STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &trans) == 1);

        std::vector<uint8_t> result = t.ImageBits;
        CHECK(STransBlt(result.data(), 0, 0, ImageWidth, trans) == 1);
        CHECK(result == t.MaskResultBits);
    }

    // TODO more
}

TEST_CASE("STransBltUsingMask", "[transparency]") {
    TransTest t;
    HSTRANS trans;

    SECTION("applies inverted mask to output") {
        REQUIRE(STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &trans) == 1);

        std::vector<uint8_t> dest(ImageWidth * ImageHeight, '.');
        std::vector<uint8_t> source = t.ImageBits;
        CHECK(STransBltUsingMask(dest.data(), source.data(), ImageWidth, ImageWidth, trans) == 1);
        CHECK(dest == t.InvertedMaskResultBits);
    }
    // TODO
}

TEST_CASE("STransCombineMasks", "[transparency]") {
    TransTest t;
    HSTRANS trans, trans2;

    SECTION("combines masks") {
        REQUIRE(STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans) == 1);
        REQUIRE(STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &trans2) == 1);

        const std::vector<uint8_t> empty(ImageBytes, '.');

        std::vector<uint8_t> result = empty;
        CHECK(STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans) == 1);
        CHECK(result == t.MaskResultBits);

        result = empty;
        CHECK(STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans2) == 1);
        CHECK(result == t.InvertedMaskResultBits);

        HSTRANS combined = nullptr;
        CHECK(STransCombineMasks(trans, trans2, 0, 0, 0, &combined) == 1); // TODO

        result = empty;
        CHECK(STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, combined) == 1);
        CHECK(result == t.ImageBits);
    }
}

TEST_CASE("STransCreateE", "[transparency]") {
    TransTest t;

    SECTION("fails if bitdepth is not 8") {
        int32_t bitdepth = GENERATE(-1, 0, 1, 2, 4, 7, 9, 16, 32);
        HSTRANS handle = reinterpret_cast<HSTRANS>(12345);
        CHECK_FALSE(STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, bitdepth, nullptr, 0, &handle));
        CHECK(handle == nullptr);
    }

    SECTION("creates a handle") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a 1x1 handle") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateE(t.ImageBits.data(), 1, 1, 8, nullptr, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 10, 2, 90, 7 };
        CHECK(STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a 1x1 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 2, 2 };
        CHECK(STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a 0x0 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 1, 1 };
        CHECK(STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a trans mask opposite to colorkey") {
        HSTRANS trans;
        CHECK(STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &trans) == 1);

        std::vector<uint8_t> result(ImageBytes, '.');
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans);
        CHECK(result == t.InvertedMaskResultBits);
    }
}

TEST_CASE("STransCreateI", "[transparency]") {
    TransTest t;

    SECTION("fails if bitdepth is not 8") {
        int32_t bitdepth = GENERATE(-1, 0, 1, 2, 4, 7, 9, 16, 32);
        HSTRANS handle = reinterpret_cast<HSTRANS>(12345);
        CHECK_FALSE(STransCreateI(t.ImageBits.data(), ImageWidth, ImageHeight, bitdepth, nullptr, 0, &handle));
        CHECK(handle == nullptr);
    }

    SECTION("creates a handle") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateI(t.ImageBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a 1x1 handle") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateI(t.ImageBits.data(), 1, 1, 8, nullptr, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 10, 2, 90, 7 };
        CHECK(STransCreateI(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a 1x1 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 2, 2 };
        CHECK(STransCreateI(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a 0x0 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 1, 1 };
        CHECK(STransCreateI(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a trans mask opposite to colorkey") {
        HSTRANS trans;
        CHECK(STransCreateI(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &trans) == 1);

        std::vector<uint8_t> result(ImageBytes, '.');
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans);
        CHECK(result == t.InvertedMaskResultBits);
    }
}

TEST_CASE("STransCreateMaskE", "[transparency]") {
    TransTest t;

    SECTION("fails if bitdepth is not 8") {
        int32_t bitdepth = GENERATE(-1, 0, 1, 2, 4, 7, 9, 16, 32);
        HSTRANS handle = reinterpret_cast<HSTRANS>(12345);
        CHECK_FALSE(STransCreateMaskE(t.ImageBits.data(), ImageWidth, ImageHeight, bitdepth, nullptr, 0, &handle));
        CHECK(handle == nullptr);
    }

    SECTION("creates a handle") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateMaskE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a 1x1 handle") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateMaskE(t.ImageBits.data(), 1, 1, 8, nullptr, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 10, 2, 90, 7 };
        CHECK(STransCreateMaskE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a 1x1 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 2, 2 };
        CHECK(STransCreateMaskE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a 0x0 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 1, 1 };
        CHECK(STransCreateMaskE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a trans mask that matches colorkey") {
        HSTRANS trans;
        CHECK(STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &trans) == 1);

        std::vector<uint8_t> result(ImageBytes, '.');
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans);
        CHECK(result == t.InvertedMaskResultBits);
    }
}

TEST_CASE("STransCreateMaskI", "[transparency]") {
    TransTest t;

    SECTION("fails if bitdepth is not 8") {
        int32_t bitdepth = GENERATE(-1, 0, 1, 2, 4, 7, 9, 16, 32);
        HSTRANS handle = reinterpret_cast<HSTRANS>(12345);
        CHECK_FALSE(STransCreateMaskI(t.ImageBits.data(), ImageWidth, ImageHeight, bitdepth, nullptr, 0, &handle));
        CHECK(handle == nullptr);
    }

    SECTION("creates a handle") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateMaskI(t.ImageBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a 1x1 handle") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateMaskI(t.ImageBits.data(), 1, 1, 8, nullptr, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 10, 2, 90, 7 };
        CHECK(STransCreateMaskI(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a 1x1 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 2, 2 };
        CHECK(STransCreateMaskI(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a 0x0 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 1, 1 };
        CHECK(STransCreateMaskI(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a trans mask that matches colorkey") {
        HSTRANS trans;
        CHECK(STransCreateMaskI(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &trans) == 1);

        std::vector<uint8_t> result(ImageBytes, '.');
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans);
        CHECK(result == t.InvertedMaskResultBits);
    }
}

TEST_CASE("STransDelete", "[transparency]") {
    TransTest t;

    SECTION("called successfully") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &handle));
        CHECK(STransDelete(handle) == 1);
    }
}

#if !defined(WHOA_TEST_STORMDLL)
TEST_CASE("STransDestroy", "[transparency]") {
    SECTION("called successfully") {
        CHECK(STransDestroy() == 1);
    }
}
#endif

TEST_CASE("STransDuplicate", "[transparency]") {
    TransTest t;
    HSTRANS trans, copy;

    SECTION("duplicates a transparency record") {
        REQUIRE(STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &trans) == 1);

        CHECK(STransDuplicate(trans, &copy) == 1);
        STransDelete(trans);

        std::vector<uint8_t> result = t.ImageBits;
        CHECK(STransBlt(result.data(), 0, 0, ImageWidth, copy) == 1);
        CHECK(result == t.MaskResultBits);
    }
}

TEST_CASE("STransIntersectDirtyArray", "[transparency]") {
    TransTest t;
    HSTRANS trans;

    // TODO
}

TEST_CASE("STransInvertMask", "[transparency]") {
    TransTest t;
    HSTRANS trans;

    SECTION("creates a new transparency record") {
        REQUIRE(STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &trans));

        HSTRANS inverted = nullptr;
        CHECK(STransInvertMask(trans, &inverted) == 1);
        CHECK(inverted != nullptr);
    }

    SECTION("inverts a mask") {
        REQUIRE(STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans));

        std::vector<uint8_t> dest(ImageWidth * ImageHeight, '.');
        std::vector<uint8_t> source = t.ImageBits;
        CHECK(STransBltUsingMask(dest.data(), source.data(), ImageWidth, ImageWidth, trans));
        CHECK(dest == t.MaskResultBits);

        HSTRANS inverted;
        REQUIRE(STransInvertMask(trans, &inverted) == 1);

        std::vector<uint8_t> dest2(ImageWidth * ImageHeight, '.');
        CHECK(STransBltUsingMask(dest2.data(), source.data(), ImageWidth, ImageWidth, inverted));
        CHECK(dest2 == t.InvertedMaskResultBits);
    }
}

TEST_CASE("STransIsPixelInMask", "[transparency]") {
    TransTest t;
    HSTRANS trans;

    SECTION("false if coordinates are invalid") {
        auto pos = GENERATE(
            std::make_pair(-1, 0),
            std::make_pair(0, -1),
            std::make_pair(1, 0),
            std::make_pair(0, 1),
            std::make_pair(1, 1),
            std::make_pair(-1, -1)
        );
        STransCreateE(t.ImageBits.data(), 1, 1, 8, nullptr, 0, &trans);
        CHECK_FALSE(STransIsPixelInMask(trans, pos.first, pos.second));
    }

    SECTION("true if coordinates are valid") {
        auto pos = GENERATE(
            std::make_pair(0, 0),
            std::make_pair(0, 4),
            std::make_pair(4, 0),
            std::make_pair(4, 4)
        );
        STransCreateE(t.ImageBits.data(), 5, 5, 8, nullptr, 0, &trans);
        CHECK(STransIsPixelInMask(trans, pos.first, pos.second));
    }

    SECTION("false if coordinates are invalid on rect and colorkey") {
        auto pos = GENERATE(
            std::make_pair(0, 4),
            std::make_pair(44, 3),
            std::make_pair(24, 0),
            std::make_pair(37, 0),
            std::make_pair(32, 4),
            std::make_pair(25, 4)
        );
        RECT rct = { 20, 1, 64, 5 };
        STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('#'), &trans);

        std::ostringstream ss("Pos: ");
        ss << pos.first << ", " << pos.second;
        INFO(ss.str());

        CHECK_FALSE(STransIsPixelInMask(trans, pos.first, pos.second));
    }

    SECTION("true if coordinates are valid on rect and colorkey") {
        auto pos = GENERATE(
            std::make_pair(23, 0),
            std::make_pair(38, 0),
            std::make_pair(33, 4),
            std::make_pair(24, 4)
        );
        RECT rct = { 20, 1, 64, 6 };
        STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('#'), &trans);

        std::ostringstream ss("Pos: ");
        ss << pos.first << ", " << pos.second;
        INFO(ss.str());

        CHECK(STransIsPixelInMask(trans, pos.first, pos.second));
    }

    SECTION("true if coordinates are valid on rect and colorkey (inverted)") {
        auto pos = GENERATE(
            std::make_pair(24, 0),
            std::make_pair(37, 0),
            std::make_pair(32, 4),
            std::make_pair(25, 4)
        );
        RECT rct = { 20, 1, 64, 6 };
        // Inverted mask using colour key here
        STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('.'), &trans);

        std::ostringstream ss("Pos: ");
        ss << pos.first << ", " << pos.second;
        INFO(ss.str());

        CHECK(STransIsPixelInMask(trans, pos.first, pos.second));
    }
}

TEST_CASE("STransSetDirtyArrayInfo", "[transparency]") {
    TransTest t;
    HSTRANS trans;

    // TODO
}
