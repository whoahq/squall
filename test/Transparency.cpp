#include "test/Test.hpp"
#include "storm/Transparency.hpp"

#include <algorithm>
#include <climits>
#include <sstream>
#include <string>

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

static std::vector<uint8_t> MakeTestCopy(const uint8_t* data) {
    return std::vector<uint8_t>(data, data + ImageBytes);
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
        STransCreateI(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &trans);

        std::vector<uint8_t> result = t.ImageBits;
        CHECK(STransBlt(result.data(), 0, 0, ImageWidth, trans) == 1);
        CHECK(result == t.MaskBits);
    }

    SECTION("produces cutout with colorkey") {
        STransCreateI(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &trans);

        std::vector<uint8_t> result = t.ImageBits;
        CHECK(STransBlt(result.data(), 0, 0, ImageWidth, trans) == 1);
        CHECK(result == t.MaskResultBits);
    }

    SECTION("produces trans mask bytes only") {
        STransCreateI(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);

        std::vector<uint8_t> result(ImageBytes);
        CHECK(STransBlt(result.data(), 0, 0, ImageWidth, trans) == 1);

        std::vector<uint8_t> expected = t.MaskBits;
        std::replace(expected.begin(), expected.end(), static_cast<uint8_t>('.'), static_cast<uint8_t>(0));
        CHECK(result == expected);
    }

    SECTION("produces cutout from small trans at offset") {
        const uint8_t rawmask[] = {
            "....#####....."
            "... don't ...."
            "..## let ##..."
            ".### your ##.."
            ".## memes ###."
            "##### be #####"
            ".#### dreams #"
        };
        std::vector<uint8_t> mask(std::begin(rawmask), std::end(rawmask) - 1);

        STransCreateI(mask.data(), 14, 7, 8, nullptr, 0, &trans);

        std::vector<uint8_t> result = t.ImageBits;
        CHECK(STransBlt(result.data(), 44, 1, ImageWidth, trans) == 1);

        const uint8_t rawexpected[] = {
            "...................................................................................................."
            "......;&&&&&&&&&&;..............................#####..............................................."
            ".......xxxx+..:x:xXXXXXXX:..;XXx....;XXXXXx:... don't ....XXXXx:....:xXX;..:XXXXXXXX:XXXXXXX;;+:...."
            "........+&&&&:.:&&&xxXxxxx:;+X&&$:+xxxxxx&&&..## let ##...xxx&&&+..;+x&&&:+xxXxxxx;:;+xxx+&&&:......"
            ".........:$&&&x....;&&&+..x&&;x&&;.x&&$.:&&$.### your ##..$.:&&$:.;&&+;&&$.:&&&xxx...:&&&x.........."
            "...........+&&&&:..;&&&+.;&&&&$&&&:x&&$x&&X..## memes ###.$x&&X...$&&&&$&&x:&&&x:....:&&&x.........."
            ".......x$;..:$&&&+.:&&&::$&&...;&&$x&&$.x&&&##### be #####$.x&&&xx&&x...$&&+&&$:.....:&&&;.........."
            ".....x&&&&&&&&&&&X..;x:......................#### dreams #............................;x:..........."
            "...................................................................................................."
        };
        std::vector<uint8_t> expected(std::begin(rawexpected), std::end(rawexpected) - 1);
        CHECK(result == expected);
    }

    SECTION("produces cutout from trans rect to target") {
        RECT rct = { 45, 1, 96, 7 };
        STransCreateI(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('.'), &trans);

        std::vector<uint8_t> result = t.ImageBits;
        CHECK(STransBlt(result.data(), 0, 1, ImageWidth, trans) == 1);

        const uint8_t rawexpected[] = {
            "...................................................................................................."
            ".;&&&&&&&&&&+&&&&;............................;&&&&&&&&&&+.........................................."
            "+&&&;..xx::;XXXXXx:XXXX:xXX;;X:XXXXXXXX:XXXXXXX;;+:...::;XXXXXx:....:xXX;..:XXXXXXXX:XXXXXXX;;+:...."
            "$&&&:...:+xxXxxx&&&+xX;+x&&&:+xxXxxxx;:;+xxx+&&&::...:+xxXxxx&&&+..;+x&&&:+xxXxxxx;:;+xxx+&&&:......"
            "$&&&:....:x&&$x:&&$:&;&&+;&&$;:&&&xxx&$.:&&&x$&&&:.....x&&$.:&&$:.;&&+;&&$.:&&&xxx...:&&&x.........."
            "x&&&x.....x&&$x&&X.;&$&&&&$&&x:&&&x:&&$x:&&&xx&&&x.....x&&$x&&X...$&&&&$&&x:&&&x:....:&&&x.........."
            ".+&&&$:x$;x&&$&x&&&xx&&x:$&$&&+&&$:x&&$.:&&&;.+&&&$:...x&&$.x&&&xx&&x...$&&+&&$:.....:&&&;.........."
            "..:x&&&&x:&&&&&&&X..;x:..................;x:...:x&&&&x:...............................;x:..........."
            "...................................................................................................."
        };
        std::vector<uint8_t> expected(std::begin(rawexpected), std::end(rawexpected) - 1);
        CHECK(result == expected);
    }

    SECTION("does nothing for a 0x0 mask") {
        RECT rct = { 7, 1, 7, 1 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &trans);

        std::vector<uint8_t> result(ImageBytes, '.');
        CHECK(STransBlt(result.data(), 0, 0, ImageWidth, trans) == 1);
        CHECK(result == std::vector<uint8_t>(ImageBytes, '.'));
    }

    SECTION("does nothing for a 1x0 mask") {
        RECT rct = { 7, 1, 8, 1 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &trans);

        std::vector<uint8_t> result(ImageBytes, '.');
        CHECK(STransBlt(result.data(), 0, 0, ImageWidth, trans) == 1);
        CHECK(result == std::vector<uint8_t>(ImageBytes, '.'));
    }

    SECTION("does nothing for a 0x1 mask") {
        RECT rct = { 7, 1, 7, 2 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &trans);

        std::vector<uint8_t> result(ImageBytes, '.');
        CHECK(STransBlt(result.data(), 0, 0, ImageWidth, trans) == 1);
        CHECK(result == std::vector<uint8_t>(ImageBytes, '.'));
    }
}

