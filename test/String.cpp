#include "storm/String.hpp"
#include "storm/Memory.hpp"
#include "test/Test.hpp"

TEST_CASE("SStrChr", "[string]") {
    SECTION("finds first character when it exists at start of string") {
        auto string = "foobar";
        auto search = 'f';
        auto result = SStrChr(string, search);
        REQUIRE(result == string);
    }

    SECTION("finds first character when it exists in middle of string") {
        auto string = "foobar";
        auto search = 'b';
        auto result = SStrChr(string, search);
        REQUIRE(result == string + 3);
    }

    SECTION("finds first character when it exists at end of string") {
        auto string = "foobar";
        auto search = 'r';
        auto result = SStrChr(string, search);
        REQUIRE(result == string + 5);
    }

    SECTION("returns nullptr when character does not exist in string") {
        auto string = "foobar";
        auto search = 'z';
        auto result = SStrChr(string, search);
        REQUIRE(result == nullptr);
    }

    SECTION("returns nullptr when string is empty") {
        auto string = "";
        auto search = 'z';
        auto result = SStrChr(string, search);
        REQUIRE(result == nullptr);
    }
}

TEST_CASE("SStrChrR", "[string]") {
    SECTION("finds last character when it exists at start of string") {
        auto string = "ffoobbaarr";
        auto search = 'f';
        auto result = SStrChrR(string, search);
        REQUIRE(result == string + 1);
    }

    SECTION("finds last character when it exists in middle of string") {
        auto string = "ffoobbaarr";
        auto search = 'b';
        auto result = SStrChrR(string, search);
        REQUIRE(result == string + 5);
    }

    SECTION("finds last character when it exists at end of string") {
        auto string = "ffoobbaarr";
        auto search = 'r';
        auto result = SStrChrR(string, search);
        REQUIRE(result == string + 9);
    }

    SECTION("finds last character when it exists at start and end of string") {
        auto string = "ffoobbaarrff";
        auto search = 'f';
        auto result = SStrChrR(string, search);
        REQUIRE(result == string + 11);
    }

    SECTION("returns nullptr when character does not exist in string") {
        auto string = "ffoobbaarr";
        auto search = 'z';
        auto result = SStrChrR(string, search);
        REQUIRE(result == nullptr);
    }

    SECTION("returns nullptr when string is empty") {
        auto string = "";
        auto search = 'z';
        auto result = SStrChrR(string, search);
        REQUIRE(result == nullptr);
    }
}

TEST_CASE("SStrCmp", "[string]") {
    SECTION("compares two strings that exactly match correctly") {
        auto compare = SStrCmp("foo", "foo", STORM_MAX_STR);
        REQUIRE(compare == 0);
    }

    SECTION("compares two strings that partially match correctly") {
        auto compare1 = SStrCmp("bar", "foobar", STORM_MAX_STR);
        auto compare2 = SStrCmp("foobar", "bar", STORM_MAX_STR);
        REQUIRE(compare1 < 0);
        REQUIRE(compare2 > 0);
    }

    SECTION("compares two strings that do not match correctly") {
        auto compare = SStrCmp("bar", "xyzzy", STORM_MAX_STR);
        REQUIRE(compare < 0);
    }
}

TEST_CASE("SStrCmpI", "[string]") {
    SECTION("compares two strings that exactly match correctly") {
        auto compare = SStrCmpI("foo", "foo", STORM_MAX_STR);
        REQUIRE(compare == 0);
    }

    SECTION("compares two strings that match with differing case correctly") {
        auto compare = SStrCmpI("foo", "foO", STORM_MAX_STR);
        REQUIRE(compare == 0);
    }

    SECTION("compares two strings that do not match correctly") {
        auto compare = SStrCmpI("bar", "xyzzy", STORM_MAX_STR);
        REQUIRE(compare < 0);
    }
}

TEST_CASE("SStrDupA", "[string]") {
    SECTION("duplicates string correctly") {
        auto string1 = "foo bar";
        auto string2 = SStrDupA(string1, __FILE__, __LINE__);
        auto compare = SStrCmp(string1, string2, STORM_MAX_STR);
        auto newPtr = string1 != string2;
        SMemFree(string2);

        REQUIRE(compare == 0);
        REQUIRE(newPtr == true);
    }
}

