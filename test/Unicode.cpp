#include "storm/Unicode.hpp"
#include "test/Test.hpp"

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

        REQUIRE(code == -1u);
        REQUIRE(chars == 0);
    }
}