TEST_CASE("STransBltUsingMask", "[transparency]") {
    TransTest t;
    HSTRANS trans;

    SECTION("applies inverted mask to output") {
        STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &trans);

        std::vector<uint8_t> dest(ImageBytes, '.');
        CHECK(STransBltUsingMask(dest.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans) == 1);
        CHECK(dest == t.InvertedMaskResultBits);
    }

    SECTION("applies mask to output") {
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);

        std::vector<uint8_t> dest(ImageBytes, '.');
        CHECK(STransBltUsingMask(dest.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans) == 1);
        CHECK(dest == t.MaskResultBits);
    }

    SECTION("reproduces image with no colorkey") {
        STransCreateI(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &trans);

        std::vector<uint8_t> dest(ImageBytes, '.');
        CHECK(STransBltUsingMask(dest.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans) == 1);
        CHECK(dest == t.ImageBits);
    }

    SECTION("reproduces image with no mask colorkey") {
        STransCreateMaskI(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &trans);

        std::vector<uint8_t> dest(ImageBytes, '.');
        CHECK(STransBltUsingMask(dest.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans) == 1);
        CHECK(dest == t.ImageBits);
    }

    SECTION("produces trans mask bytes only") {
        STransCreateI(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);

        std::vector<uint8_t> result(ImageBytes);
        CHECK(STransBltUsingMask(result.data(), t.MaskBits.data(), ImageWidth, ImageWidth, trans) == 1);

        std::vector<uint8_t> expected = t.MaskBits;
        std::replace(expected.begin(), expected.end(), static_cast<uint8_t>('.'), static_cast<uint8_t>(0));
        CHECK(result == expected);
    }

    SECTION("produces cutout from small trans") {
        uint8_t rawmask[] = {
            "....#####....."
            "... don't ...."
            "..## let ##..."
            ".### your ##.."
            ".## memes ###."
            "##### be #####"
            ".#### dreams #"
        };

        STransCreateMaskI(rawmask, 14, 7, 8, nullptr, STRANS_COLORKEY('.'), &trans);

        std::vector<uint8_t> result = t.ImageBits;
        CHECK(STransBltUsingMask(result.data(), rawmask, ImageWidth, 14, trans) == 1);

        const uint8_t rawexpected[] = {
            "....#####..........................................................................................."
            "... don't &&&&&&&;............................;&&&&&&&&&&+.........................................."
            "..## let ##+..:x:xXXXXXXX:..;XXx....;XXXXXx:.+&&&;....::;XXXXXx:....:xXX;..:XXXXXXXX:XXXXXXX;;+:...."
            ".### your ##&:.:&&&xxXxxxx:;+X&&$:+xxxxxx&&&+$&&&:...:+xxXxxx&&&+..;+x&&&:+xxXxxxx;:;+xxx+&&&:......"
            ".## memes ###&x....;&&&+..x&&;x&&;.x&&$.:&&$:$&&&:.....x&&$.:&&$:.;&&+;&&$.:&&&xxx...:&&&x.........."
            "##### be #####&&:..;&&&+.;&&&&$&&&:x&&$x&&X..x&&&x.....x&&$x&&X...$&&&&$&&x:&&&x:....:&&&x.........."
            ".#### dreams #&&&+.:&&&::$&&...;&&$x&&$.x&&&x.+&&&$:...x&&$.x&&&xx&&x...$&&+&&$:.....:&&&;.........."
            ".....x&&&&&&&&&&&X..;x:........................:x&&&&x:...............................;x:..........."
            "...................................................................................................."
        };
        std::vector<uint8_t> expected(std::begin(rawexpected), std::end(rawexpected) - 1);
        CHECK(result == expected);
    }

    SECTION("produces cutout from trans rect to target") {
        RECT rct = { 45, 1, 96, 7 };
        STransCreateMaskI(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('.'), &trans);

        std::vector<uint8_t> result(ImageBytes, '.');
        CHECK(STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans) == 1);

        const uint8_t rawexpected[] = {
            "...................................................................................................."
            ".........&&&&&&&&;............................;&&&&................................................."
            "........xxx+..:x:xXX..XXX:..;XXx....;XXXXXx:.+&&&..................................................."
            "..........&&&:.:&&&x.Xxxxx:;+.&&$:+xx...x&&&+......................................................."
            "..........$&&&x......&&+..x&&;x&&;.x....:&&$:......................................................."
            "...........+&&.&:..;&&&+...&&&$&&&:.....&&X........................................................."
            ".......x$;...............................&&&........................................................"
            "...................................................................................................."
            "...................................................................................................."
        };
        std::vector<uint8_t> expected(std::begin(rawexpected), std::end(rawexpected) - 1);
        CHECK(result == expected);
    }

    SECTION("does nothing for a 0x0 mask") {
        RECT rct = { 7, 1, 7, 1 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &trans);

        std::vector<uint8_t> result(ImageBytes, '.');
        CHECK(STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans) == 1);
        CHECK(result == std::vector<uint8_t>(ImageBytes, '.'));
    }

    SECTION("does nothing for a 1x0 mask") {
        RECT rct = { 7, 1, 8, 1 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &trans);

        std::vector<uint8_t> result(ImageBytes, '.');
        CHECK(STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans) == 1);
        CHECK(result == std::vector<uint8_t>(ImageBytes, '.'));
    }

    SECTION("does nothing for a 0x1 mask") {
        RECT rct = { 7, 1, 7, 2 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &trans);

        std::vector<uint8_t> result(ImageBytes, '.');
        CHECK(STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans) == 1);
        CHECK(result == std::vector<uint8_t>(ImageBytes, '.'));
    }
}

