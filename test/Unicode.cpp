#include "storm/Unicode.hpp"
#include "storm/String.hpp"
#include "test/Test.hpp"
#include <algorithm>

constexpr uint32_t RESULT_STR_SIZE = 100;

static std::pair<std::u16string, std::string> make_testcase(const std::u16string& strA, const std::string& strB) {
    return std::make_pair(strA, strB);
}

TEST_CASE("SUniConvertUTF16ToDos", "[unicode]") {
    SECTION("does nothing on empty dest size") {
        auto result = SUniConvertUTF16ToDos(nullptr, nullptr, 0);
        CHECK(result == 0);
    }

    SECTION("operates on empty string") {
        char resultStr[RESULT_STR_SIZE];
        const char16_t str[] = u"";

        auto result = SUniConvertUTF16ToDos(resultStr, str, RESULT_STR_SIZE);
        CHECK(std::string(resultStr) == "");
        CHECK(result == 1); // wrote 1 null terminator
    }

    SECTION("translates ASCII string") {
        char resultStr[RESULT_STR_SIZE];
        const char16_t str[] = u"Quick lazy brown fox or something";

        auto result = SUniConvertUTF16ToDos(resultStr, str, RESULT_STR_SIZE);
        CHECK(std::string(resultStr) == "Quick lazy brown fox or something");
        CHECK(result == 34);
    }

    SECTION("fails to write null terminator if string isn't long enough") {
        char resultStr[RESULT_STR_SIZE];
        std::fill(std::begin(resultStr), std::end(resultStr), 0xCC);
        const char16_t str[] = u"Quick lazy brown fox or something";

        auto result = SUniConvertUTF16ToDos(resultStr, str, 5);
        CHECK(std::string(resultStr, 5) == "Quick");
        CHECK(static_cast<uint8_t>(resultStr[5]) == 0xCC);
        CHECK(result == 5);
    }

    SECTION("writes ??? on unsupported characters") {
        char resultStr[RESULT_STR_SIZE];
        const char16_t str[] = u"\u8080\u8080lol\u8080\u8080\u8080";

        auto result = SUniConvertUTF16ToDos(resultStr, str, RESULT_STR_SIZE);
        CHECK(std::string(resultStr) == "??lol???");
        CHECK(result == 9);
    }

    SECTION("translates shared ASCII characters") {
        for (int i = 1; i < 128; i++) {
            char16_t str[2] = { static_cast<char16_t>(i) };
            char resultStr[2];

            auto result = SUniConvertUTF16ToDos(resultStr, str, 2);
            INFO(i);
            CHECK(resultStr[0] == static_cast<char>(i));
            CHECK(resultStr[1] == '\0');
            CHECK(result == 2);
        }
    }

    SECTION("translates special characters") {
        auto testCase = GENERATE(
            make_testcase(u"\u00C7", "\x80"),   // Ç
            make_testcase(u"\u00FC", "\x81"),   // ü
            make_testcase(u"\u00E9", "\x82"),   // é
            make_testcase(u"\u00E2", "\x83"),   // â
            make_testcase(u"\u00E4", "\x84"),   // ä
            make_testcase(u"\u00E0", "\x85"),   // à
            make_testcase(u"\u00E5", "\x86"),   // å
            make_testcase(u"\u00E7", "\x87"),   // ç
            make_testcase(u"\u00EA", "\x88"),   // ê
            make_testcase(u"\u00EB", "\x89"),   // ë
            make_testcase(u"\u00E8", "\x8A"),   // è
            make_testcase(u"\u00EF", "\x8B"),   // ï
            make_testcase(u"\u00EE", "\x8C"),   // î
            make_testcase(u"\u00EC", "\x8D"),   // ì
            make_testcase(u"\u00C4", "\x8E"),   // Ä
            make_testcase(u"\u00C5", "\x8F"),   // Å
            make_testcase(u"\u00C9", "\x90"),   // É
            make_testcase(u"\u00E6", "\x91"),   // æ
            make_testcase(u"\u00C6", "\x92"),   // Æ
            make_testcase(u"\u00F4", "\x93"),   // ô
            make_testcase(u"\u00F6", "\x94"),   // ö
            make_testcase(u"\u00F2", "\x95"),   // ò
            make_testcase(u"\u00FB", "\x96"),   // û
            make_testcase(u"\u00F9", "\x97"),   // ù
            make_testcase(u"\u00FF", "\x98"),   // ÿ
            make_testcase(u"\u00D6", "\x99"),   // Ö
            make_testcase(u"\u00DC", "\x9A"),   // Ü
            make_testcase(u"\u00A2", "\x9B"),   // ¢
            make_testcase(u"\u00A3", "\x9C"),   // £
            make_testcase(u"\u00A5", "\x9D"),   // ¥
            make_testcase(u"\u20A7", "\x9E"),   // ₧
            make_testcase(u"\u0192", "\x9F"),   // ƒ
            make_testcase(u"\u00E1", "\xA0"),   // á
            make_testcase(u"\u00ED", "\xA1"),   // í
            make_testcase(u"\u00F3", "\xA2"),   // ó
            make_testcase(u"\u00FA", "\xA3"),   // ú
            make_testcase(u"\u00F1", "\xA4"),   // ñ
            make_testcase(u"\u00D1", "\xA5"),   // Ñ
            make_testcase(u"\u00AA", "\xA6"),   // ª
            make_testcase(u"\u00BA", "\xA7"),   // º
            make_testcase(u"\u00BF", "\xA8"),   // ¿
            make_testcase(u"\u2310", "\xA9"),   // ⌐
            make_testcase(u"\u00AC", "\xAA"),   // ¬
            make_testcase(u"\u00BD", "\xAB"),   // ½
            make_testcase(u"\u00BC", "\xAC"),   // ¼
            make_testcase(u"\u00A1", "\xAD"),   // ¡
            make_testcase(u"\u00AB", "\xAE"),   // «
            make_testcase(u"\u00BB", "\xAF"),   // »
            make_testcase(u"\u2591", "\xB0"),   // ░
            make_testcase(u"\u2592", "\xB1"),   // ▒
            make_testcase(u"\u2593", "\xB2"),   // ▓
            make_testcase(u"\u2502", "\xB3"),   // │
            make_testcase(u"\u2524", "\xB4"),   // ┤
            make_testcase(u"\u2561", "\xB5"),   // ╡
            make_testcase(u"\u2562", "\xB6"),   // ╢
            make_testcase(u"\u2556", "\xB7"),   // ╖
            make_testcase(u"\u2555", "\xB8"),   // ╕
            make_testcase(u"\u2563", "\xB9"),   // ╣
            make_testcase(u"\u2551", "\xBA"),   // ║
            make_testcase(u"\u2557", "\xBB"),   // ╗
            make_testcase(u"\u255D", "\xBC"),   // ╝
            make_testcase(u"\u255C", "\xBD"),   // ╜
            make_testcase(u"\u255B", "\xBE"),   // ╛
            make_testcase(u"\u2510", "\xBF"),   // ┐
            make_testcase(u"\u2514", "\xC0"),   // └
            make_testcase(u"\u2534", "\xC1"),   // ┴
            make_testcase(u"\u252C", "\xC2"),   // ┬
            make_testcase(u"\u251C", "\xC3"),   // ├
            make_testcase(u"\u2500", "\xC4"),   // ─
            make_testcase(u"\u253C", "\xC5"),   // ┼
            make_testcase(u"\u255E", "\xC6"),   // ╞
            make_testcase(u"\u255F", "\xC7"),   // ╟
            make_testcase(u"\u255A", "\xC8"),   // ╚
            make_testcase(u"\u2554", "\xC9"),   // ╔
            make_testcase(u"\u2569", "\xCA"),   // ╩
            make_testcase(u"\u2566", "\xCB"),   // ╦
            make_testcase(u"\u2560", "\xCC"),   // ╠
            make_testcase(u"\u2550", "\xCD"),   // ═
            make_testcase(u"\u256C", "\xCE"),   // ╬
            make_testcase(u"\u2567", "\xCF"),   // ╧
            make_testcase(u"\u2568", "\xD0"),   // ╨
            make_testcase(u"\u2564", "\xD1"),   // ╤
            make_testcase(u"\u2565", "\xD2"),   // ╥
            make_testcase(u"\u2559", "\xD3"),   // ╙
            make_testcase(u"\u2558", "\xD4"),   // ╘
            make_testcase(u"\u2552", "\xD5"),   // ╒
            make_testcase(u"\u2553", "\xD6"),   // ╓
            make_testcase(u"\u256B", "\xD7"),   // ╫
            make_testcase(u"\u256A", "\xD8"),   // ╪
            make_testcase(u"\u2518", "\xD9"),   // ┘
            make_testcase(u"\u250C", "\xDA"),   // ┌
            make_testcase(u"\u2588", "\xDB"),   // █
            make_testcase(u"\u2584", "\xDC"),   // ▄
            make_testcase(u"\u258C", "\xDD"),   // ▌
            make_testcase(u"\u2590", "\xDE"),   // ▐
            make_testcase(u"\u2580", "\xDF"),   // ▀
            make_testcase(u"\u03B1", "\xE0"),   // α
            make_testcase(u"\u00DF", "\xE1"),   // ß
            make_testcase(u"\u0393", "\xE2"),   // Γ
            make_testcase(u"\u03C0", "\xE3"),   // π
            make_testcase(u"\u03A3", "\xE4"),   // Σ
            make_testcase(u"\u03C3", "\xE5"),   // σ
            make_testcase(u"\u00B5", "\xE6"),   // µ
            make_testcase(u"\u03C4", "\xE7"),   // τ
            make_testcase(u"\u03A6", "\xE8"),   // Φ
            make_testcase(u"\u0398", "\xE9"),   // Θ
            make_testcase(u"\u03A9", "\xEA"),   // Ω
            make_testcase(u"\u03B4", "\xEB"),   // δ
            make_testcase(u"\u221E", "\xEC"),   // ∞
            make_testcase(u"\u03C6", "\xED"),   // φ
            make_testcase(u"\u03B5", "\xEE"),   // ε
            make_testcase(u"\u2229", "\xEF"),   // ∩
            make_testcase(u"\u2261", "\xF0"),   // ≡
            make_testcase(u"\u00B1", "\xF1"),   // ±
            make_testcase(u"\u2265", "\xF2"),   // ≥
            make_testcase(u"\u2264", "\xF3"),   // ≤
            make_testcase(u"\u2320", "\xF4"),   // ⌠
            make_testcase(u"\u2321", "\xF5"),   // ⌡
            make_testcase(u"\u00F7", "\xF6"),   // ÷
            make_testcase(u"\u2248", "\xF7"),   // ≈
            make_testcase(u"\u00B0", "\xF8"),   // °
            make_testcase(u"\u2219", "\xF9"),   // ∙
            make_testcase(u"\u00B7", "\xFA"),   // ·
            make_testcase(u"\u221A", "\xFB"),   // √
            make_testcase(u"\u207F", "\xFC"),   // ⁿ
            make_testcase(u"\u00B2", "\xFD"),   // ²
            make_testcase(u"\u25A0", "\xFE"),   // ■
            make_testcase(u"\u00A0", "\xFF")    // r
        );

        char resultStr[2];
        auto result = SUniConvertUTF16ToDos(resultStr, testCase.first.c_str(), 2);

        INFO(testCase.second);
        CHECK(std::string(resultStr) == testCase.second);
        CHECK(result == 2);
    }
}

