#include "storm/String.hpp"
#include "storm/Memory.hpp"
#include "test/Test.hpp"

TEST_CASE("SStrChr", "[string]") {
    auto string = "foobar";

    SECTION("finds first character when it exists at start of string") {
        auto result = SStrChr(string, 'f');
        REQUIRE(result == string);
    }

    SECTION("finds first character when it exists in middle of string") {
        auto result = SStrChr(string, 'b');
        REQUIRE(result == string + 3);
    }

    SECTION("finds first character when it exists at end of string") {
        auto result = SStrChr(string, 'r');
        REQUIRE(result == string + 5);
    }

    SECTION("returns nullptr when character does not exist in string") {
        auto result = SStrChr(string, 'z');
        REQUIRE(result == nullptr);
    }

    SECTION("returns nullptr when string is empty") {
        auto result = SStrChr("", 'z');
        REQUIRE(result == nullptr);
    }

    SECTION("returns nullptr when character is 0") {
        auto result = SStrChr(string, '\0');
        REQUIRE(result == nullptr);
    }
}

TEST_CASE("SStrChrR", "[string]") {
    auto string = "ffoobbaarr";

    SECTION("finds last character when it exists at start of string") {
        auto result = SStrChrR(string, 'f');
        REQUIRE(result == string + 1);
    }

    SECTION("finds last character when it exists in middle of string") {
        auto result = SStrChrR(string, 'b');
        REQUIRE(result == string + 5);
    }

    SECTION("finds last character when it exists at end of string") {
        auto result = SStrChrR(string, 'r');
        REQUIRE(result == string + 9);
    }

    SECTION("finds last character when it exists at start and end of string") {
        auto string = "ffoobbaarrff";
        auto result = SStrChrR(string, 'f');
        REQUIRE(result == string + 11);
    }

    SECTION("returns nullptr when character does not exist in string") {
        auto result = SStrChrR(string, 'z');
        REQUIRE(result == nullptr);
    }

    SECTION("returns nullptr when string is empty") {
        auto result = SStrChrR("", 'z');
        REQUIRE(result == nullptr);
    }

    SECTION("returns nullptr when character is 0") {
        auto result = SStrChrR(string, '\0');
        REQUIRE(result == nullptr);
    }
}

TEST_CASE("SStrCmp", "[string]") {
    SECTION("compares two strings that exactly match correctly") {
        auto compare = SStrCmp("foo", "foo");
        REQUIRE(compare == 0);
    }

    SECTION("compares two strings that partially match correctly") {
        auto compare1 = SStrCmp("bar", "foobar");
        auto compare2 = SStrCmp("foobar", "bar");
        REQUIRE(compare1 < 0);
        REQUIRE(compare2 > 0);
    }

    SECTION("compares two strings that do not match correctly") {
        auto compare = SStrCmp("bar", "xyzzy");
        REQUIRE(compare < 0);
    }

    SECTION("only compares up to maxchars") {
        auto compare = SStrCmp("donkeykong", "donkeypinnochio", 6);
        REQUIRE(compare == 0);
    }

    SECTION("compare is case sensitive") {
        auto compare = SStrCmp("dingdong", "dInGdOnG");
        REQUIRE(compare != 0);
    }
}

TEST_CASE("SStrCmpI", "[string]") {
    SECTION("compares two strings that exactly match correctly") {
        auto compare = SStrCmpI("foo", "foo");
        REQUIRE(compare == 0);
    }

    SECTION("compares two strings that match with differing case correctly") {
        auto compare = SStrCmpI("foo", "FoO");
        REQUIRE(compare == 0);
    }

    SECTION("compares two strings that partially match correctly") {
        auto compare1 = SStrCmpI("bar", "foobar");
        auto compare2 = SStrCmpI("foobar", "bar");
        REQUIRE(compare1 < 0);
        REQUIRE(compare2 > 0);
    }

    SECTION("compares two strings that do not match correctly") {
        auto compare = SStrCmpI("bar", "xyzzy");
        REQUIRE(compare < 0);
    }

    SECTION("only compares up to maxchars") {
        auto compare = SStrCmpI("donkeykong", "donkeypinnochio", 6);
        REQUIRE(compare == 0);
    }
}

TEST_CASE("SStrDupA", "[string]") {
    SECTION("duplicates string correctly") {
        auto string1 = "foo bar";
        auto string2 = SStrDupA(string1, __FILE__, __LINE__);
        auto compare = SStrCmp(string1, string2);
        auto newPtr = string1 != string2;
        SMemFree(string2);

        REQUIRE(compare == 0);
        REQUIRE(newPtr == true);
    }

    SECTION("duplicates empty string correctly") {
        auto string1 = "";
        auto string2 = SStrDupA(string1, __FILE__, __LINE__);
        auto compare = SStrCmp(string1, string2);
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
        REQUIRE(SStrLen("foo") == 3);
    }

    SECTION("calculates empty string length correctly") {
        REQUIRE(SStrLen("") == 0);
    }
}