TEST_CASE("STransCombineMasks", "[transparency]") {
    TransTest t;
    HSTRANS trans, trans2;
    HSTRANS combined = nullptr;

    SECTION("combines masks") {
        STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);
        STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &trans2);

        const std::vector<uint8_t> empty(ImageBytes, '.');

        std::vector<uint8_t> result = empty;
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans);
        CHECK(result == t.MaskResultBits);

        result = empty;
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans2);
        CHECK(result == t.InvertedMaskResultBits);

        CHECK(STransCombineMasks(trans, trans2, 0, 0, 0, &combined) == 1);

        result = empty;
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, combined);
        CHECK(result == t.ImageBits);
    }

    SECTION("combines masks inverted") {
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &trans2);

        const std::vector<uint8_t> empty(ImageBytes, '.');

        std::vector<uint8_t> result = empty;
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans);
        CHECK(result == t.MaskResultBits);

        result = empty;
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans2);
        CHECK(result == t.InvertedMaskResultBits);

        CHECK(STransCombineMasks(trans, trans2, 0, 0, 0, &combined) == 1);

        result = empty;
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, combined);
        CHECK(result == t.ImageBits);
    }

    SECTION("combines with self") {
        uint32_t flag = GENERATE(0, STRANS_CF_INTERSECT);
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);

        CHECK(STransCombineMasks(trans, trans, 0, 0, flag, &combined) == 1);

        std::vector<uint8_t> result(ImageBytes, '.');
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, combined);
        CHECK(result == t.MaskResultBits);
    }

    SECTION("combines with inverted self") {
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);

        CHECK(STransCombineMasks(trans, trans, 0, 0, STRANS_CF_INVERTSECOND, &combined) == 1);

        std::vector<uint8_t> result(ImageBytes, '.');
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, combined);
        CHECK(result == t.ImageBits);
    }

    SECTION("subtracts from self") {
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);

        CHECK(STransCombineMasks(trans, trans, 0, 0, STRANS_CF_SUBTRACT, &combined) == 1);

        std::vector<uint8_t> result(ImageBytes, '.');
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, combined);
        CHECK(result == std::vector<uint8_t>(ImageBytes, '.'));
    }

    SECTION("combines with empty trans") {
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);
        STransCreateMaskE(t.MaskBits.data(), 1, 1, 8, nullptr, 0, &trans2);

        CHECK(STransCombineMasks(trans, trans2, 0, 0, 0, &combined) == 1);

        std::vector<uint8_t> result(ImageBytes, '.');
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, combined);
        CHECK(result == t.MaskResultBits);
    }

    SECTION("combines with gap and offset") {
        uint8_t rawSMask[] = {
            "##################.................................................................................."
            "##################.................................................................................."
            "##############......................................................................................"
            "##############......................................................................................"
            "##################.................................................................................."
            "##################.................................................................................."
            "##################.................................................................................."
            "##################.................................................................................."
            "##################.................................................................................."
        };
        uint8_t rawCMask[] = {
            "##############"
            "#########....."
            "#########....."
            "#########....."
            ".########....."
            ".########....."
            ".##########..."
        };
        STransCreateMaskE(rawSMask, ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);
        STransCreateMaskE(rawCMask, 14, 7, 8, nullptr, STRANS_COLORKEY('.'), &trans2);

        CHECK(STransCombineMasks(trans, trans2, 44, 1, 0, &combined) == 1);

        std::vector<uint8_t> result(ImageBytes, '.');
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, combined);
        CHECK(result == t.MaskResultBits);
    }

    SECTION("excludes gap combination out of bounds") {
        uint8_t rawSMask[] = {
            "##################"
            "##################"
            "##############...."
            "##############...."
            "##################"
            "##################"
            "##################"
            "##################"
            "##################"
        };
        uint8_t rawCMask[] = {
            "##############"
            "#########....."
            "#########....."
            "#########....."
            ".########....."
            ".########....."
            ".##########..."
        };
        STransCreateMaskE(rawSMask, 18, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);
        STransCreateMaskE(rawCMask, 14, 7, 8, nullptr, STRANS_COLORKEY('.'), &trans2);

        CHECK(STransCombineMasks(trans, trans2, 44, 1, 0, &combined) == 1);

        std::vector<uint8_t> result(ImageBytes, '.');
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, combined);

        const uint8_t expectedraw[] = {
            "...................................................................................................."
            "......;&&&&&&&&&&;.................................................................................."
            ".......xxxx+........................................................................................"
            "........+&&&&:......................................................................................"
            ".........:$&&&x....................................................................................."
            "...........+&&&&:..................................................................................."
            ".......x$;..:$&&&+.................................................................................."
            ".....x&&&&&&&&&&&X.................................................................................."
            "...................................................................................................."
        };
        std::vector<uint8_t> expected(std::begin(expectedraw), std::end(expectedraw) - 1);
        CHECK(result == expected);
    }

    SECTION("combines with self using invert flag") {
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);

        const std::vector<uint8_t> empty(ImageBytes, '.');

        std::vector<uint8_t> result = empty;
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans);
        CHECK(result == t.MaskResultBits);

        CHECK(STransCombineMasks(trans, trans, 0, 0, STRANS_CF_INVERTSECOND, &combined) == 1);

        result = empty;
        STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, combined);
        CHECK(result == t.ImageBits);
    }

    SECTION("combines using intersect flag") {
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);
        STransCreateMaskE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans2);

        CHECK(STransCombineMasks(trans, trans2, 0, 0, STRANS_CF_INTERSECT, &combined) == 1);

        std::vector<uint8_t> result(ImageBytes, '.');
        STransBltUsingMask(result.data(), t.MaskBits.data(), ImageWidth, ImageWidth, combined);

        const uint8_t expectedraw[] = {
            "...................................................................................................."
            "......############............................############.........................................."
            ".......#####.................................#####.................................................."
            "........######..............................######.................................................."
            ".........######.............................######.................................................."
            "...........######............................#####.................................................."
            ".......###..######............................######................................................"
            ".....#############.............................########............................................."
            "...................................................................................................."
        };
        std::vector<uint8_t> expected(std::begin(expectedraw), std::end(expectedraw) - 1);
        CHECK(result == expected);
    }

    SECTION("combines with subtract") {
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);
        STransCreateMaskE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans2);

        CHECK(STransCombineMasks(trans, trans2, 0, 0, STRANS_CF_SUBTRACT, &combined) == 1);

        std::vector<uint8_t> result(ImageBytes, '.');
        STransBltUsingMask(result.data(), t.MaskBits.data(), ImageWidth, ImageWidth, combined);

        const uint8_t expectedraw[] = {
            "##################..........................##############.........................................."
            "######......................................##......................................................"
            "#######.....##..............................#.....###..............................................."
            "########..........................................###..............................................."
            "#########......###................................###..............................................."
            "###########......#................................###..............................................."
            "#######...##.................................#......#..............................................."
            "#####........................................##....................................................."
            "##################...........................##########............................................."
        };
        std::vector<uint8_t> expected(std::begin(expectedraw), std::end(expectedraw) - 1);
        CHECK(result == expected);
    }

    SECTION("combines with 0x0 mask") {
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);

        RECT rct = { 7, 1, 7, 1 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &trans2);

        CHECK(STransCombineMasks(trans, trans2, 0, 0, STRANS_CF_INTERSECT, &combined) == 1);

        std::vector<uint8_t> result(ImageBytes, '.');
        STransBltUsingMask(result.data(), t.MaskBits.data(), ImageWidth, ImageWidth, combined);
        CHECK(result == std::vector<uint8_t>(ImageBytes, '.'));
    }

    SECTION("combines with 1x0 mask") {
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);

        RECT rct = { 7, 1, 8, 1 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &trans2);

        CHECK(STransCombineMasks(trans, trans2, 0, 0, STRANS_CF_INTERSECT, &combined) == 1);

        std::vector<uint8_t> result(ImageBytes, '.');
        STransBltUsingMask(result.data(), t.MaskBits.data(), ImageWidth, ImageWidth, combined);
        CHECK(result == std::vector<uint8_t>(ImageBytes, '.'));
    }

    SECTION("combines with 0x1 mask") {
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);

        RECT rct = { 7, 1, 7, 2 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &trans2);

        CHECK(STransCombineMasks(trans, trans2, 0, 0, STRANS_CF_INTERSECT, &combined) == 1);

        std::vector<uint8_t> result(ImageBytes, '.');
        STransBltUsingMask(result.data(), t.MaskBits.data(), ImageWidth, ImageWidth, combined);
        CHECK(result == std::vector<uint8_t>(ImageBytes, '.'));
    }

    // TODO test after STransDelete for s_savedata (investigate)
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
        REQUIRE(handle != nullptr);

        std::vector<uint8_t> result(4);
        STransBlt(result.data(), 0, 0, 2, handle);

        std::vector<uint8_t> expected = { '.', 0, 0, 0 };
        CHECK(result == expected);
    }

    SECTION("creates a handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 10, 2, 90, 7 };
        CHECK(STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        CHECK(handle != nullptr);
    }

    SECTION("creates a 1x1 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 7, 1, 8, 2 };
        CHECK(STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        REQUIRE(handle != nullptr);

        std::vector<uint8_t> result(4);
        STransBlt(result.data(), 0, 0, 2, handle);

        std::vector<uint8_t> expected = { '&', 0, 0, 0 };
        CHECK(result == expected);
    }

    SECTION("creates a 0x0 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 1, 1 };
        CHECK(STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        REQUIRE(handle != nullptr);

        std::vector<uint8_t> result(4);
        STransBlt(result.data(), 0, 0, 2, handle);

        std::vector<uint8_t> expected = { 0, 0, 0, 0 };
        CHECK(result == expected);
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
        RECT rct = { 7, 1, 7, 1 };
        CHECK(STransCreateI(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        REQUIRE(handle != nullptr);

        std::vector<uint8_t> result(4);
        STransBlt(result.data(), 0, 0, 2, handle);

        std::vector<uint8_t> expected = { '&', 0, 0, 0 };
        CHECK(result == expected);
    }

    SECTION("creates a 0x0 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 0, 0 };
        CHECK(STransCreateI(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &handle) == 1);
        REQUIRE(handle != nullptr);

        std::vector<uint8_t> result(4);
        STransBlt(result.data(), 0, 0, 2, handle);

        std::vector<uint8_t> expected = { 0, 0, 0, 0 };
        CHECK(result == expected);
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
        RECT rct = { 7, 1, 8, 2 };
        CHECK(STransCreateMaskE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('.'), &handle) == 1);
        REQUIRE(handle != nullptr);

        std::vector<uint8_t> result(4);
        STransBlt(result.data(), 0, 0, 2, handle);

        std::vector<uint8_t> expected = { 1, 0, 0, 0 };
        CHECK(result == expected);
    }

    SECTION("creates a 0x0 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 1, 1 };
        CHECK(STransCreateMaskE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('.'), &handle) == 1);
        REQUIRE(handle != nullptr);

        std::vector<uint8_t> result(4);
        STransBlt(result.data(), 0, 0, 2, handle);

        std::vector<uint8_t> expected = { 0, 0, 0, 0 };
        CHECK(result == expected);
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
        RECT rct = { 7, 1, 7, 1 };
        CHECK(STransCreateMaskI(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('.'), &handle) == 1);
        REQUIRE(handle != nullptr);

        std::vector<uint8_t> result(4);
        STransBlt(result.data(), 0, 0, 2, handle);

        std::vector<uint8_t> expected = { 1, 0, 0, 0 };
        CHECK(result == expected);
    }

    SECTION("creates a 0x0 handle using a rect") {
        HSTRANS handle = nullptr;
        RECT rct = { 1, 1, 0, 0 };
        CHECK(STransCreateMaskI(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('.'), &handle) == 1);
        REQUIRE(handle != nullptr);

        std::vector<uint8_t> result(4);
        STransBlt(result.data(), 0, 0, 2, handle);

        std::vector<uint8_t> expected = { 0, 0, 0, 0 };
        CHECK(result == expected);
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
        STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &trans);

        CHECK(STransDuplicate(trans, &copy) == 1);
        STransDelete(trans);

        std::vector<uint8_t> result = t.ImageBits;
        CHECK(STransBlt(result.data(), 0, 0, ImageWidth, copy) == 1);
        CHECK(result == t.MaskResultBits);
    }

    SECTION("duplicates a 0x0 mask") {
        RECT rct = { 7, 1, 7, 1 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('.'), &trans);

        CHECK(STransDuplicate(trans, &copy) == 1);
        STransDelete(trans);

        std::vector<uint8_t> result(ImageBytes, '.');
        CHECK(STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, copy) == 1);
        CHECK(result == std::vector<uint8_t>(ImageBytes, '.'));
    }

    SECTION("duplicates a 1x0 mask") {
        RECT rct = { 7, 1, 8, 1 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('.'), &trans);

        CHECK(STransDuplicate(trans, &copy) == 1);
        STransDelete(trans);

        std::vector<uint8_t> result(ImageBytes, '.');
        CHECK(STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, copy) == 1);
        CHECK(result == std::vector<uint8_t>(ImageBytes, '.'));
    }

    SECTION("duplicates a 0x1 mask") {
        RECT rct = { 7, 1, 7, 2 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('.'), &trans);

        CHECK(STransDuplicate(trans, &copy) == 1);
        STransDelete(trans);

        std::vector<uint8_t> result(ImageBytes, '.');
        CHECK(STransBltUsingMask(result.data(), t.ImageBits.data(), ImageWidth, ImageWidth, copy) == 1);
        CHECK(result == std::vector<uint8_t>(ImageBytes, '.'));
    }
}