TEST_CASE("SUniConvertUTF16ToMac", "[unicode]") {
    SECTION("does nothing on empty dest size") {
        auto result = SUniConvertUTF16ToMac(nullptr, nullptr, 0);
        CHECK(result == 0);
    }

    SECTION("operates on empty string") {
        char resultStr[RESULT_STR_SIZE];
        const char16_t str[] = u"";

        auto result = SUniConvertUTF16ToMac(resultStr, str, RESULT_STR_SIZE);
        CHECK(std::string(resultStr) == "");
        CHECK(result == 1); // wrote 1 null terminator
    }

    SECTION("translates ASCII string") {
        char resultStr[RESULT_STR_SIZE];
        const char16_t str[] = u"Quick lazy brown fox or something";

        auto result = SUniConvertUTF16ToMac(resultStr, str, RESULT_STR_SIZE);
        CHECK(std::string(resultStr) == "Quick lazy brown fox or something");
        CHECK(result == 34);
    }

    SECTION("fails to write null terminator if string isn't long enough") {
        char resultStr[RESULT_STR_SIZE];
        std::fill(std::begin(resultStr), std::end(resultStr), 0xCC);
        const char16_t str[] = u"Quick lazy brown fox or something";

        auto result = SUniConvertUTF16ToMac(resultStr, str, 5);
        CHECK(std::string(resultStr, 5) == "Quick");
        CHECK(static_cast<uint8_t>(resultStr[5]) == 0xCC);
        CHECK(result == 5);
    }

    SECTION("writes ??? on unsupported characters") {
        char resultStr[RESULT_STR_SIZE];
        const char16_t str[] = u"\u8080\u8080lol\u8080\u8080\u8080";

        auto result = SUniConvertUTF16ToMac(resultStr, str, RESULT_STR_SIZE);
        CHECK(std::string(resultStr) == "??lol???");
        CHECK(result == 9);
    }

    SECTION("translates shared ASCII characters") {
        for (int i = 1; i < 128; i++) {
            char16_t str[2] = { static_cast<char16_t>(i) };
            char resultStr[2];

            auto result = SUniConvertUTF16ToMac(resultStr, str, 2);
            INFO(i);
            CHECK(resultStr[0] == static_cast<char>(i));
            CHECK(resultStr[1] == '\0');
            CHECK(result == 2);
        }
    }

    SECTION("translates special characters") {
        auto testCase = GENERATE(
            make_testcase(u"\u00C4", "\x80"), // Ä
            make_testcase(u"\u00C5", "\x81"), // Å
            make_testcase(u"\u00C7", "\x82"), // Ç
            make_testcase(u"\u00C9", "\x83"), // É
            make_testcase(u"\u00D1", "\x84"), // Ñ
            make_testcase(u"\u00D6", "\x85"), // Ö
            make_testcase(u"\u00DC", "\x86"), // Ü
            make_testcase(u"\u00E1", "\x87"), // á
            make_testcase(u"\u00E0", "\x88"), // à
            make_testcase(u"\u00E2", "\x89"), // â
            make_testcase(u"\u00E4", "\x8A"), // ä
            make_testcase(u"\u00E3", "\x8B"), // ã
            make_testcase(u"\u00E5", "\x8C"), // å
            make_testcase(u"\u00E7", "\x8D"), // ç
            make_testcase(u"\u00E9", "\x8E"), // é
            make_testcase(u"\u00E8", "\x8F"), // è
            make_testcase(u"\u00EA", "\x90"), // ê
            make_testcase(u"\u00EB", "\x91"), // ë
            make_testcase(u"\u00ED", "\x92"), // í
            make_testcase(u"\u00EC", "\x93"), // ì
            make_testcase(u"\u00EE", "\x94"), // î
            make_testcase(u"\u00EF", "\x95"), // ï
            make_testcase(u"\u00F1", "\x96"), // ñ
            make_testcase(u"\u00F3", "\x97"), // ó
            make_testcase(u"\u00F2", "\x98"), // ò
            make_testcase(u"\u00F4", "\x99"), // ô
            make_testcase(u"\u00F6", "\x9A"), // ö
            make_testcase(u"\u00F5", "\x9B"), // õ
            make_testcase(u"\u00FA", "\x9C"), // ú
            make_testcase(u"\u00F9", "\x9D"), // ù
            make_testcase(u"\u00FB", "\x9E"), // û
            make_testcase(u"\u00FC", "\x9F"), // ü
            make_testcase(u"\u2020", "\xA0"), // †
            make_testcase(u"\u00B0", "\xA1"), // °
            make_testcase(u"\u00A2", "\xA2"), // ¢
            make_testcase(u"\u00A3", "\xA3"), // £
            make_testcase(u"\u00A7", "\xA4"), // §
            make_testcase(u"\u2022", "\xA5"), // •
            make_testcase(u"\u00B6", "\xA6"), // ¶
            make_testcase(u"\u00DF", "\xA7"), // ß
            make_testcase(u"\u00AE", "\xA8"), // ®
            make_testcase(u"\u00A9", "\xA9"), // ©
            make_testcase(u"\u2122", "\xAA"), // ™
            make_testcase(u"\u00B4", "\xAB"), // ´
            make_testcase(u"\u00A8", "\xAC"), // ¨
            make_testcase(u"\u2260", "\xAD"), // ≠
            make_testcase(u"\u00C6", "\xAE"), // Æ
            make_testcase(u"\u00D8", "\xAF"), // Ø
            make_testcase(u"\u221E", "\xB0"), // ∞
            make_testcase(u"\u00B1", "\xB1"), // ±
            make_testcase(u"\u2264", "\xB2"), // ≤
            make_testcase(u"\u2265", "\xB3"), // ≥
            make_testcase(u"\u00A5", "\xB4"), // ¥
            make_testcase(u"\u00B5", "\xB5"), // µ
            make_testcase(u"\u2202", "\xB6"), // ∂
            make_testcase(u"\u2211", "\xB7"), // ∑
            make_testcase(u"\u220F", "\xB8"), // ∏
            make_testcase(u"\u03C0", "\xB9"), // π
            make_testcase(u"\u222B", "\xBA"), // ∫
            make_testcase(u"\u00AA", "\xBB"), // ª
            make_testcase(u"\u00BA", "\xBC"), // º
            make_testcase(u"\u2126", "\xBD"), // Ω
            make_testcase(u"\u00E6", "\xBE"), // æ
            make_testcase(u"\u00F8", "\xBF"), // ø
            make_testcase(u"\u00BF", "\xC0"), // ¿
            make_testcase(u"\u00A1", "\xC1"), // ¡
            make_testcase(u"\u00AC", "\xC2"), // ¬
            make_testcase(u"\u221A", "\xC3"), // √
            make_testcase(u"\u0192", "\xC4"), // ƒ
            make_testcase(u"\u2248", "\xC5"), // ≈
            make_testcase(u"\u2206", "\xC6"), // ∆
            make_testcase(u"\u00AB", "\xC7"), // «
            make_testcase(u"\u00BB", "\xC8"), // »
            make_testcase(u"\u2026", "\xC9"), // …
            make_testcase(u"\u00A0", "\xCA"), // NBSP
            make_testcase(u"\u00C0", "\xCB"), // À
            make_testcase(u"\u00C3", "\xCC"), // Ã
            make_testcase(u"\u00D5", "\xCD"), // Õ
            make_testcase(u"\u0152", "\xCE"), // Œ
            make_testcase(u"\u0153", "\xCF"), // œ
            make_testcase(u"\u2013", "\xD0"), // –
            make_testcase(u"\u2014", "\xD1"), // —
            make_testcase(u"\u201C", "\xD2"), // “
            make_testcase(u"\u201D", "\xD3"), // ”
            make_testcase(u"\u2018", "\xD4"), // ‘
            make_testcase(u"\u2019", "\xD5"), // ’
            make_testcase(u"\u00F7", "\xD6"), // ÷
            make_testcase(u"\u25CA", "\xD7"), // ◊
            make_testcase(u"\u00FF", "\xD8"), // ÿ
            make_testcase(u"\u0178", "\xD9"), // Ÿ
            make_testcase(u"\u2044", "\xDA"), // ⁄
            make_testcase(u"\u00A4", "\xDB"), // ¤
            make_testcase(u"\u2039", "\xDC"), // ‹
            make_testcase(u"\u203A", "\xDD"), // ›
            make_testcase(u"\uFB01", "\xDE"), // ﬁ
            make_testcase(u"\uFB02", "\xDF"), // ﬂ
            make_testcase(u"\u2021", "\xE0"), // ‡
            make_testcase(u"\u00B7", "\xE1"), // ·
            make_testcase(u"\u201A", "\xE2"), // ‚
            make_testcase(u"\u201E", "\xE3"), // „
            make_testcase(u"\u2030", "\xE4"), // ‰
            make_testcase(u"\u00C2", "\xE5"), // Â
            make_testcase(u"\u00CA", "\xE6"), // Ê
            make_testcase(u"\u00C1", "\xE7"), // Á
            make_testcase(u"\u00CB", "\xE8"), // Ë
            make_testcase(u"\u00C8", "\xE9"), // È
            make_testcase(u"\u00CD", "\xEA"), // Í
            make_testcase(u"\u00CE", "\xEB"), // Î
            make_testcase(u"\u00CF", "\xEC"), // Ï
            make_testcase(u"\u00CC", "\xED"), // Ì
            make_testcase(u"\u00D3", "\xEE"), // Ó
            make_testcase(u"\u00D4", "\xEF"), // Ô
            make_testcase(u"\uFFFF", "\xF0"), // invalid (apple logo)
            make_testcase(u"\u00D2", "\xF1"), // Ò
            make_testcase(u"\u00DA", "\xF2"), // Ú
            make_testcase(u"\u00DB", "\xF3"), // Û
            make_testcase(u"\u00D9", "\xF4"), // Ù
            make_testcase(u"\u0131", "\xF5"), // ı
            make_testcase(u"\u02C6", "\xF6"), // ˆ
            make_testcase(u"\u02DC", "\xF7"), // ˜
            make_testcase(u"\u00AF", "\xF8"), // ¯
            make_testcase(u"\u02D8", "\xF9"), // ˘
            make_testcase(u"\u02D9", "\xFA"), // ˙
            make_testcase(u"\u02DA", "\xFB"), // ˚
            make_testcase(u"\u00B8", "\xFC"), // ¸
            make_testcase(u"\u02DD", "\xFD"), // ˝
            make_testcase(u"\u02DB", "\xFE"), // ˛
            make_testcase(u"\u02C7", "\xFF")  // ˇ
        );

        char resultStr[2];
        auto result = SUniConvertUTF16ToMac(resultStr, testCase.first.c_str(), 2);

        INFO(testCase.second);
        CHECK(std::string(resultStr) == testCase.second);
        CHECK(result == 2);
    }
}

