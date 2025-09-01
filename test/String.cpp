#include "storm/String.hpp"
#include "storm/Memory.hpp"
#include "test/Test.hpp"

#include <cfloat>
#include <cstdarg>
#include <type_traits>


TEST_CASE("SStrChr const", "[string]") {
    const char* string = "foobar";

    static_assert(std::is_same<decltype(SStrChr(string, 'f')), const char*>::value, "Expect result to be const char*");

    SECTION("finds first character when it exists at start of string") {
        const char* result = SStrChr(string, 'f');
        REQUIRE(result == string);
    }

    SECTION("finds first character when it exists in middle of string") {
        const char* result = SStrChr(string, 'b');
        REQUIRE(result == string + 3);
    }

    SECTION("finds first character when it exists at end of string") {
        const char* result = SStrChr(string, 'r');
        REQUIRE(result == string + 5);
    }

    SECTION("returns nullptr when character does not exist in string") {
        const char* result = SStrChr(string, 'z');
        REQUIRE(result == nullptr);
    }

    SECTION("returns nullptr when string is empty") {
        const char* result = SStrChr("", 'z');
        REQUIRE(result == nullptr);
    }

    SECTION("returns nullptr when character is 0") {
        const char* result = SStrChr(string, '\0');
        REQUIRE(result == nullptr);
    }
}

TEST_CASE("SStrChr", "[string]") {
    char string[] = "foobar";

    static_assert(std::is_same<decltype(SStrChr(string, 'f')), char*>::value, "Expect result to be char*");

    SECTION("finds first character when it exists at start of string") {
        char* result = SStrChr(string, 'f');
        REQUIRE(result == string);
    }

    SECTION("finds first character when it exists in middle of string") {
        char* result = SStrChr(string, 'b');
        REQUIRE(result == string + 3);
    }

    SECTION("finds first character when it exists at end of string") {
        char* result = SStrChr(string, 'r');
        REQUIRE(result == string + 5);
    }

    SECTION("returns nullptr when character does not exist in string") {
        char* result = SStrChr(string, 'z');
        REQUIRE(result == nullptr);
    }

    SECTION("returns nullptr when string is empty") {
        char string[] = "";
        char* result = SStrChr(string, 'z');
        REQUIRE(result == nullptr);
    }

    SECTION("returns nullptr when character is 0") {
        char* result = SStrChr(string, '\0');
        REQUIRE(result == nullptr);
    }
}

TEST_CASE("SStrChrR const", "[string]") {
    const char* string = "ffoobbaarr";

    static_assert(std::is_same<decltype(SStrChrR(string, 'f')), const char*>::value, "Expect result to be const char*");

    SECTION("finds last character when it exists at start of string") {
        const char* result = SStrChrR(string, 'f');
        REQUIRE(result == string + 1);
    }

    SECTION("finds last character when it exists in middle of string") {
        const char* result = SStrChrR(string, 'b');
        REQUIRE(result == string + 5);
    }

    SECTION("finds last character when it exists at end of string") {
        const char* result = SStrChrR(string, 'r');
        REQUIRE(result == string + 9);
    }

    SECTION("finds last character when it exists at start and end of string") {
        const char* string = "ffoobbaarrff";
        const char* result = SStrChrR(string, 'f');
        REQUIRE(result == string + 11);
    }

    SECTION("returns nullptr when character does not exist in string") {
        const char* result = SStrChrR(string, 'z');
        REQUIRE(result == nullptr);
    }

    SECTION("returns nullptr when string is empty") {
        const char* result = SStrChrR("", 'z');
        REQUIRE(result == nullptr);
    }

    SECTION("returns nullptr when character is 0") {
        const char* result = SStrChrR(string, '\0');
        REQUIRE(result == nullptr);
    }
}

