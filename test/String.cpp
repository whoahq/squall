#include "storm/String.hpp"
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