TEST_CASE("SUniConvertUTF16ToWin", "[unicode]") {
    SECTION("does nothing on empty dest size") {
        auto result = SUniConvertUTF16ToWin(nullptr, nullptr, 0);
        CHECK(result == 0);
    }

    SECTION("operates on empty string") {
        char resultStr[RESULT_STR_SIZE];
        const char16_t str[] = u"";

        auto result = SUniConvertUTF16ToWin(resultStr, str, RESULT_STR_SIZE);
        CHECK(std::string(resultStr) == "");
        CHECK(result == 1); // wrote 1 null terminator
    }

    SECTION("translates ASCII string") {
        char resultStr[RESULT_STR_SIZE];
        const char16_t str[] = u"Quick lazy brown fox or something";

        auto result = SUniConvertUTF16ToWin(resultStr, str, RESULT_STR_SIZE);
        CHECK(std::string(resultStr) == "Quick lazy brown fox or something");
        CHECK(result == 34);
    }

    SECTION("fails to write null terminator if string isn't long enough") {
        char resultStr[RESULT_STR_SIZE];
        std::fill(std::begin(resultStr), std::end(resultStr), 0xCC);
        const char16_t str[] = u"Quick lazy brown fox or something";

        auto result = SUniConvertUTF16ToWin(resultStr, str, 5);
        CHECK(std::string(resultStr, 5) == "Quick");
        CHECK(static_cast<uint8_t>(resultStr[5]) == 0xCC);
        CHECK(result == 5);
    }

    SECTION("writes ??? on unsupported characters") {
        char resultStr[RESULT_STR_SIZE];
        const char16_t str[] = u"\u8080\u8080lol\u8080\u8080\u8080";

        auto result = SUniConvertUTF16ToWin(resultStr, str, RESULT_STR_SIZE);
        CHECK(std::string(resultStr) == "??lol???");
        CHECK(result == 9);
    }

    SECTION("translates shared ASCII characters") {
        for (int i = 1; i < 128; i++) {
            char16_t str[2] = { static_cast<char16_t>(i) };
            char resultStr[2];

            auto result = SUniConvertUTF16ToWin(resultStr, str, 2);
            INFO(i);
            CHECK(resultStr[0] == static_cast<char>(i));
            CHECK(resultStr[1] == '\0');
            CHECK(result == 2);
        }
    }

    SECTION("translates special characters") {
        auto testCase = GENERATE(
            make_testcase(u"\u20AC", "\x80"),     // €
            //make_testcase(u"\uFFFF", "\x81"),   // invalid
            make_testcase(u"\u201A", "\x82"),     // ‚
            make_testcase(u"\u0192", "\x83"),     // ƒ
            make_testcase(u"\u201E", "\x84"),     // „
            make_testcase(u"\u2026", "\x85"),     // …
            make_testcase(u"\u2020", "\x86"),     // †
            make_testcase(u"\u2021", "\x87"),     // ‡
            make_testcase(u"\u02C6", "\x88"),     // ˆ
            make_testcase(u"\u2030", "\x89"),     // ‰
            make_testcase(u"\u0160", "\x8A"),     // Š
            make_testcase(u"\u2039", "\x8B"),     // ‹
            make_testcase(u"\u0152", "\x8C"),     // Œ
            //make_testcase(u"\uFFFF", "\x8D"),   // invalid
            make_testcase(u"\u017D", "\x8E"),     // Ž
            //make_testcase(u"\uFFFF", "\x8F"),   // invalid
            //make_testcase(u"\uFFFF", "\x90"),   // invalid
            make_testcase(u"\u2018", "\x91"),     // ‘
            make_testcase(u"\u2019", "\x92"),     // ’
            make_testcase(u"\u201C", "\x93"),     // “
            make_testcase(u"\u201D", "\x94"),     // ”
            make_testcase(u"\u2022", "\x95"),     // •
            make_testcase(u"\u2013", "\x96"),     // –
            make_testcase(u"\u2014", "\x97"),     // —
            make_testcase(u"\u02DC", "\x98"),     // ˜
            make_testcase(u"\u2122", "\x99"),     // ™
            make_testcase(u"\u0161", "\x9A"),     // š
            make_testcase(u"\u203A", "\x9B"),     // ›
            make_testcase(u"\u0153", "\x9C"),     // œ
            make_testcase(u"\uFFFF", "\x9D"),     // invalid
            make_testcase(u"\u017E", "\x9E"),     // ž
            make_testcase(u"\u0178", "\x9F")      // Ÿ
        );

        char resultStr[2];
        auto result = SUniConvertUTF16ToWin(resultStr, testCase.first.c_str(), 2);

        INFO(testCase.second);
        CHECK(std::string(resultStr) == testCase.second);
        CHECK(result == 2);
    }

    SECTION("translates extended ASCII characters") {
        for (int i = 0xA0; i < 256; i++) {
            char16_t str[2] = { static_cast<char16_t>(i) };
            char resultStr[2];

            auto result = SUniConvertUTF16ToWin(resultStr, str, 2);
            INFO(i);
            CHECK(resultStr[0] == static_cast<char>(i));
            CHECK(resultStr[1] == '\0');
            CHECK(result == 2);
        }
    }

    // TODO: Starcraft flavour Korean codepage
}