TEST_CASE("SStrChrR", "[string]") {
    char string[] = "ffoobbaarr";

    static_assert(std::is_same<decltype(SStrChrR(string, 'f')), char*>::value, "Expect result to be char*");

    SECTION("finds last character when it exists at start of string") {
        char* result = SStrChrR(string, 'f');
        REQUIRE(result == string + 1);
    }

    SECTION("finds last character when it exists in middle of string") {
        char* result = SStrChrR(string, 'b');
        REQUIRE(result == string + 5);
    }

    SECTION("finds last character when it exists at end of string") {
        char* result = SStrChrR(string, 'r');
        REQUIRE(result == string + 9);
    }

    SECTION("finds last character when it exists at start and end of string") {
        char string[] = "ffoobbaarrff";
        char* result = SStrChrR(string, 'f');
        REQUIRE(result == string + 11);
    }

    SECTION("returns nullptr when character does not exist in string") {
        char* result = SStrChrR(string, 'z');
        REQUIRE(result == nullptr);
    }

    SECTION("returns nullptr when string is empty") {
        char string[] = "";
        char* result = SStrChrR(string, 'z');
        REQUIRE(result == nullptr);
    }

    SECTION("returns nullptr when character is 0") {
        char* result = SStrChrR(string, '\0');
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

struct TestHash {
    const char *str;
    uint32_t hash;
};

TEST_CASE("SStrHash", "[string]") {
    SECTION("hashes strings with case insensitivity") {
        // Results obtained by directly calling Starcraft 1.17's SStrHash
        auto testcase = GENERATE(
            TestHash{ "bloop bloop", 0x3EB42E62 },
            TestHash{ "BLOOP bloop", 0x3EB42E62 },
            TestHash{ "Objects\\CameraHelper\\CameraHelper.mdx", 0xE99A51F5 },
            TestHash{ "Objects/CameraHelper/CameraHelper.mdx", 0xE99A51F5 },
            TestHash{ "abcdefghijklmnopqrstuvwxyz", 0x29564240 },
            TestHash{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0x29564240 },
            TestHash{ "123456789~!@#$%^&*()_+`-=[]{}|;':\",.<>?", 0x54E31141 },
            TestHash{ "/", 0x57EC56C7 },
            TestHash{ "\\", 0x57EC56C7 },
            TestHash{ "\r\n\t ", 0x89351E43 }
        );

        auto hash = SStrHash(testcase.str);
        CHECK(hash == testcase.hash);
    }

    SECTION("hashes strings with case sensitivity") {
        // Results obtained by directly calling Starcraft 1.17's SStrHash
        auto testcase = GENERATE(
            TestHash{ "bloop bloop", 0x4E7D519E },
            TestHash{ "BLOOP bloop", 0x307459CC },
            TestHash{ "Objects\\CameraHelper\\CameraHelper.mdx", 0x5202C0C5 },
            TestHash{ "Objects/CameraHelper/CameraHelper.mdx", 0x58BD40A9 },
            TestHash{ "abcdefghijklmnopqrstuvwxyz", 0xEF065306 },
            TestHash{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0x29564240 },
            TestHash{ "123456789~!@#$%^&*()_+`-=[]{}|;':\",.<>?", 0x54E31141 },
            TestHash{ "/", 0x93912757 },
            TestHash{ "\\", 0x57EC56C7 },
            TestHash{ "\r\n\t ", 0x89351E43 }
        );

        auto hash = SStrHash(testcase.str, SSTR_HASH_CASESENSITIVE);
        CHECK(hash == testcase.hash);
    }

    SECTION("hashes strings with fixed seeds case insensitive") {
        // Results obtained by directly calling Starcraft 1.17's SStrHash
        auto testcase = GENERATE(
            TestHash{ "bloop bloop", 0xDD85B006 },
            TestHash{ "BLOOP bloop", 0xDD85B006 },
            TestHash{ "Objects\\CameraHelper\\CameraHelper.mdx", 0x0EA7F863 },
            TestHash{ "Objects/CameraHelper/CameraHelper.mdx", 0x0EA7F863 },
            TestHash{ "abcdefghijklmnopqrstuvwxyz", 0x4BAB46BC },
            TestHash{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0x4BAB46BC },
            TestHash{ "123456789~!@#$%^&*()_+`-=[]{}|;':\",.<>?", 0x2A52D7D3 },
            TestHash{ "/", 0xD7DED775 },
            TestHash{ "\\", 0xD7DED775 },
            TestHash{ "\r\n\t ", 0xC0038449 }
        );

        auto hash = SStrHash(testcase.str, 0, 123);
        CHECK(hash == testcase.hash);
    }

    SECTION("hashes strings with fixed seeds case sensitive") {
        // Results obtained by directly calling Starcraft 1.17's SStrHash
        auto testcase = GENERATE(
            TestHash{ "bloop bloop", 0x96187662 },
            TestHash{ "BLOOP bloop", 0xC421E888 },
            TestHash{ "Objects\\CameraHelper\\CameraHelper.mdx", 0x625F6763 },
            TestHash{ "Objects/CameraHelper/CameraHelper.mdx", 0xC2A7DFE7 },
            TestHash{ "abcdefghijklmnopqrstuvwxyz", 0x4018029E },
            TestHash{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0x4BAB46BC },
            TestHash{ "123456789~!@#$%^&*()_+`-=[]{}|;':\",.<>?", 0x2A52D7D3 },
            TestHash{ "/", 0x13A3A6E5 },
            TestHash{ "\\", 0xD7DED775 },
            TestHash{ "\r\n\t ", 0xC0038449 }
        );

        auto hash = SStrHash(testcase.str, SSTR_HASH_CASESENSITIVE, 123);
        CHECK(hash == testcase.hash);
    }

    SECTION("hashing empty string with seed of 0 gives default seed") {
        auto hash = SStrHash("", 0, 0);
        CHECK(hash == 0x7FED7FED);

        hash = SStrHash("", SSTR_HASH_CASESENSITIVE, 0);
        CHECK(hash == 0x7FED7FED);
    }

    SECTION("hashing empty string with custom seed returns that seed") {
        auto seed = GENERATE(1, 123, 0x7FED7FED, 0xFFFFFFFF);

        auto hash = SStrHash("", 0, seed);
        CHECK(hash == seed);

        hash = SStrHash("", SSTR_HASH_CASESENSITIVE, seed);
        CHECK(hash == seed);
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

    SECTION("does nothing if maxchars is 0") {
        char dest[10] = {};
        auto length = SStrPrintf(dest, 0, "%d", 69);

        REQUIRE(length == 0);
        REQUIRE(!SStrCmp(dest, ""));
    }
}

static size_t CallSStrVPrintf(char* dest, size_t maxchars, const char* format, ...) {
    va_list va;
    va_start(va, format);
    auto length = SStrVPrintf(dest, maxchars, format, va);
    va_end(va);
    return length;
}

TEST_CASE("SStrVPrintf", "[string]") {
    SECTION("fills dest with formatted string") {
        char dest[100] = {};
        auto length = CallSStrVPrintf(dest, sizeof(dest), "%s - %s", "foo", "bar");

        REQUIRE(length == 9);
        REQUIRE(!SStrCmp(dest, "foo - bar"));
    }

    SECTION("fills dest with int") {
        char dest[100] = {};
        auto length = CallSStrVPrintf(dest, sizeof(dest), "%d", 69);

        REQUIRE(length == 2);
        REQUIRE(!SStrCmp(dest, "69"));
    }

    SECTION("fills dest with empty string") {
        char dest[100] = {};
        auto length = CallSStrVPrintf(dest, sizeof(dest), "");

        REQUIRE(length == 0);
        REQUIRE(!SStrCmp(dest, ""));
    }

    SECTION("truncates when dest size is not large enough") {
        char dest[4] = {};
        auto length = CallSStrVPrintf(dest, sizeof(dest), "%s", "wowzers");

        REQUIRE(length == 3);
        REQUIRE(!SStrCmp(dest, "wow"));
    }

    SECTION("does nothing if maxchars is 0") {
        char dest[10] = {};
        auto length = CallSStrVPrintf(dest, 0, "%d", 69);

        REQUIRE(length == 0);
        REQUIRE(!SStrCmp(dest, ""));
    }
}

TEST_CASE("SStrStr", "[string]") {
    char string[] = "foobar";

    static_assert(std::is_same<decltype(SStrStr(string, "")), char*>::value, "Expect result to be char*");

    SECTION("is case sensitive") {
        char* substring = SStrStr(string, "OOBA");
        REQUIRE(substring == nullptr);
    }

    SECTION("finds substring when it exists at end of string") {
        char* substring = SStrStr(string, "bar");
        REQUIRE(substring == string + 3);
    }

    SECTION("finds substring when it exists at start of string") {
        char* substring = SStrStr(string, "foo");
        REQUIRE(substring == string);
    }

    SECTION("finds substring when search is empty") {
        char* substring = SStrStr(string, "");
        REQUIRE(substring == string);
    }

    SECTION("returns nullptr when search does not exist in string") {
        char* substring = SStrStr(string, "xyzzy");
        REQUIRE(substring == nullptr);
    }

    SECTION("returns nullptr when given empty string") {
        char string[] = "";
        char* substring = SStrStr(string, "bar");
        REQUIRE(substring == nullptr);
    }
}

TEST_CASE("SStrStr const", "[string]") {
    const char* string = "foobar";

    static_assert(std::is_same<decltype(SStrStr(string, "")), const char*>::value, "Expect result to be const char*");

    SECTION("is case sensitive") {
        const char* substring = SStrStr(string, "OOBA");
        REQUIRE(substring == nullptr);
    }

    SECTION("finds substring when it exists at end of string") {
        const char* substring = SStrStr(string, "bar");
        REQUIRE(substring == string + 3);
    }

    SECTION("finds substring when it exists at start of string") {
        const char* substring = SStrStr(string, "foo");
        REQUIRE(substring == string);
    }

    SECTION("finds substring when search is empty") {
        const char* substring = SStrStr(string, "");
        REQUIRE(substring == string);
    }

    SECTION("returns nullptr when search does not exist in string") {
        const char* substring = SStrStr(string, "xyzzy");
        REQUIRE(substring == nullptr);
    }

    SECTION("returns nullptr when given empty string") {
        const char* substring = SStrStr("", "bar");
        REQUIRE(substring == nullptr);
    }
}

TEST_CASE("SStrStrI", "[string]") {
    char string[] = "foobar";

    static_assert(std::is_same<decltype(SStrStrI(string, "")), char*>::value, "Expect result to be char*");

    SECTION("is case insensitive") {
        char* substring = SStrStrI(string, "OOBA");
        REQUIRE(substring == string + 1);
    }

    SECTION("finds substring when it exists at end of string") {
        char* substring = SStrStrI(string, "bar");
        REQUIRE(substring == string + 3);
    }

    SECTION("finds substring when it exists at start of string") {
        char* substring = SStrStrI(string, "foo");
        REQUIRE(substring == string);
    }

    SECTION("finds substring when search is empty") {
        char* substring = SStrStrI(string, "");
        REQUIRE(substring == string);
    }

    SECTION("returns nullptr when search does not exist in string") {
        char* substring = SStrStrI(string, "xyzzy");
        REQUIRE(substring == nullptr);
    }

    SECTION("returns nullptr when given empty string") {
        char string[] = "";
        char* substring = SStrStrI(string, "bar");
        REQUIRE(substring == nullptr);
    }
}

TEST_CASE("SStrStrI const", "[string]") {
    const char* string = "foobar";

    static_assert(std::is_same<decltype(SStrStrI(string, "")), const char*>::value, "Expect result to be const char*");

    SECTION("is case insensitive") {
        const char* substring = SStrStrI(string, "OOBA");
        REQUIRE(substring == string + 1);
    }

    SECTION("finds substring when it exists at end of string") {
        const char* substring = SStrStrI(string, "bar");
        REQUIRE(substring == string + 3);
    }

    SECTION("finds substring when it exists at start of string") {
        const char* substring = SStrStrI(string, "foo");
        REQUIRE(substring == string);
    }

    SECTION("finds substring when search is empty") {
        const char* substring = SStrStrI(string, "");
        REQUIRE(substring == string);
    }

    SECTION("returns nullptr when search does not exist in string") {
        const char* substring = SStrStrI(string, "xyzzy");
        REQUIRE(substring == nullptr);
    }

    SECTION("returns nullptr when given empty string") {
        const char* substring = SStrStrI("", "bar");
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

TEST_CASE("SStrToDouble", "[string]") {
    SECTION("converts empty string to double") {
        auto result = SStrToDouble("");
        REQUIRE(result == 0.0f);
    }

    SECTION("converts whitespace string to double") {
        auto result = SStrToDouble(" \t\r\n");
        REQUIRE(result == 0.0f);
    }

    SECTION("converts random characters to double") {
        auto result = SStrToDouble("abcd");
        REQUIRE(result == 0.0f);
    }

    SECTION("converts string with positive int to double") {
        auto result = SStrToDouble("123");
        REQUIRE(result == 123.0f);
    }

    SECTION("converts string with negative int to double") {
        auto result = SStrToDouble("-123");
        REQUIRE(result == -123.0f);
    }

    SECTION("converts string with positive float to double") {
        auto result = SStrToDouble("1.5");
        REQUIRE(result == 1.5f);
    }

    SECTION("converts string with negative float to double") {
        auto result = SStrToDouble("-1.5");
        REQUIRE(result == -1.5f);
    }

    SECTION("converts string with float and positive exponent to double") {
        auto result = SStrToDouble("1.5e3");
        REQUIRE(result == 1500.0f);
    }

    SECTION("converts string with float and positive full form exponent to double") {
        auto result = SStrToDouble("1.5e+3");
        REQUIRE(result == 1500.0f);
    }

    SECTION("converts string with capital exponent to double") {
        auto result = SStrToDouble("1.5E+3");
        REQUIRE(result == 1500.0f);
    }

    SECTION("converts string with random added letter to double") {
        auto result = SStrToDouble("1.5g3");
        REQUIRE(result == 1.5f);
    }

    SECTION("converts string with float and negative exponent to double") {
        auto result = SStrToDouble("1500.0e-3");
        REQUIRE(result == 1.5f);
    }

    SECTION("converts string with zero to double") {
        auto result = SStrToDouble("0");
        REQUIRE(result == 0.0f);
    }

    SECTION("converts string with leading zero to double") {
        auto result = SStrToDouble("01");
        REQUIRE(result == 1.0f);
    }

    SECTION("converts large values to infinity") {
        // beyond max double
        REQUIRE(SStrToDouble("1.79769e+310") == HUGE_VAL);
        REQUIRE(SStrToDouble("-1.79769e+310") == -HUGE_VAL);
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

    SECTION("converts large values to infinity") {
        // max double
        REQUIRE(SStrToFloat("1.79769e+308") == HUGE_VALF);

        // beyond max double
        REQUIRE(SStrToFloat("1.79769e+310") == HUGE_VALF);
        REQUIRE(SStrToFloat("-1.79769e+310") == -HUGE_VALF);
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

TEST_CASE("SStrToUnsigned", "[string]") {
    SECTION("converts empty string to int") {
        auto result = SStrToUnsigned("");
        REQUIRE(result == 0);
    }

    SECTION("converts whitespace string to int") {
        auto result = SStrToUnsigned(" ");
        REQUIRE(result == 0);
    }

    SECTION("converts random characters to int") {
        auto result = SStrToUnsigned("abcd");
        REQUIRE(result == 0);
    }

    SECTION("converts string with positive number to int") {
        auto result = SStrToUnsigned("123");
        REQUIRE(result == 123);
    }

    SECTION("returns 0 if number is negative") {
        auto result = SStrToUnsigned("-123");
        REQUIRE(result == 0);
    }

    SECTION("converts string with zero to int") {
        auto result = SStrToUnsigned("0");
        REQUIRE(result == 0);
    }

    SECTION("converts string with leading zero to int") {
        auto result = SStrToUnsigned("01");
        REQUIRE(result == 1);
    }

    SECTION("converts string with two whitespace-separated numbers to int") {
        auto result = SStrToUnsigned("123 456");
        REQUIRE(result == 123);
    }

    SECTION("converts max unsigned int") {
        auto result = SStrToUnsigned("4294967295");
        REQUIRE(result == 4294967295u);
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
