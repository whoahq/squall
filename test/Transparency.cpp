#include "test/Test.hpp"
#include "storm/Transparency.hpp"
#include <string>
#include <sstream>
#include <iostream>

uint8_t MemeImageBits[] = {
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

uint8_t MemeMaskBits[] = {
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

uint8_t MemeMaskResultBits[] = {
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

uint8_t MemeInvertedMaskResultBits[] = {
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

const uint32_t MemeImageWidth = 100;
const uint32_t MemeImageHeight = 9;

std::string DbgMeme(uint8_t* data) {
    std::ostringstream ss;
    for (int y = 0; y < MemeImageHeight; y++) {
        ss.write(reinterpret_cast<char*>(data), MemeImageWidth);
        ss << '\n';
    }
    return ss.str();
}

// Currently broken, produces the following:
//....................................................................................................
//.................;............................;&&&&&&&&&&+..........................................
//......................XXX:..;XXx....;XXXXXx:.+&&&;....::;XXXXXx:....:xXX;..:XXXXXXXX:XXXXXXX;;+:....
//.............:........xxxx:;+X&&$:+xxxxxx&&&+$&&&:...:+xxXxxx&&&+..;+x&&&:+xxXxxxx;:;+xxx+&&&:......
//..........................x&&;x&&;.x&&$.:&&$:$&&&:.....x&&$.:&&$:.;&&+;&&$.:&&&xxx...:&&&x..........
//.........................;&&&&$&&&:x&&$x&&X..x&&&x.....x&&$x&&X...$&&&&$&&x:&&&x:....:&&&x..........
//.................+.......$&&...;&&$x&&$.x&&&x.+&&&$:...x&&$.x&&&xx&&x...$&&+&&$:.....:&&&;..........
//.................X.............................:x&&&&x:...............................;x:...........
//....................................................................................................

TEST_CASE("STransBlt", "[transparency]") {
    SECTION("reproduces trans with no colorkey") {
        HSTRANS trans;
        REQUIRE(STransCreateE(MemeMaskBits, MemeImageWidth, MemeImageHeight, 8, nullptr, '#' | 0x1000000, &trans) == 1);

        std::vector<uint8_t> result(std::begin(MemeImageBits), std::end(MemeImageBits));
        CHECK(STransBlt(result.data(), 0, 0, MemeImageWidth, trans) == 1);
        CHECK(result == std::vector<uint8_t>(std::begin(MemeMaskResultBits), std::end(MemeMaskResultBits)));
    }
    // TODO
}

TEST_CASE("STransBltUsingMask", "[transparency]") {
    SECTION("applies mask to output") {
        HSTRANS trans;
        REQUIRE(STransCreateE(MemeMaskBits, MemeImageWidth, MemeImageHeight, 8, nullptr, '#' | 0x1000000, &trans) == 1);

        std::vector<uint8_t> dest(MemeImageWidth * MemeImageHeight, '.');
        std::vector<uint8_t> source(std::begin(MemeImageBits), std::end(MemeImageBits));
        CHECK(STransBltUsingMask(dest.data(), source.data(), MemeImageWidth, MemeImageWidth, trans) == 1);
        CHECK(dest == std::vector<uint8_t>(std::begin(MemeMaskResultBits), std::end(MemeMaskResultBits)));
    }
    // TODO
}

TEST_CASE("STransCombineMasks", "[transparency]") {
    // TODO
}

TEST_CASE("STransCreateE", "[transparency]") {
    SECTION("fails if bitdepth is not 8") {
        int32_t bitdepth = GENERATE(-1, 0, 1, 2, 4, 7, 9, 16, 32);
        HSTRANS handle = reinterpret_cast<HSTRANS>(12345);
        CHECK_FALSE(STransCreateE(MemeImageBits, MemeImageWidth, MemeImageHeight, bitdepth, nullptr, 0, &handle));
        CHECK(handle == nullptr);
    }

    SECTION("creates a handle") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateE(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, nullptr, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }

    SECTION("creates a 1x1 handle") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateE(MemeImageBits, 1, 1, 8, nullptr, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }

    SECTION("creates a handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 10, 2, 90, 7 };
        CHECK(STransCreateE(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }

    SECTION("creates a 1x1 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 2, 2 };
        CHECK(STransCreateE(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }

    SECTION("creates a 0x0 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 1, 1 };
        CHECK(STransCreateE(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }
}

TEST_CASE("STransCreateI", "[transparency]") {
    SECTION("fails if bitdepth is not 8") {
        int32_t bitdepth = GENERATE(-1, 0, 1, 2, 4, 7, 9, 16, 32);
        HSTRANS handle = reinterpret_cast<HSTRANS>(12345);
        CHECK_FALSE(STransCreateI(MemeImageBits, MemeImageWidth, MemeImageHeight, bitdepth, nullptr, 0, &handle));
        CHECK(handle == nullptr);
    }

    SECTION("creates a handle") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateI(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, nullptr, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }

    SECTION("creates a 1x1 handle") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateI(MemeImageBits, 1, 1, 8, nullptr, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }

    SECTION("creates a handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 10, 2, 90, 7 };
        CHECK(STransCreateI(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }

    SECTION("creates a 1x1 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 2, 2 };
        CHECK(STransCreateI(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }

    SECTION("creates a 0x0 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 1, 1 };
        CHECK(STransCreateI(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }
}

TEST_CASE("STransCreateMaskE", "[transparency]") {
    SECTION("fails if bitdepth is not 8") {
        int32_t bitdepth = GENERATE(-1, 0, 1, 2, 4, 7, 9, 16, 32);
        HSTRANS handle = reinterpret_cast<HSTRANS>(12345);
        CHECK_FALSE(STransCreateMaskE(MemeImageBits, MemeImageWidth, MemeImageHeight, bitdepth, nullptr, 0, &handle));
        CHECK(handle == nullptr);
    }

    SECTION("creates a handle") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateMaskE(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, nullptr, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }

    SECTION("creates a 1x1 handle") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateMaskE(MemeImageBits, 1, 1, 8, nullptr, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }

    SECTION("creates a handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 10, 2, 90, 7 };
        CHECK(STransCreateMaskE(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }

    SECTION("creates a 1x1 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 2, 2 };
        CHECK(STransCreateMaskE(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }

    SECTION("creates a 0x0 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 1, 1 };
        CHECK(STransCreateMaskE(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }
}

TEST_CASE("STransCreateMaskI", "[transparency]") {
    SECTION("fails if bitdepth is not 8") {
        int32_t bitdepth = GENERATE(-1, 0, 1, 2, 4, 7, 9, 16, 32);
        HSTRANS handle = reinterpret_cast<HSTRANS>(12345);
        CHECK_FALSE(STransCreateMaskI(MemeImageBits, MemeImageWidth, MemeImageHeight, bitdepth, nullptr, 0, &handle));
        CHECK(handle == nullptr);
    }

    SECTION("creates a handle") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateMaskI(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, nullptr, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }

    SECTION("creates a 1x1 handle") {
        HSTRANS handle = nullptr;
        CHECK(STransCreateMaskI(MemeImageBits, 1, 1, 8, nullptr, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }

    SECTION("creates a handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 10, 2, 90, 7 };
        CHECK(STransCreateMaskI(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }

    SECTION("creates a 1x1 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 2, 2 };
        CHECK(STransCreateMaskI(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }

    SECTION("creates a 0x0 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 1, 1 };
        CHECK(STransCreateMaskI(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
        CHECK(STransDelete(handle) == 1);
    }
}

TEST_CASE("STransDelete", "[transparency]") {
    SECTION("called successfully") {
        HSTRANS handle = nullptr;
        STransCreateE(MemeImageBits, MemeImageWidth, MemeImageHeight, 8, nullptr, 0, &handle);
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
    // TODO
}

TEST_CASE("STransIntersectDirtyArray", "[transparency]") {
    // TODO
}

TEST_CASE("STransInvertMask", "[transparency]") {
    // TODO
}

TEST_CASE("STransIsPixelInMask", "[transparency]") {
    // TODO
}

TEST_CASE("STransSetDirtyArrayInfo", "[transparency]") {
    // TODO
}