TEST_CASE("SUniConvertDosToUTF16", "[unicode]") {
    SECTION("does nothing on empty dest size") {
        auto result = SUniConvertDosToUTF16(nullptr, nullptr, 0);
        CHECK(result == 0);
    }

    SECTION("operates on empty string") {
        char16_t resultStr[RESULT_STR_SIZE];
        const char str[] = "";

        auto result = SUniConvertDosToUTF16(resultStr, str, RESULT_STR_SIZE);
        CHECK(std::u16string(resultStr) == u"");
        CHECK(result == 1); // wrote 1 null terminator
    }

    SECTION("translates ASCII string") {
        char16_t resultStr[RESULT_STR_SIZE];
        const char str[] = "Quick lazy brown fox or something";

        auto result = SUniConvertDosToUTF16(resultStr, str, RESULT_STR_SIZE);
        CHECK(std::u16string(resultStr) == u"Quick lazy brown fox or something");
        CHECK(result == 34);
    }

    SECTION("fails to write null terminator if string isn't long enough") {
        char16_t resultStr[RESULT_STR_SIZE];
        std::fill(std::begin(resultStr), std::end(resultStr), 0xCCCC);
        const char str[] = "Quick lazy brown fox or something";

        auto result = SUniConvertDosToUTF16(resultStr, str, 5);
        CHECK(std::u16string(resultStr, 5) == u"Quick");
        CHECK(static_cast<uint16_t>(resultStr[5]) == 0xCCCC);
        CHECK(result == 5);
    }

    SECTION("translates shared ASCII characters") {
        for (int i = 1; i < 128; i++) {
            char str[2] = { static_cast<char>(i) };
            char16_t resultStr[2];

            auto result = SUniConvertDosToUTF16(resultStr, str, 2);
            INFO(i);
            CHECK(resultStr[0] == static_cast<char16_t>(i));
            CHECK(resultStr[1] == L'\0');
            CHECK(result == 2);
        }
    }

    SECTION("translates special characters") {
        auto testCase = GENERATE(
            make_testcase(u"\u00C7", "\x80"),   // Ç
            make_testcase(u"\u00FC", "\x81"),   // ü
            make_testcase(u"\u00E9", "\x82"),   // é
            make_testcase(u"\u00E2", "\x83"),   // â
            make_testcase(u"\u00E4", "\x84"),   // ä
            make_testcase(u"\u00E0", "\x85"),   // à
            make_testcase(u"\u00E5", "\x86"),   // å
            make_testcase(u"\u00E7", "\x87"),   // ç
            make_testcase(u"\u00EA", "\x88"),   // ê
            make_testcase(u"\u00EB", "\x89"),   // ë
            make_testcase(u"\u00E8", "\x8A"),   // è
            make_testcase(u"\u00EF", "\x8B"),   // ï
            make_testcase(u"\u00EE", "\x8C"),   // î
            make_testcase(u"\u00EC", "\x8D"),   // ì
            make_testcase(u"\u00C4", "\x8E"),   // Ä
            make_testcase(u"\u00C5", "\x8F"),   // Å
            make_testcase(u"\u00C9", "\x90"),   // É
            make_testcase(u"\u00E6", "\x91"),   // æ
            make_testcase(u"\u00C6", "\x92"),   // Æ
            make_testcase(u"\u00F4", "\x93"),   // ô
            make_testcase(u"\u00F6", "\x94"),   // ö
            make_testcase(u"\u00F2", "\x95"),   // ò
            make_testcase(u"\u00FB", "\x96"),   // û
            make_testcase(u"\u00F9", "\x97"),   // ù
            make_testcase(u"\u00FF", "\x98"),   // ÿ
            make_testcase(u"\u00D6", "\x99"),   // Ö
            make_testcase(u"\u00DC", "\x9A"),   // Ü
            make_testcase(u"\u00A2", "\x9B"),   // ¢
            make_testcase(u"\u00A3", "\x9C"),   // £
            make_testcase(u"\u00A5", "\x9D"),   // ¥
            make_testcase(u"\u20A7", "\x9E"),   // ₧
            make_testcase(u"\u0192", "\x9F"),   // ƒ
            make_testcase(u"\u00E1", "\xA0"),   // á
            make_testcase(u"\u00ED", "\xA1"),   // í
            make_testcase(u"\u00F3", "\xA2"),   // ó
            make_testcase(u"\u00FA", "\xA3"),   // ú
            make_testcase(u"\u00F1", "\xA4"),   // ñ
            make_testcase(u"\u00D1", "\xA5"),   // Ñ
            make_testcase(u"\u00AA", "\xA6"),   // ª
            make_testcase(u"\u00BA", "\xA7"),   // º
            make_testcase(u"\u00BF", "\xA8"),   // ¿
            make_testcase(u"\u2310", "\xA9"),   // ⌐
            make_testcase(u"\u00AC", "\xAA"),   // ¬
            make_testcase(u"\u00BD", "\xAB"),   // ½
            make_testcase(u"\u00BC", "\xAC"),   // ¼
            make_testcase(u"\u00A1", "\xAD"),   // ¡
            make_testcase(u"\u00AB", "\xAE"),   // «
            make_testcase(u"\u00BB", "\xAF"),   // »
            make_testcase(u"\u2591", "\xB0"),   // ░
            make_testcase(u"\u2592", "\xB1"),   // ▒
            make_testcase(u"\u2593", "\xB2"),   // ▓
            make_testcase(u"\u2502", "\xB3"),   // │
            make_testcase(u"\u2524", "\xB4"),   // ┤
            make_testcase(u"\u2561", "\xB5"),   // ╡
            make_testcase(u"\u2562", "\xB6"),   // ╢
            make_testcase(u"\u2556", "\xB7"),   // ╖
            make_testcase(u"\u2555", "\xB8"),   // ╕
            make_testcase(u"\u2563", "\xB9"),   // ╣
            make_testcase(u"\u2551", "\xBA"),   // ║
            make_testcase(u"\u2557", "\xBB"),   // ╗
            make_testcase(u"\u255D", "\xBC"),   // ╝
            make_testcase(u"\u255C", "\xBD"),   // ╜
            make_testcase(u"\u255B", "\xBE"),   // ╛
            make_testcase(u"\u2510", "\xBF"),   // ┐
            make_testcase(u"\u2514", "\xC0"),   // └
            make_testcase(u"\u2534", "\xC1"),   // ┴
            make_testcase(u"\u252C", "\xC2"),   // ┬
            make_testcase(u"\u251C", "\xC3"),   // ├
            make_testcase(u"\u2500", "\xC4"),   // ─
            make_testcase(u"\u253C", "\xC5"),   // ┼
            make_testcase(u"\u255E", "\xC6"),   // ╞
            make_testcase(u"\u255F", "\xC7"),   // ╟
            make_testcase(u"\u255A", "\xC8"),   // ╚
            make_testcase(u"\u2554", "\xC9"),   // ╔
            make_testcase(u"\u2569", "\xCA"),   // ╩
            make_testcase(u"\u2566", "\xCB"),   // ╦
            make_testcase(u"\u2560", "\xCC"),   // ╠
            make_testcase(u"\u2550", "\xCD"),   // ═
            make_testcase(u"\u256C", "\xCE"),   // ╬
            make_testcase(u"\u2567", "\xCF"),   // ╧
            make_testcase(u"\u2568", "\xD0"),   // ╨
            make_testcase(u"\u2564", "\xD1"),   // ╤
            make_testcase(u"\u2565", "\xD2"),   // ╥
            make_testcase(u"\u2559", "\xD3"),   // ╙
            make_testcase(u"\u2558", "\xD4"),   // ╘
            make_testcase(u"\u2552", "\xD5"),   // ╒
            make_testcase(u"\u2553", "\xD6"),   // ╓
            make_testcase(u"\u256B", "\xD7"),   // ╫
            make_testcase(u"\u256A", "\xD8"),   // ╪
            make_testcase(u"\u2518", "\xD9"),   // ┘
            make_testcase(u"\u250C", "\xDA"),   // ┌
            make_testcase(u"\u2588", "\xDB"),   // █
            make_testcase(u"\u2584", "\xDC"),   // ▄
            make_testcase(u"\u258C", "\xDD"),   // ▌
            make_testcase(u"\u2590", "\xDE"),   // ▐
            make_testcase(u"\u2580", "\xDF"),   // ▀
            make_testcase(u"\u03B1", "\xE0"),   // α
            make_testcase(u"\u00DF", "\xE1"),   // ß
            make_testcase(u"\u0393", "\xE2"),   // Γ
            make_testcase(u"\u03C0", "\xE3"),   // π
            make_testcase(u"\u03A3", "\xE4"),   // Σ
            make_testcase(u"\u03C3", "\xE5"),   // σ
            make_testcase(u"\u00B5", "\xE6"),   // µ
            make_testcase(u"\u03C4", "\xE7"),   // τ
            make_testcase(u"\u03A6", "\xE8"),   // Φ
            make_testcase(u"\u0398", "\xE9"),   // Θ
            make_testcase(u"\u03A9", "\xEA"),   // Ω
            make_testcase(u"\u03B4", "\xEB"),   // δ
            make_testcase(u"\u221E", "\xEC"),   // ∞
            make_testcase(u"\u03C6", "\xED"),   // φ
            make_testcase(u"\u03B5", "\xEE"),   // ε
            make_testcase(u"\u2229", "\xEF"),   // ∩
            make_testcase(u"\u2261", "\xF0"),   // ≡
            make_testcase(u"\u00B1", "\xF1"),   // ±
            make_testcase(u"\u2265", "\xF2"),   // ≥
            make_testcase(u"\u2264", "\xF3"),   // ≤
            make_testcase(u"\u2320", "\xF4"),   // ⌠
            make_testcase(u"\u2321", "\xF5"),   // ⌡
            make_testcase(u"\u00F7", "\xF6"),   // ÷
            make_testcase(u"\u2248", "\xF7"),   // ≈
            make_testcase(u"\u00B0", "\xF8"),   // °
            make_testcase(u"\u2219", "\xF9"),   // ∙
            make_testcase(u"\u00B7", "\xFA"),   // ·
            make_testcase(u"\u221A", "\xFB"),   // √
            make_testcase(u"\u207F", "\xFC"),   // ⁿ
            make_testcase(u"\u00B2", "\xFD"),   // ²
            make_testcase(u"\u25A0", "\xFE"),   // ■
            make_testcase(u"\u00A0", "\xFF")    // r
        );

        char16_t resultStr[2];
        auto result = SUniConvertDosToUTF16(resultStr, testCase.second.c_str(), 2);

        INFO(testCase.second);
        CHECK(std::u16string(resultStr) == testCase.first);
        CHECK(result == 2);
    }
}