TEST_CASE("STransIntersectDirtyArray", "[transparency]") {
    TransTest t;
    HSTRANS mask;
    HSTRANS trans;

    SECTION("does nothing if dirty info hasn't been set") {
        STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &mask);

        trans = reinterpret_cast<HSTRANS>(1234);
        CHECK_FALSE(STransIntersectDirtyArray(mask, t.ImageBits.data(), 1, &trans));
        CHECK(trans == nullptr);
    }

    SECTION("intersects dirty array with source mask") {
        STransSetDirtyArrayInfo(ImageWidth, ImageHeight, 4, 1);

        uint8_t dirty[] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
            0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
            0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
            0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        };

        STransCreateMaskI(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &mask);
        CHECK(STransIntersectDirtyArray(mask, dirty, 1, &trans) == 1);

        std::vector<uint8_t> dest(ImageBytes, '.');
        CHECK(STransBltUsingMask(dest.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans));

        // This is a strict subset of the S C array (MaskResultBits)
        const uint8_t rawexpected[] = {
            "...................................................................................................."
            "......;&&&&&&&&&&;............................;&&&&&&&&&............................................"
            "........xxx+.................................+&&&;.................................................."
            "........+&&&................................+$&&&:.................................................."
            ".........:$&&&x.............................:$&&&:.................................................."
            "............&&&&.............................x&&&x.................................................."
            "............:$&&................................&&$:................................................"
            ".....x&&&&&&&&&&................................x&&&&x:............................................."
            "...................................................................................................."
        };
        std::vector<uint8_t> expected(std::begin(rawexpected), std::end(rawexpected) - 1);
        CHECK(dest == expected);
    }

    SECTION("applies mask with dirty array scale") {
        STransSetDirtyArrayInfo(ImageWidth, ImageHeight, 8, 2);

        uint8_t dirty[] = {
            0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
            0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0,
            0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0,
            0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        };

        STransCreateI(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &mask);
        CHECK(STransIntersectDirtyArray(mask, dirty, 1, &trans) == 1);

        std::vector<uint8_t> dest(ImageBytes, '.');
        CHECK(STransBltUsingMask(dest.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans));

        const uint8_t rawexpected[] = {
            "...................................................................................................."
            "...................................................................................................."
            "................:xXXXXXXX:..;XXx....;XXXXXx:..........::............:xXX;..:XXXXXXXX:XXX............"
            "................&&&xxXxxxx:;+X&&$:+xxxxxx&&&.........:+x........+..;+x&&&:+xxXxxxx;:;+xx............"
            "..........................x&&;x&.......................x&&$.:&&$........&$.:&&&xxx...:&&&x.........."
            ".........................;&&&&$&.......................x&&$x&&X.........&&x:&&&x:....:&&&x.........."
            "...................:&&&::$&&...;&&$x&&$................x........xx&&x...$&&+&&$:...................."
            "....................;x:............................................................................."
            "...................................................................................................."
        };
        std::vector<uint8_t> expected(std::begin(rawexpected), std::end(rawexpected) - 1);
        CHECK(dest == expected);
    }

    SECTION("only applies mask which passes dirtyarraymask test") {
        STransSetDirtyArrayInfo(ImageWidth, ImageHeight, 8, 2);

        uint8_t dirty[] = {
            0, 0, 0, 2, 2, 2, 1, 0, 0, 1, 1, 0, 0,
            0, 0, 1, 3, 3, 3, 2, 0, 1, 1, 1, 0, 0,
            0, 0, 2, 3, 7, 7, 1, 1, 0, 255, 1, 1, 0,
            0, 0, 1, 6, 7, 7, 2, 0, 1, 1, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        };

        STransCreateI(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &mask);
        CHECK(STransIntersectDirtyArray(mask, dirty, 2, &trans) == 1);

        std::vector<uint8_t> dest(ImageBytes, '.');
        CHECK(STransBltUsingMask(dest.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans));

        const uint8_t rawexpected[] = {
            "...................................................................................................."
            "...................................................................................................."
            "........................X:..;XXx....;XXXXXx:..........::............................................"
            "........................xx:;+X&&$:+xxxxxx&&&.........:+x............................................"
            "...................;&&&+..x&&;x&&;.x&&$.:&&$............................&$.:&&&x...................."
            "...................;&&&+.;&&&&$&&&:x&&$x&&X.............................&&x:&&&x...................."
            "........................:$&&...;&&$x&&$.x&&&x..........x............................................"
            "...................................................................................................."
            "...................................................................................................."
        };
        std::vector<uint8_t> expected(std::begin(rawexpected), std::end(rawexpected) - 1);
        CHECK(dest == expected);
    }

    SECTION("applies whole mask if 1x1 dirty array is set") {
        STransSetDirtyArrayInfo(ImageWidth, ImageHeight, 128, 16);

        uint8_t dirty[] = { 1 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &mask);

        CHECK(STransIntersectDirtyArray(mask, dirty, 1, &trans) == 1);

        std::vector<uint8_t> dest(ImageBytes, '.');
        CHECK(STransBltUsingMask(dest.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans));
        CHECK(dest == t.InvertedMaskResultBits);
    }

    SECTION("applies nothing if 1x1 dirty array is unset") {
        STransSetDirtyArrayInfo(ImageWidth, ImageHeight, 128, 16);

        uint8_t dirty[] = { 0 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('#'), &mask);

        CHECK(STransIntersectDirtyArray(mask, dirty, 1, &trans) == 1);

        std::vector<uint8_t> dest(ImageBytes, '.');
        CHECK(STransBltUsingMask(dest.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans));
        CHECK(dest == std::vector<uint8_t>(ImageBytes, '.'));
    }

    SECTION("does nothing on a 0x0 mask") {
        STransSetDirtyArrayInfo(ImageWidth, ImageHeight, 16, 4);

        std::vector<uint8_t> dirty((ImageWidth / 16 + 1) * (ImageHeight / 4 + 1), 1);

        RECT rct = { 7, 1, 7, 1 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('#'), &mask);
        CHECK(STransIntersectDirtyArray(mask, dirty.data(), 1, &trans) == 1);

        std::vector<uint8_t> dest(ImageBytes, '.');
        CHECK(STransBltUsingMask(dest.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans));
        CHECK(dest == std::vector<uint8_t>(ImageBytes, '.'));
    }

    SECTION("does nothing on a 0x1 mask") {
        STransSetDirtyArrayInfo(ImageWidth, ImageHeight, 16, 4);

        std::vector<uint8_t> dirty((ImageWidth / 16 + 1) * (ImageHeight / 4 + 1), 1);

        RECT rct = { 7, 1, 7, 2 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('#'), &mask);
        CHECK(STransIntersectDirtyArray(mask, dirty.data(), 1, &trans) == 1);

        std::vector<uint8_t> dest(ImageBytes, '.');
        CHECK(STransBltUsingMask(dest.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans));
        CHECK(dest == std::vector<uint8_t>(ImageBytes, '.'));
    }

    SECTION("does nothing on a 1x0 mask") {
        STransSetDirtyArrayInfo(ImageWidth, ImageHeight, 16, 4);

        std::vector<uint8_t> dirty((ImageWidth / 16 + 1) * (ImageHeight / 4 + 1), 1);

        RECT rct = { 7, 1, 8, 1 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('#'), &mask);
        CHECK(STransIntersectDirtyArray(mask, dirty.data(), 1, &trans) == 1);

        std::vector<uint8_t> dest(ImageBytes, '.');
        CHECK(STransBltUsingMask(dest.data(), t.ImageBits.data(), ImageWidth, ImageWidth, trans));
        CHECK(dest == std::vector<uint8_t>(ImageBytes, '.'));
    }

    // TODO test after STransDelete for s_savedata (investigate)
}