TEST_CASE("SStrHashHT", "[string]") {
    SECTION("hashes simple string correctly") {
        auto hash = SStrHashHT("foo");
        REQUIRE(hash == 1371562358u);
    }

    SECTION("hashes string with forward slash correctly") {
        auto hash = SStrHashHT("foo/bar");
        REQUIRE(hash == 2270424393u);
    }

    SECTION("hashes string with forward slash equivalent to back slash") {
        auto hashForwardSlash = SStrHashHT("foo/bar");
        auto hashBackSlash = SStrHashHT("foo\\bar");
        REQUIRE(hashForwardSlash == hashBackSlash);
    }
}

TEST_CASE("SStrLen", "[string]") {
    SECTION("calculates string length correctly") {
        auto length = SStrLen("foo");
        REQUIRE(length == 3);
    }
}

TEST_CASE("SStrLower", "[string]") {
    SECTION("rewrites uppercase string to lowercase correctly") {
        auto upper = "FOOBAR";
        char* lower = static_cast<char*>(SMemAlloc(SStrLen(upper) + 1, __FILE__, __LINE__, 0x0));
        SStrCopy(lower, upper, STORM_MAX_STR);
        SStrLower(lower);
        auto compare = SStrCmp(lower, "foobar", SStrLen(lower));
        SMemFree(lower);

        REQUIRE(!compare);
    }

    SECTION("rewrites lowercase string to lowercase correctly") {
        auto lower1 = "foobar";
        char* lower2 = static_cast<char*>(SMemAlloc(SStrLen(lower1) + 1, __FILE__, __LINE__, 0x0));
        SStrCopy(lower2, lower1, STORM_MAX_STR);
        SStrLower(lower2);
        auto compare = SStrCmp(lower2, "foobar", SStrLen(lower2));
        SMemFree(lower2);

        REQUIRE(!compare);
    }
}

TEST_CASE("SStrPack", "[string]") {
    SECTION("truncates dest correctly when first byte in source is null") {
        char dest[10] = { 0 };
        auto source = "\0foobar";
        auto length = SStrPack(dest, source, 10);
        REQUIRE(length == 0);
        REQUIRE(!SStrCmp(dest, "", SStrLen("")));
    }

    SECTION("truncates dest correctly when middle byte in source is null") {
        char dest[10] = { 0 };
        auto source = "foo\0bar";
        auto length = SStrPack(dest, source, 10);
        REQUIRE(length == 3);
        REQUIRE(!SStrCmp(dest, "foo", SStrLen("foo")));
    }

    SECTION("does not truncate dest when source has no early null byte") {
        char dest[10] = { 0 };
        auto source = "foobar";
        auto length = SStrPack(dest, source, 10);
        REQUIRE(length == 6);
        REQUIRE(!SStrCmp(dest, "foobar", SStrLen("foobar")));
    }
}

TEST_CASE("SStrPrintf", "[string]") {
    SECTION("fills dest with formatted string") {
        char dest[100] = { 0 };
        auto format = "%s - %s";
        auto length = SStrPrintf(dest, 100, format, "foo", "bar");
        REQUIRE(length == 9);
        REQUIRE(!SStrCmp(dest, "foo - bar", SStrLen("foo - bar")));
    }

    SECTION("fills dest with empty string") {
        char dest[100] = { 0 };
        auto format = "";
        auto length = SStrPrintf(dest, 100, format);
        REQUIRE(length == 0);
        REQUIRE(!SStrCmp(dest, "", SStrLen("")));
    }
}

TEST_CASE("SStrStr", "[string]") {
    SECTION("finds substring when it exists at end of string") {
        auto string = "foobar";
        auto search = "bar";
        auto substring = SStrStr(string, search);
        REQUIRE(!SStrCmp(search, substring, SStrLen(search)));
    }

    SECTION("finds substring when it exists at start of string") {
        auto string = "foobar";
        auto search = "foo";
        auto substring = SStrStr(string, search);
        REQUIRE(!SStrCmp(search, substring, SStrLen(search)));
    }

    SECTION("finds substring when search is empty") {
        auto string = "foobar";
        auto search = "";
        auto substring = SStrStr(string, search);
        REQUIRE(!SStrCmp(string, substring, SStrLen(string)));
    }

    SECTION("returns nullptr when search does not exist in string") {
        auto string = "foobar";
        auto search = "xyzzy";
        auto substring = SStrStr(string, search);
        REQUIRE(substring == nullptr);
    }

    SECTION("returns nullptr when given empty string") {
        auto string = "";
        auto search = "bar";
        auto substring = SStrStr(string, search);
        REQUIRE(substring == nullptr);
    }
}