TEST_CASE("SUniConvertMacToUTF16", "[unicode]") {
    SECTION("does nothing on empty dest size") {
        auto result = SUniConvertMacToUTF16(nullptr, nullptr, 0);
        CHECK(result == 0);
    }

    SECTION("operates on empty string") {
        char16_t resultStr[RESULT_STR_SIZE];
        const char str[] = "";

        auto result = SUniConvertMacToUTF16(resultStr, str, RESULT_STR_SIZE);
        CHECK(std::u16string(resultStr) == u"");
        CHECK(result == 1); // wrote 1 null terminator
    }

    SECTION("translates ASCII string") {
        char16_t resultStr[RESULT_STR_SIZE];
        const char str[] = "Quick lazy brown fox or something";

        auto result = SUniConvertMacToUTF16(resultStr, str, RESULT_STR_SIZE);
        CHECK(std::u16string(resultStr) == u"Quick lazy brown fox or something");
        CHECK(result == 34);
    }

    SECTION("fails to write null terminator if string isn't long enough") {
        char16_t resultStr[RESULT_STR_SIZE];
        std::fill(std::begin(resultStr), std::end(resultStr), 0xCCCC);
        const char str[] = "Quick lazy brown fox or something";

        auto result = SUniConvertMacToUTF16(resultStr, str, 5);
        CHECK(std::u16string(resultStr, 5) == u"Quick");
        CHECK(static_cast<uint16_t>(resultStr[5]) == 0xCCCC);
        CHECK(result == 5);
    }

    SECTION("translates shared ASCII characters") {
        for (int i = 1; i < 128; i++) {
            char str[2] = { static_cast<char>(i) };
            char16_t resultStr[2];

            auto result = SUniConvertMacToUTF16(resultStr, str, 2);
            INFO(i);
            CHECK(resultStr[0] == static_cast<char16_t>(i));
            CHECK(resultStr[1] == L'\0');
            CHECK(result == 2);
        }
    }

    SECTION("translates special characters") {
        auto testCase = GENERATE(
            make_testcase(u"\u00C4", "\x80"), // Ä
            make_testcase(u"\u00C5", "\x81"), // Å
            make_testcase(u"\u00C7", "\x82"), // Ç
            make_testcase(u"\u00C9", "\x83"), // É
            make_testcase(u"\u00D1", "\x84"), // Ñ
            make_testcase(u"\u00D6", "\x85"), // Ö
            make_testcase(u"\u00DC", "\x86"), // Ü
            make_testcase(u"\u00E1", "\x87"), // á
            make_testcase(u"\u00E0", "\x88"), // à
            make_testcase(u"\u00E2", "\x89"), // â
            make_testcase(u"\u00E4", "\x8A"), // ä
            make_testcase(u"\u00E3", "\x8B"), // ã
            make_testcase(u"\u00E5", "\x8C"), // å
            make_testcase(u"\u00E7", "\x8D"), // ç
            make_testcase(u"\u00E9", "\x8E"), // é
            make_testcase(u"\u00E8", "\x8F"), // è
            make_testcase(u"\u00EA", "\x90"), // ê
            make_testcase(u"\u00EB", "\x91"), // ë
            make_testcase(u"\u00ED", "\x92"), // í
            make_testcase(u"\u00EC", "\x93"), // ì
            make_testcase(u"\u00EE", "\x94"), // î
            make_testcase(u"\u00EF", "\x95"), // ï
            make_testcase(u"\u00F1", "\x96"), // ñ
            make_testcase(u"\u00F3", "\x97"), // ó
            make_testcase(u"\u00F2", "\x98"), // ò
            make_testcase(u"\u00F4", "\x99"), // ô
            make_testcase(u"\u00F6", "\x9A"), // ö
            make_testcase(u"\u00F5", "\x9B"), // õ
            make_testcase(u"\u00FA", "\x9C"), // ú
            make_testcase(u"\u00F9", "\x9D"), // ù
            make_testcase(u"\u00FB", "\x9E"), // û
            make_testcase(u"\u00FC", "\x9F"), // ü
            make_testcase(u"\u2020", "\xA0"), // †
            make_testcase(u"\u00B0", "\xA1"), // °
            make_testcase(u"\u00A2", "\xA2"), // ¢
            make_testcase(u"\u00A3", "\xA3"), // £
            make_testcase(u"\u00A7", "\xA4"), // §
            make_testcase(u"\u2022", "\xA5"), // •
            make_testcase(u"\u00B6", "\xA6"), // ¶
            make_testcase(u"\u00DF", "\xA7"), // ß
            make_testcase(u"\u00AE", "\xA8"), // ®
            make_testcase(u"\u00A9", "\xA9"), // ©
            make_testcase(u"\u2122", "\xAA"), // ™
            make_testcase(u"\u00B4", "\xAB"), // ´
            make_testcase(u"\u00A8", "\xAC"), // ¨
            make_testcase(u"\u2260", "\xAD"), // ≠
            make_testcase(u"\u00C6", "\xAE"), // Æ
            make_testcase(u"\u00D8", "\xAF"), // Ø
            make_testcase(u"\u221E", "\xB0"), // ∞
            make_testcase(u"\u00B1", "\xB1"), // ±
            make_testcase(u"\u2264", "\xB2"), // ≤
            make_testcase(u"\u2265", "\xB3"), // ≥
            make_testcase(u"\u00A5", "\xB4"), // ¥
            make_testcase(u"\u00B5", "\xB5"), // µ
            make_testcase(u"\u2202", "\xB6"), // ∂
            make_testcase(u"\u2211", "\xB7"), // ∑
            make_testcase(u"\u220F", "\xB8"), // ∏
            make_testcase(u"\u03C0", "\xB9"), // π
            make_testcase(u"\u222B", "\xBA"), // ∫
            make_testcase(u"\u00AA", "\xBB"), // ª
            make_testcase(u"\u00BA", "\xBC"), // º
            make_testcase(u"\u2126", "\xBD"), // Ω
            make_testcase(u"\u00E6", "\xBE"), // æ
            make_testcase(u"\u00F8", "\xBF"), // ø
            make_testcase(u"\u00BF", "\xC0"), // ¿
            make_testcase(u"\u00A1", "\xC1"), // ¡
            make_testcase(u"\u00AC", "\xC2"), // ¬
            make_testcase(u"\u221A", "\xC3"), // √
            make_testcase(u"\u0192", "\xC4"), // ƒ
            make_testcase(u"\u2248", "\xC5"), // ≈
            make_testcase(u"\u2206", "\xC6"), // ∆
            make_testcase(u"\u00AB", "\xC7"), // «
            make_testcase(u"\u00BB", "\xC8"), // »
            make_testcase(u"\u2026", "\xC9"), // …
            make_testcase(u"\u00A0", "\xCA"), // NBSP
            make_testcase(u"\u00C0", "\xCB"), // À
            make_testcase(u"\u00C3", "\xCC"), // Ã
            make_testcase(u"\u00D5", "\xCD"), // Õ
            make_testcase(u"\u0152", "\xCE"), // Œ
            make_testcase(u"\u0153", "\xCF"), // œ
            make_testcase(u"\u2013", "\xD0"), // –
            make_testcase(u"\u2014", "\xD1"), // —
            make_testcase(u"\u201C", "\xD2"), // “
            make_testcase(u"\u201D", "\xD3"), // ”
            make_testcase(u"\u2018", "\xD4"), // ‘
            make_testcase(u"\u2019", "\xD5"), // ’
            make_testcase(u"\u00F7", "\xD6"), // ÷
            make_testcase(u"\u25CA", "\xD7"), // ◊
            make_testcase(u"\u00FF", "\xD8"), // ÿ
            make_testcase(u"\u0178", "\xD9"), // Ÿ
            make_testcase(u"\u2044", "\xDA"), // ⁄
            make_testcase(u"\u00A4", "\xDB"), // ¤
            make_testcase(u"\u2039", "\xDC"), // ‹
            make_testcase(u"\u203A", "\xDD"), // ›
            make_testcase(u"\uFB01", "\xDE"), // ﬁ
            make_testcase(u"\uFB02", "\xDF"), // ﬂ
            make_testcase(u"\u2021", "\xE0"), // ‡
            make_testcase(u"\u00B7", "\xE1"), // ·
            make_testcase(u"\u201A", "\xE2"), // ‚
            make_testcase(u"\u201E", "\xE3"), // „
            make_testcase(u"\u2030", "\xE4"), // ‰
            make_testcase(u"\u00C2", "\xE5"), // Â
            make_testcase(u"\u00CA", "\xE6"), // Ê
            make_testcase(u"\u00C1", "\xE7"), // Á
            make_testcase(u"\u00CB", "\xE8"), // Ë
            make_testcase(u"\u00C8", "\xE9"), // È
            make_testcase(u"\u00CD", "\xEA"), // Í
            make_testcase(u"\u00CE", "\xEB"), // Î
            make_testcase(u"\u00CF", "\xEC"), // Ï
            make_testcase(u"\u00CC", "\xED"), // Ì
            make_testcase(u"\u00D3", "\xEE"), // Ó
            make_testcase(u"\u00D4", "\xEF"), // Ô
            make_testcase(u"\uFFFF", "\xF0"), // invalid (apple logo)
            make_testcase(u"\u00D2", "\xF1"), // Ò
            make_testcase(u"\u00DA", "\xF2"), // Ú
            make_testcase(u"\u00DB", "\xF3"), // Û
            make_testcase(u"\u00D9", "\xF4"), // Ù
            make_testcase(u"\u0131", "\xF5"), // ı
            make_testcase(u"\u02C6", "\xF6"), // ˆ
            make_testcase(u"\u02DC", "\xF7"), // ˜
            make_testcase(u"\u00AF", "\xF8"), // ¯
            make_testcase(u"\u02D8", "\xF9"), // ˘
            make_testcase(u"\u02D9", "\xFA"), // ˙
            make_testcase(u"\u02DA", "\xFB"), // ˚
            make_testcase(u"\u00B8", "\xFC"), // ¸
            make_testcase(u"\u02DD", "\xFD"), // ˝
            make_testcase(u"\u02DB", "\xFE"), // ˛
            make_testcase(u"\u02C7", "\xFF")  // ˇ
        );

        char16_t resultStr[2];
        auto result = SUniConvertMacToUTF16(resultStr, testCase.second.c_str(), 2);

        INFO(testCase.second);
        CHECK(std::u16string(resultStr) == testCase.first);
        CHECK(result == 2);
    }
}