TEST_CASE("STransInvertMask", "[transparency]") {
    TransTest t;
    HSTRANS trans;
    HSTRANS inverted = nullptr;

    SECTION("creates a new transparency record") {
        STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &trans);

        REQUIRE(STransInvertMask(trans, &inverted) == 1);
        STransDelete(trans);

        REQUIRE(inverted != nullptr);

        std::vector<uint8_t> dest(ImageBytes, '.');
        STransBltUsingMask(dest.data(), t.ImageBits.data(), ImageWidth, ImageWidth, inverted);
        CHECK(dest == std::vector<uint8_t>(ImageBytes, '.'));
    }

    SECTION("inverts a mask") {
        STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, STRANS_COLORKEY('.'), &trans);

        std::vector<uint8_t> dest(ImageBytes, '.');
        std::vector<uint8_t> source = t.ImageBits;
        CHECK(STransBltUsingMask(dest.data(), source.data(), ImageWidth, ImageWidth, trans));
        CHECK(dest == t.MaskResultBits);

        REQUIRE(STransInvertMask(trans, &inverted) == 1);

        std::vector<uint8_t> dest2(ImageBytes, '.');
        CHECK(STransBltUsingMask(dest2.data(), source.data(), ImageWidth, ImageWidth, inverted));
        CHECK(dest2 == t.InvertedMaskResultBits);
    }

    SECTION("does nothing for a 0x0 mask") {
        RECT rct = { 7, 1, 7, 1 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('.'), &trans);

        REQUIRE(STransInvertMask(trans, &inverted) == 1);

        std::vector<uint8_t> dest(ImageBytes, '.');
        STransBltUsingMask(dest.data(), t.ImageBits.data(), ImageWidth, ImageWidth, inverted);
        CHECK(dest == std::vector<uint8_t>(ImageBytes, '.'));
    }

    SECTION("does nothing for a 1x0 mask") {
        RECT rct = { 7, 1, 7, 2 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('.'), &trans);

        REQUIRE(STransInvertMask(trans, &inverted) == 1);

        std::vector<uint8_t> dest(ImageBytes, '.');
        STransBltUsingMask(dest.data(), t.ImageBits.data(), ImageWidth, ImageWidth, inverted);
        CHECK(dest == std::vector<uint8_t>(ImageBytes, '.'));
    }

    SECTION("does nothing for a 0x1 mask") {
        RECT rct = { 7, 1, 8, 1 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, STRANS_COLORKEY('.'), &trans);

        REQUIRE(STransInvertMask(trans, &inverted) == 1);

        std::vector<uint8_t> dest(ImageBytes, '.');
        STransBltUsingMask(dest.data(), t.ImageBits.data(), ImageWidth, ImageWidth, inverted);
        CHECK(dest == std::vector<uint8_t>(ImageBytes, '.'));
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
        CHECK(STransIsPixelInMask(trans, pos.first, pos.second) == 1);
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

        CHECK(STransIsPixelInMask(trans, pos.first, pos.second) == 1);
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

        CHECK(STransIsPixelInMask(trans, pos.first, pos.second) == 1);
    }

    SECTION("false for a 0x0 mask") {
        RECT rct = { 7, 1, 7, 1 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &trans);
        CHECK_FALSE(STransIsPixelInMask(trans, 0, 0));
        CHECK_FALSE(STransIsPixelInMask(trans, 7, 1));
    }

    SECTION("false for a 1x0 mask") {
        RECT rct = { 7, 1, 8, 1 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &trans);
        CHECK_FALSE(STransIsPixelInMask(trans, 0, 0));
        CHECK_FALSE(STransIsPixelInMask(trans, 7, 1));
    }

    SECTION("false for a 0x1 mask") {
        RECT rct = { 7, 1, 7, 2 };
        STransCreateMaskE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &trans);
        CHECK_FALSE(STransIsPixelInMask(trans, 0, 0));
        CHECK_FALSE(STransIsPixelInMask(trans, 7, 1));
    }
}