TEST_CASE("SStrTokenize", "[string]") {
    SECTION("finds all tokens in comma-delimited string") {
        auto string = "foo,bar,baz";
        char buffer[100] = { 0 };
        const char* tokens[] = { "foo", "bar", "baz" };

        for (auto& token : tokens) {
            SStrTokenize(&string, buffer, 1000, " ,", nullptr);
            REQUIRE(!SStrCmp(buffer, token, STORM_MAX_STR));
        }
    }

    SECTION("finds all tokens in comma-and-whitespace-delimited string") {
        auto string = "foo , bar , baz";
        char buffer[100] = { 0 };
        const char* tokens[] = { "foo", "bar", "baz" };

        for (auto& token : tokens) {
            SStrTokenize(&string, buffer, 1000, " ,", nullptr);
            REQUIRE(!SStrCmp(buffer, token, STORM_MAX_STR));
        }
    }

    SECTION("finds no tokens empty string") {
        auto string = "";
        char buffer[100] = { 0 };
        SStrTokenize(&string, buffer, 1000, " ,", nullptr);
        REQUIRE(!SStrCmp(buffer, "", STORM_MAX_STR));
    }
}

TEST_CASE("SStrToFloat", "[string]") {
    SECTION("converts empty string to float") {
        auto string = "";
        auto result = SStrToFloat(string);
        REQUIRE(result == 0.0f);
    }

    SECTION("converts whitespace string to float") {
        auto string = " ";
        auto result = SStrToFloat(string);
        REQUIRE(result == 0.0f);
    }

    SECTION("converts string with positive int to float") {
        auto string = "123";
        auto result = SStrToFloat(string);
        REQUIRE(result == 123.0f);
    }

    SECTION("converts string with negative int to float") {
        auto string = "-123";
        auto result = SStrToFloat(string);
        REQUIRE(result == -123.0f);
    }

    SECTION("converts string with positive float to float") {
        auto string = "1.5";
        auto result = SStrToFloat(string);
        REQUIRE(result == 1.5f);
    }

    SECTION("converts string with negative float to float") {
        auto string = "-1.5";
        auto result = SStrToFloat(string);
        REQUIRE(result == -1.5f);
    }

    SECTION("converts string with float and positive exponent to float") {
        auto string = "1.5e3";
        auto result = SStrToFloat(string);
        REQUIRE(result == 1500.0f);
    }

    SECTION("converts string with float and negative exponent to float") {
        auto string = "1500.0e-3";
        auto result = SStrToFloat(string);
        REQUIRE(result == 1.5f);
    }

    SECTION("converts string with zero to float") {
        auto string = "0";
        auto result = SStrToFloat(string);
        REQUIRE(result == 0.0f);
    }

    SECTION("converts string with leading zero to float") {
        auto string = "01";
        auto result = SStrToFloat(string);
        REQUIRE(result == 1.0f);
    }
}

TEST_CASE("SStrToInt", "[string]") {
    SECTION("converts empty string to int") {
        auto string = "";
        auto result = SStrToInt(string);
        REQUIRE(result == 0);
    }

    SECTION("converts whitespace string to int") {
        auto string = " ";
        auto result = SStrToInt(string);
        REQUIRE(result == 0);
    }

    SECTION("converts string with positive number to int") {
        auto string = "123";
        auto result = SStrToInt(string);
        REQUIRE(result == 123);
    }

    SECTION("converts string with negative number to int") {
        auto string = "-123";
        auto result = SStrToInt(string);
        REQUIRE(result == -123);
    }

    SECTION("converts string with zero to int") {
        auto string = "0";
        auto result = SStrToInt(string);
        REQUIRE(result == 0);
    }

    SECTION("converts string with leading zero to int") {
        auto string = "01";
        auto result = SStrToInt(string);
        REQUIRE(result == 1);
    }

    SECTION("converts string with two whitespace-separated numbers to int") {
        auto string = "123 456";
        auto result = SStrToInt(string);
        REQUIRE(result == 123);
    }
}