TEST_CASE("SUniConvertWinToUTF16", "[unicode]") {
    SECTION("does nothing on empty dest size") {
        auto result = SUniConvertWinToUTF16(nullptr, nullptr, 0);
        CHECK(result == 0);
    }

    SECTION("operates on empty string") {
        char16_t resultStr[RESULT_STR_SIZE];
        const char str[] = "";

        auto result = SUniConvertWinToUTF16(resultStr, str, RESULT_STR_SIZE);
        CHECK(std::u16string(resultStr) == u"");
        CHECK(result == 1); // wrote 1 null terminator
    }

    SECTION("translates ASCII string") {
        char16_t resultStr[RESULT_STR_SIZE];
        const char str[] = "Quick lazy brown fox or something";

        auto result = SUniConvertWinToUTF16(resultStr, str, RESULT_STR_SIZE);
        CHECK(std::u16string(resultStr) == u"Quick lazy brown fox or something");
        CHECK(result == 34);
    }

    SECTION("fails to write null terminator if string isn't long enough") {
        char16_t resultStr[RESULT_STR_SIZE];
        std::fill(std::begin(resultStr), std::end(resultStr), 0xCCCC);
        const char str[] = "Quick lazy brown fox or something";

        auto result = SUniConvertWinToUTF16(resultStr, str, 5);
        CHECK(std::u16string(resultStr, 5) == u"Quick");
        CHECK(static_cast<uint16_t>(resultStr[5]) == 0xCCCC);
        CHECK(result == 5);
    }

    SECTION("translates shared ASCII characters") {
        for (int i = 1; i < 128; i++) {
            char str[2] = { static_cast<char>(i) };
            char16_t resultStr[2];

            auto result = SUniConvertWinToUTF16(resultStr, str, 2);
            INFO(i);
            CHECK(resultStr[0] == static_cast<char16_t>(i));
            CHECK(resultStr[1] == L'\0');
            CHECK(result == 2);
        }
    }

    SECTION("translates special characters") {
        auto testCase = GENERATE(
            make_testcase(u"\u20AC", "\x80"),     // €
            make_testcase(u"\uFFFF", "\x81"),     // invalid
            make_testcase(u"\u201A", "\x82"),     // ‚
            make_testcase(u"\u0192", "\x83"),     // ƒ
            make_testcase(u"\u201E", "\x84"),     // „
            make_testcase(u"\u2026", "\x85"),     // …
            make_testcase(u"\u2020", "\x86"),     // †
            make_testcase(u"\u2021", "\x87"),     // ‡
            make_testcase(u"\u02C6", "\x88"),     // ˆ
            make_testcase(u"\u2030", "\x89"),     // ‰
            make_testcase(u"\u0160", "\x8A"),     // Š
            make_testcase(u"\u2039", "\x8B"),     // ‹
            make_testcase(u"\u0152", "\x8C"),     // Œ
            make_testcase(u"\uFFFF", "\x8D"),     // invalid
            make_testcase(u"\u017D", "\x8E"),     // Ž
            make_testcase(u"\uFFFF", "\x8F"),     // invalid
            make_testcase(u"\uFFFF", "\x90"),     // invalid
            make_testcase(u"\u2018", "\x91"),     // ‘
            make_testcase(u"\u2019", "\x92"),     // ’
            make_testcase(u"\u201C", "\x93"),     // “
            make_testcase(u"\u201D", "\x94"),     // ”
            make_testcase(u"\u2022", "\x95"),     // •
            make_testcase(u"\u2013", "\x96"),     // –
            make_testcase(u"\u2014", "\x97"),     // —
            make_testcase(u"\u02DC", "\x98"),     // ˜
            make_testcase(u"\u2122", "\x99"),     // ™
            make_testcase(u"\u0161", "\x9A"),     // š
            make_testcase(u"\u203A", "\x9B"),     // ›
            make_testcase(u"\u0153", "\x9C"),     // œ
            make_testcase(u"\uFFFF", "\x9D"),     // invalid
            make_testcase(u"\u017E", "\x9E"),     // ž
            make_testcase(u"\u0178", "\x9F")      // Ÿ
        );

        char16_t resultStr[2];
        auto result = SUniConvertWinToUTF16(resultStr, testCase.second.c_str(), 2);

        INFO(testCase.second);
        CHECK(std::u16string(resultStr) == testCase.first);
        CHECK(result == 2);
    }

    SECTION("translates extended ASCII characters") {
        for (int i = 0xA0; i < 256; i++) {
            char str[2] = { static_cast<char>(i) };
            char16_t resultStr[2];

            auto result = SUniConvertWinToUTF16(resultStr, str, 2);
            INFO(i);
            CHECK(resultStr[0] == static_cast<char16_t>(i));
            CHECK(resultStr[1] == u'\0');
            CHECK(result == 2);
        }
    }

    // TODO: Starcraft flavour Korean codepage
}