TEST_CASE("STransSetDirtyArrayInfo", "[transparency]") {
    TransTest t;
    HSTRANS mask, trans;

    SECTION("clears dirty offset and fails if cx is <= 0") {
        int32_t cx = GENERATE(0, -1, INT_MIN);
        CHECK_FALSE(STransSetDirtyArrayInfo(0, 0, cx, 42));

        STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &mask);
        CHECK_FALSE(STransIntersectDirtyArray(mask, t.ImageBits.data(), 1, &trans));
    }

    SECTION("clears dirty offset and fails if cy is <= 0") {
        int32_t cy = GENERATE(0, -1, INT_MIN);
        CHECK_FALSE(STransSetDirtyArrayInfo(0, 0, 42, cy));

        STransCreateE(t.MaskBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &mask);
        CHECK_FALSE(STransIntersectDirtyArray(mask, t.ImageBits.data(), 1, &trans));
    }

    SECTION("fails if cx is not power of 2") {
        int32_t cx = GENERATE(3, 5, 6, 9, 255);
        CHECK_FALSE(STransSetDirtyArrayInfo(8, 8, cx, 1));
    }

    SECTION("fails if cy is not power of 2") {
        int32_t cy = GENERATE(3, 5, 6, 9, 255);
        CHECK_FALSE(STransSetDirtyArrayInfo(8, 8, 1, cy));
    }

    SECTION("allocates dirty info") {
        CHECK(STransSetDirtyArrayInfo(8, 8, 1, 1) == 1);

        std::vector<uint8_t> dirty(8 * 8, 0);
        STransCreateE(t.MaskBits.data(), 4, 4, 8, nullptr, 0, &mask);
        CHECK(STransUpdateDirtyArray(dirty.data(), 1, 0, 0, mask) == 1);

        std::vector<uint8_t> expected = {
            1, 1, 1, 1, 0, 0, 0, 0,
            1, 1, 1, 1, 0, 0, 0, 0,
            1, 1, 1, 1, 0, 0, 0, 0,
            1, 1, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
        };
        CHECK(dirty == expected);
    }

    SECTION("sets x axis scale") {
        CHECK(STransSetDirtyArrayInfo(8, 8, 4, 1) == 1);

        std::vector<uint8_t> dirty(2 * 8, 0);
        STransCreateE(t.MaskBits.data(), 4, 4, 8, nullptr, 0, &mask);
        CHECK(STransUpdateDirtyArray(dirty.data(), 1, 0, 0, mask) == 1);

        std::vector<uint8_t> expected = {
            1, 0,
            1, 0,
            1, 0,
            1, 0,
            0, 0,
            0, 0,
            0, 0,
            0, 0,
        };
        CHECK(dirty == expected);
    }

    SECTION("sets y axis scale") {
        CHECK(STransSetDirtyArrayInfo(8, 8, 1, 4) == 1);

        std::vector<uint8_t> dirty(8 * 2, 0);
        STransCreateE(t.MaskBits.data(), 4, 4, 8, nullptr, 0, &mask);
        CHECK(STransUpdateDirtyArray(dirty.data(), 1, 0, 0, mask) == 1);

        std::vector<uint8_t> expected = {
            1, 1, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
        };
        CHECK(dirty == expected);
    }
}

