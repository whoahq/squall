#include "storm/Unicode.hpp"
#include "storm/String.hpp"
#include "test/Test.hpp"

TEST_CASE("SUniConvertUTF16ToDos", "[unicode]") {
    SECTION("does nothing on empty dest size") {
        auto result = SUniConvertUTF16ToDos(nullptr, nullptr, 0);
        CHECK(result == 0);
    }

    SECTION("operates on empty string") {
        char resultStr[100];
        const wchar_t str[] = L"";

        auto result = SUniConvertUTF16ToDos(resultStr, str, sizeof(resultStr));
        CHECK(std::string(resultStr) == "");
        CHECK(result == 1); // wrote 1 null terminator
    }

    // TODO
    // test failure to write null terminator when not enough space
    // test writing ???? when character unsupported
    // test one-to-one ascii
    // test conversions
}

TEST_CASE("SUniConvertUTF16ToMac", "[unicode]") {
    SECTION("does nothing on empty dest size") {
        auto result = SUniConvertUTF16ToMac(nullptr, nullptr, 0);
        CHECK(result == 0);
    }

    SECTION("operates on empty string") {
        char resultStr[100];
        const wchar_t str[] = L"";

        auto result = SUniConvertUTF16ToMac(resultStr, str, sizeof(resultStr));
        CHECK(std::string(resultStr) == "");
        CHECK(result == 1); // wrote 1 null terminator
    }

    // TODO
}

TEST_CASE("SUniConvertUTF16ToWin", "[unicode]") {
    SECTION("does nothing on empty dest size") {
        auto result = SUniConvertUTF16ToWin(nullptr, nullptr, 0);
        CHECK(result == 0);
    }

    SECTION("operates on empty string") {
        char resultStr[100];
        const wchar_t str[] = L"";

        auto result = SUniConvertUTF16ToWin(resultStr, str, sizeof(resultStr));
        CHECK(std::string(resultStr) == "");
        CHECK(result == 1); // wrote 1 null terminator
    }

    // TODO
    // Starcraft flavour Korean codepage
}

TEST_CASE("SUniConvertDosToUTF16", "[unicode]") {
    SECTION("does nothing on empty dest size") {
        auto result = SUniConvertDosToUTF16(nullptr, nullptr, 0);
        CHECK(result == 0);
    }

    SECTION("operates on empty string") {
        wchar_t resultStr[100];
        const char str[] = "";

        auto result = SUniConvertDosToUTF16(resultStr, str, sizeof(resultStr));
        CHECK(std::wstring(resultStr) == L"");
        CHECK(result == 1); // wrote 1 null terminator
    }

    // TODO
    // test failure to write null terminator when not enough space
    // test one-to-one ascii
    // test conversions
}

TEST_CASE("SUniConvertMacToUTF16", "[unicode]") {
    SECTION("does nothing on empty dest size") {
        auto result = SUniConvertMacToUTF16(nullptr, nullptr, 0);
        CHECK(result == 0);
    }

    SECTION("operates on empty string") {
        wchar_t resultStr[100];
        const char str[] = "";

        auto result = SUniConvertMacToUTF16(resultStr, str, sizeof(resultStr));
        CHECK(std::wstring(resultStr) == L"");
        CHECK(result == 1); // wrote 1 null terminator
    }

    // TODO
}

TEST_CASE("SUniConvertWinToUTF16", "[unicode]") {
    SECTION("does nothing on empty dest size") {
        auto result = SUniConvertWinToUTF16(nullptr, nullptr, 0);
        CHECK(result == 0);
    }

    SECTION("operates on empty string") {
        wchar_t resultStr[100];
        const char str[] = "";

        auto result = SUniConvertWinToUTF16(resultStr, str, sizeof(resultStr));
        CHECK(std::wstring(resultStr) == L"");
        CHECK(result == 1); // wrote 1 null terminator
    }

    // TODO
    // Starcraft flavour Korean codepage
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
        char buffer[100] = { 0 };
        SUniSPutUTF8(code, buffer);

        REQUIRE(SStrLen(buffer) == 1);
        REQUIRE(!SStrCmp(buffer, "f", SStrLen(buffer)));
    }

    SECTION("writes non-ascii-range utf-8 first character") {
        auto code = 0x1F642;
        char buffer[100] = { 0 };
        SUniSPutUTF8(code, buffer);

        REQUIRE(SStrLen(buffer) == 4);
        REQUIRE(!SStrCmp(buffer, "\xF0\x9F\x99\x82", SStrLen(buffer)));
    }

    SECTION("writes null first character") {
        auto code = '\0';
        char buffer[100] = { 0 };
        SUniSPutUTF8(code, buffer);

        REQUIRE(SStrLen(buffer) == 0);
    }
}