TEST_CASE("SUniFindAfterUTF8Chr", "[unicode]") {
    SECTION("returns index+1 if index is negative") {
        CHECK(SUniFindAfterUTF8Chr(nullptr, -1) == 0);
    }

    SECTION("returns 1 on string with 1 char") {
        CHECK(SUniFindAfterUTF8Chr(" ", 0) == 1);
    }

    SECTION("returns next char on non-UTF8 string") {
        CHECK(SUniFindAfterUTF8Chr("happy cakes", 4) == 5);
    }

    SECTION("finds the character after an emoji") {
        const char str[] = "Merry 🎄 Christmas";
        CHECK(sizeof(str) == 21);
        CHECK(SUniFindAfterUTF8Chr(str, 4) == 5);
        CHECK(SUniFindAfterUTF8Chr(str, 5) == 6);
        CHECK(SUniFindAfterUTF8Chr(str, 6) == 10);
        CHECK(SUniFindAfterUTF8Chr(str, 7) == 10);
        CHECK(SUniFindAfterUTF8Chr(str, 8) == 10);
        CHECK(SUniFindAfterUTF8Chr(str, 9) == 10);
        CHECK(SUniFindAfterUTF8Chr(str, 10) == 11);
        CHECK(SUniFindAfterUTF8Chr(str, 11) == 12);
    }

    SECTION("finds the character after a Korean char") {
        const char str[] = "메리 크리스마스";
        CHECK(sizeof(str) == 23);
        CHECK(SUniFindAfterUTF8Chr(str, 3) == 6);
        CHECK(SUniFindAfterUTF8Chr(str, 4) == 6);
        CHECK(SUniFindAfterUTF8Chr(str, 5) == 6);
        CHECK(SUniFindAfterUTF8Chr(str, 6) == 7); // ASCII space
        CHECK(SUniFindAfterUTF8Chr(str, 7) == 10);
        CHECK(SUniFindAfterUTF8Chr(str, 8) == 10);
        CHECK(SUniFindAfterUTF8Chr(str, 9) == 10);
    }
}