TEST_CASE("SStrLower", "[string]") {
    SECTION("rewrites uppercase string to lowercase correctly") {
        char string[] = "FOOBAR";
        SStrLower(string);

        REQUIRE(!SStrCmp(string, "foobar"));
    }

    SECTION("rewrites lowercase string to lowercase correctly") {
        char string[] = "foobar";
        SStrLower(string);

        REQUIRE(!SStrCmp(string, "foobar"));
    }

    SECTION("does nothing on empty string") {
        char string[] = "";
        SStrLower(string);

        REQUIRE(!SStrCmp(string, ""));
    }
}

TEST_CASE("SStrPack", "[string]") {
    SECTION("truncates dest correctly when first byte in source is null") {
        char dest[10] = {};
        auto source = "\0foobar";
        auto length = SStrPack(dest, source, sizeof(dest));

        REQUIRE(length == 0);
        REQUIRE(!SStrCmp(dest, ""));
    }

    SECTION("truncates dest correctly when middle byte in source is null") {
        char dest[10] = {};
        auto source = "foo\0bar";
        auto length = SStrPack(dest, source, sizeof(dest));

        REQUIRE(length == 3);
        REQUIRE(!SStrCmp(dest, "foo"));
    }

    SECTION("does not truncate dest when source has no early null byte") {
        char dest[10] = {};
        auto source = "foobar";
        auto length = SStrPack(dest, source, sizeof(dest));

        REQUIRE(length == 6);
        REQUIRE(!SStrCmp(dest, "foobar"));
    }

    SECTION("appends to target string") {
        char dest[10] = "who";
        auto length = SStrPack(dest, "dis", sizeof(dest));

        REQUIRE(length == 6);
        REQUIRE(!SStrCmp(dest, "whodis"));
    }

    SECTION("truncates when dest size is not large enough") {
        char dest[5] = "who";
        auto length = SStrPack(dest, "dis", sizeof(dest));

        REQUIRE(length == 4);
        REQUIRE(!SStrCmp(dest, "whod"));
    }
}

TEST_CASE("SStrPrintf", "[string]") {
    SECTION("fills dest with formatted string") {
        char dest[100] = {};
        auto length = SStrPrintf(dest, sizeof(dest), "%s - %s", "foo", "bar");

        REQUIRE(length == 9);
        REQUIRE(!SStrCmp(dest, "foo - bar"));
    }

    SECTION("fills dest with int") {
        char dest[100] = {};
        auto length = SStrPrintf(dest, sizeof(dest), "%d", 69);

        REQUIRE(length == 2);
        REQUIRE(!SStrCmp(dest, "69"));
    }

    SECTION("fills dest with empty string") {
        char dest[100] = {};
        auto length = SStrPrintf(dest, sizeof(dest), "");

        REQUIRE(length == 0);
        REQUIRE(!SStrCmp(dest, ""));
    }

    SECTION("truncates when dest size is not large enough") {
        char dest[4] = {};
        auto length = SStrPrintf(dest, sizeof(dest), "%s", "wowzers");

        REQUIRE(length == 3);
        REQUIRE(!SStrCmp(dest, "wow"));
    }

}

TEST_CASE("SStrStr", "[string]") {
    auto string = "foobar";

    SECTION("finds substring when it exists at end of string") {
        auto search = "bar";
        auto substring = SStrStr(string, search);
        REQUIRE(substring == string + 3);
    }

    SECTION("finds substring when it exists at start of string") {
        auto search = "foo";
        auto substring = SStrStr(string, search);
        REQUIRE(substring == string);
    }

    SECTION("finds substring when search is empty") {
        auto substring = SStrStr(string, "");
        REQUIRE(substring == string);
    }

    SECTION("returns nullptr when search does not exist in string") {
        auto substring = SStrStr(string, "xyzzy");
        REQUIRE(substring == nullptr);
    }

    SECTION("returns nullptr when given empty string") {
        auto substring = SStrStr("", "bar");
        REQUIRE(substring == nullptr);
    }
}