TEST_CASE("STransUpdateDirtyArray", "[transparency]") {
    TransTest t;
    HSTRANS trans, transtest;

    SECTION("fails if dirty offset is not set") {
        STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &trans);
        CHECK_FALSE(STransUpdateDirtyArray(t.MaskBits.data(), 1, 0, 0, trans));
    }

    SECTION("fails if trans width <= 0") {
        STransSetDirtyArrayInfo(ImageWidth, ImageHeight, 1, 1);

        std::vector<uint8_t> dirty(ImageBytes, 0);
        // confirm that dirty info is initialized
        STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &transtest);
        CHECK(STransUpdateDirtyArray(dirty.data(), 1, 0, 0, transtest) == 1);

        RECT rct = { 0, 0, 0, 5 };
        STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &trans);
        CHECK_FALSE(STransUpdateDirtyArray(dirty.data(), 1, 0, 0, trans));
    }

    SECTION("fails if trans height <= 0") {
        STransSetDirtyArrayInfo(ImageWidth, ImageHeight, 1, 1);

        std::vector<uint8_t> dirty(ImageBytes, 0);
        // confirm that dirty info is initialized
        STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &transtest);
        CHECK(STransUpdateDirtyArray(dirty.data(), 1, 0, 0, transtest) == 1);

        RECT rct = { 0, 0, 5, 0 };
        STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, &rct, 0, &trans);
        CHECK_FALSE(STransUpdateDirtyArray(dirty.data(), 1, 0, 0, trans));
    }

    SECTION("fails if tracecontour is true") {
        STransSetDirtyArrayInfo(ImageWidth, ImageHeight, 1, 1);

        std::vector<uint8_t> dirty(ImageBytes, 0);
        // confirm that dirty info is initialized
        STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &transtest);
        CHECK(STransUpdateDirtyArray(dirty.data(), 1, 0, 0, transtest) == 1);

        int32_t tracecontour = GENERATE(-1, 1, INT_MIN, INT_MAX);
        STransCreateE(t.ImageBits.data(), ImageWidth, ImageHeight, 8, nullptr, 0, &trans);
        CHECK_FALSE(STransUpdateDirtyArray(dirty.data(), 1, 0, 0, trans, tracecontour));
    }

    SECTION("updates only trans area with x scale") {
        STransSetDirtyArrayInfo(8, 8, 4, 1);

        std::vector<uint8_t> dirty(2 * 8, 0);
        STransCreateE(t.MaskBits.data(), 4, 4, 8, nullptr, 0, &trans);
        CHECK(STransUpdateDirtyArray(dirty.data(), 1, 0, 0, trans) == 1);

        std::vector<uint8_t> expected = {
            1, 0,
            1, 0,
            1, 0,
            1, 0,
            0, 0,
            0, 0,
            0, 0,
            0, 0,
        };
        CHECK(dirty == expected);
    }

    SECTION("updates only trans area with y scale") {
        STransSetDirtyArrayInfo(8, 8, 1, 4);

        std::vector<uint8_t> dirty(8 * 2, 0);
        STransCreateE(t.MaskBits.data(), 4, 4, 8, nullptr, 0, &trans);
        CHECK(STransUpdateDirtyArray(dirty.data(), 1, 0, 0, trans) == 1);

        std::vector<uint8_t> expected = {
            1, 1, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
        };
        CHECK(dirty == expected);
    }

    SECTION("updates area specified by trans rect and position") {
        STransSetDirtyArrayInfo(8, 8, 1, 1);

        std::vector<uint8_t> dirty(8 * 8, 0);
        RECT rct = { 3, 5, 7, 7 };
        STransCreateI(t.MaskBits.data(), 8, 8, 8, &rct, 0, &trans);
        CHECK(STransUpdateDirtyArray(dirty.data(), 1, 3, 2, trans) == 1);

        std::vector<uint8_t> expected = {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 1,
            0, 0, 0, 1, 1, 1, 1, 1,
            0, 0, 0, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
        };
        CHECK(dirty == expected);
    }

    SECTION("applies OR operation on dirty array") {
        STransSetDirtyArrayInfo(3, 3, 1, 1);

        std::vector<uint8_t> dirty(3 * 3, 0);
        STransCreateI(t.MaskBits.data(), 3, 3, 8, nullptr, 0, &trans);
        STransCreateI(t.MaskBits.data(), 1, 2, 8, nullptr, 0, &transtest);

        CHECK(STransUpdateDirtyArray(dirty.data(), 1, 0, 0, trans) == 1);

        std::vector<uint8_t> expected = {
            1, 1, 1,
            1, 1, 1,
            1, 1, 1,
        };
        CHECK(dirty == expected);

        CHECK(STransUpdateDirtyArray(dirty.data(), 6, 1, 0, transtest) == 1);
        expected = {
            1, 7, 1,
            1, 7, 1,
            1, 1, 1,
        };
        CHECK(dirty == expected);

        CHECK(STransUpdateDirtyArray(dirty.data(), 8, 0, 0, trans) == 1);
        expected = {
            9, 15, 9,
            9, 15, 9,
            9, 9, 9,
        };
        CHECK(dirty == expected);

        CHECK(STransUpdateDirtyArray(dirty.data(), 0xFF, 0, 0, trans) == 1);
        expected = {
            255, 255, 255,
            255, 255, 255,
            255, 255, 255,
        };
        CHECK(dirty == expected);
    }

    SECTION("chooses appropriate positions with scaling") {
        STransSetDirtyArrayInfo(7 * 16, 5 * 4, 16, 4);

        std::vector<uint8_t> dirty(7 * 5, 0);
        STransCreateI(t.MaskBits.data(), 40, 6, 8, nullptr, 0, &trans);

        CHECK(STransUpdateDirtyArray(dirty.data(), 1, 56, 3, trans) == 1);

        std::vector<uint8_t> expected = {
            0, 0, 0, 1, 1, 1, 0,
            0, 0, 0, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
        };
        CHECK(dirty == expected);

        CHECK(STransUpdateDirtyArray(dirty.data(), 2, 40, 2, trans) == 1);

        expected = {
            0, 0, 2, 3, 3, 1, 0,
            0, 0, 2, 3, 3, 1, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
        };
        CHECK(dirty == expected);

        CHECK(STransUpdateDirtyArray(dirty.data(), 4, 50, 1, trans) == 1);

        expected = {
            0, 0, 2, 7, 7, 1, 0,
            0, 0, 2, 3, 3, 1, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
        };
        CHECK(dirty == expected);

        CHECK(STransUpdateDirtyArray(dirty.data(), 8, 63, 8, trans) == 1);

        expected = {
            0, 0, 2, 7, 7, 1, 0,
            0, 0, 2, 3, 3, 1, 0,
            0, 0, 0, 8, 8, 8, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
        };
        CHECK(dirty == expected);
    }
}