TEST_CASE("SUniFindUTF8ChrStart", "[unicode]") {
    SECTION("returns index if index is negative") {
        CHECK(SUniFindUTF8ChrStart(nullptr, -1) == -1);
    }

    SECTION("returns 0 if index is 0") {
        CHECK(SUniFindUTF8ChrStart(nullptr, 0) == 0);
    }

    SECTION("returns 0 on empty string") {
        CHECK(SUniFindUTF8ChrStart("", 0) == 0);
    }

    SECTION("returns current char on non-UTF8 string") {
        CHECK(SUniFindUTF8ChrStart("happy cakes", 5) == 5);
    }

    SECTION("finds the beginning of an emoji") {
        const char str[] = "Merry 🎄 Christmas";
        CHECK(sizeof(str) == 21);
        CHECK(SUniFindUTF8ChrStart(str, 4) == 4);
        CHECK(SUniFindUTF8ChrStart(str, 5) == 5);
        CHECK(SUniFindUTF8ChrStart(str, 6) == 6);
        CHECK(SUniFindUTF8ChrStart(str, 7) == 6);
        CHECK(SUniFindUTF8ChrStart(str, 8) == 6);
        CHECK(SUniFindUTF8ChrStart(str, 9) == 6);
        CHECK(SUniFindUTF8ChrStart(str, 10) == 10);
        CHECK(SUniFindUTF8ChrStart(str, 11) == 11);
    }

    SECTION("finds the beginning of a Korean char") {
        const char str[] = "메리 크리스마스";
        CHECK(sizeof(str) == 23);
        CHECK(SUniFindUTF8ChrStart(str, 3) == 3);
        CHECK(SUniFindUTF8ChrStart(str, 4) == 3);
        CHECK(SUniFindUTF8ChrStart(str, 5) == 3);
        CHECK(SUniFindUTF8ChrStart(str, 6) == 6); // ASCII space
        CHECK(SUniFindUTF8ChrStart(str, 7) == 7);
        CHECK(SUniFindUTF8ChrStart(str, 8) == 7);
        CHECK(SUniFindUTF8ChrStart(str, 9) == 7);
    }
}

TEST_CASE("SUniSGetUTF8", "[unicode]") {
    SECTION("returns ascii-range utf-8 first character") {
        auto string = "foobar";
        int32_t chars = 0;
        auto code = SUniSGetUTF8(reinterpret_cast<const uint8_t*>(string), &chars);

        REQUIRE(code == 'f');
        REQUIRE(chars == 1);
    }

    SECTION("returns non-ascii-range utf-8 first character") {
        auto string = "\xF0\x9F\x99\x82"
                      "foobar";
        int32_t chars = 0;
        auto code = SUniSGetUTF8(reinterpret_cast<const uint8_t*>(string), &chars);

        REQUIRE(code == 0x1F642);
        REQUIRE(chars == 4);
    }

    SECTION("returns null first character") {
        auto string = "";
        int32_t chars = 0;
        auto code = SUniSGetUTF8(reinterpret_cast<const uint8_t*>(string), &chars);

        REQUIRE(code == ~0u);
        REQUIRE(chars == 0);
    }
}

TEST_CASE("SUniSPutUTF8", "[unicode]") {
    SECTION("writes ascii-range utf-8 first character") {
        auto code = 'f';
        char buffer[RESULT_STR_SIZE] = { 0 };
        SUniSPutUTF8(code, buffer);

        REQUIRE(SStrLen(buffer) == 1);
        REQUIRE(!SStrCmp(buffer, "f", SStrLen(buffer)));
    }

    SECTION("writes non-ascii-range utf-8 first character") {
        auto code = 0x1F642;
        char buffer[RESULT_STR_SIZE] = { 0 };
        SUniSPutUTF8(code, buffer);

        REQUIRE(SStrLen(buffer) == 4);
        REQUIRE(!SStrCmp(buffer, "\xF0\x9F\x99\x82", SStrLen(buffer)));
    }

    SECTION("writes null first character") {
        auto code = '\0';
        char buffer[RESULT_STR_SIZE] = { 0 };
        SUniSPutUTF8(code, buffer);

        REQUIRE(SStrLen(buffer) == 0);
    }
}
