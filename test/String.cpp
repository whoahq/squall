#include "storm/String.hpp"
#include "storm/Memory.hpp"
#include "test/Test.hpp"

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