TEST_CASE("SStrTokenize", "[string]") {
    SECTION("finds all tokens in comma-delimited string") {
        auto string = "foo,bar,baz";
        char buffer[100] = {};
        const char* tokens[] = { "foo", "bar", "baz" };

        for (auto& token : tokens) {
            SStrTokenize(&string, buffer, sizeof(buffer), " ,", nullptr);
            REQUIRE(!SStrCmp(buffer, token));
        }
    }

    SECTION("finds all tokens in comma-and-whitespace-delimited string") {
        auto string = "foo , bar , baz";
        char buffer[100] = {};
        const char* tokens[] = { "foo", "bar", "baz" };

        for (auto& token : tokens) {
            SStrTokenize(&string, buffer, sizeof(buffer), " ,", nullptr);
            REQUIRE(!SStrCmp(buffer, token));
        }
    }

    SECTION("finds no tokens empty string") {
        auto string = "";
        char buffer[100] = {};
        SStrTokenize(&string, buffer, sizeof(buffer), " ,", nullptr);
        REQUIRE(!SStrCmp(buffer, ""));
    }

    SECTION("identifies quoted tokens") {
        auto string = "foo bar \"baz bazinga\" bodonkers \"donga dongs\"";
        char buffer[100] = {};
        std::pair<const char*, int> tokens[] = {
            std::make_pair("foo", 0),
            std::make_pair("bar", 0),
            std::make_pair("baz bazinga", 1),
            std::make_pair("bodonkers", 0),
            std::make_pair("donga dongs", 1)
        };

        for (auto& token : tokens) {
            int quoted = 0;
            SStrTokenize(&string, buffer, sizeof(buffer), " \"", &quoted);
            std::string result = buffer;
            REQUIRE(result == token.first);
            REQUIRE(quoted == token.second);
        }
    }

    SECTION("doesn't identify quoted tokens if excluded from whitespace") {
        auto string = "foo bar \"baz bazinga\" bodonkers \"donga dongs\"";
        char buffer[100] = {};
        std::pair<const char*, int> tokens[] = {
            std::make_pair("foo", 0),
            std::make_pair("bar", 0),
            std::make_pair("\"baz", 0),
            std::make_pair("bazinga\"", 0),
            std::make_pair("bodonkers", 0),
            std::make_pair("\"donga", 0),
            std::make_pair("dongs\"", 0)
        };

        for (auto& token : tokens) {
            int quoted = 0;
            SStrTokenize(&string, buffer, sizeof(buffer), " ", &quoted);
            std::string result = buffer;
            REQUIRE(result == token.first);
            REQUIRE(quoted == token.second);
        }
    }
}

TEST_CASE("SStrToFloat", "[string]") {
    SECTION("converts empty string to float") {
        auto result = SStrToFloat("");
        REQUIRE(result == 0.0f);
    }

    SECTION("converts whitespace string to float") {
        auto result = SStrToFloat(" \t\r\n");
        REQUIRE(result == 0.0f);
    }

    SECTION("converts random characters to float") {
        auto result = SStrToFloat("abcd");
        REQUIRE(result == 0.0f);
    }

    SECTION("converts string with positive int to float") {
        auto result = SStrToFloat("123");
        REQUIRE(result == 123.0f);
    }

    SECTION("converts string with negative int to float") {
        auto result = SStrToFloat("-123");
        REQUIRE(result == -123.0f);
    }

    SECTION("converts string with positive float to float") {
        auto result = SStrToFloat("1.5");
        REQUIRE(result == 1.5f);
    }

    SECTION("converts string with negative float to float") {
        auto result = SStrToFloat("-1.5");
        REQUIRE(result == -1.5f);
    }

    SECTION("converts string with float and positive exponent to float") {
        auto result = SStrToFloat("1.5e3");
        REQUIRE(result == 1500.0f);
    }

    SECTION("converts string with float and positive full form exponent to float") {
        auto result = SStrToFloat("1.5e+3");
        REQUIRE(result == 1500.0f);
    }

    SECTION("converts string with capital exponent to float") {
        auto result = SStrToFloat("1.5E+3");
        REQUIRE(result == 1500.0f);
    }

    SECTION("converts string with random added letter to float") {
        auto result = SStrToFloat("1.5g3");
        REQUIRE(result == 1.5f);
    }

    SECTION("converts string with float and negative exponent to float") {
        auto result = SStrToFloat("1500.0e-3");
        REQUIRE(result == 1.5f);
    }

    SECTION("converts string with zero to float") {
        auto result = SStrToFloat("0");
        REQUIRE(result == 0.0f);
    }

    SECTION("converts string with leading zero to float") {
        auto result = SStrToFloat("01");
        REQUIRE(result == 1.0f);
    }
}

TEST_CASE("SStrToInt", "[string]") {
    SECTION("converts empty string to int") {
        auto result = SStrToInt("");
        REQUIRE(result == 0);
    }

    SECTION("converts whitespace string to int") {
        auto result = SStrToInt(" ");
        REQUIRE(result == 0);
    }

    SECTION("converts random characters to int") {
        auto result = SStrToInt("abcd");
        REQUIRE(result == 0);
    }

    SECTION("converts string with positive number to int") {
        auto result = SStrToInt("123");
        REQUIRE(result == 123);
    }

    SECTION("converts string with negative number to int") {
        auto result = SStrToInt("-123");
        REQUIRE(result == -123);
    }

    SECTION("converts string with zero to int") {
        auto result = SStrToInt("0");
        REQUIRE(result == 0);
    }

    SECTION("converts string with leading zero to int") {
        auto result = SStrToInt("01");
        REQUIRE(result == 1);
    }

    SECTION("converts string with two whitespace-separated numbers to int") {
        auto result = SStrToInt("123 456");
        REQUIRE(result == 123);
    }
}

TEST_CASE("SStrUpper", "[string]") {
    SECTION("rewrites lowercase string to uppercase correctly") {
        char string[] = "foobar";
        SStrUpper(string);

        REQUIRE(!SStrCmp(string, "FOOBAR"));
    }

    SECTION("rewrites uppercase string to uppercase correctly") {
        char string[] = "FOOBAR";
        SStrUpper(string);

        REQUIRE(!SStrCmp(string, "FOOBAR"));
    }

    SECTION("does nothing on empty string") {
        char string[] = "";
        SStrUpper(string);

        REQUIRE(!SStrCmp(string, ""));
    }
}
