#include "storm/Crypto.hpp"
#include "test/Test.hpp"
#include <cstring>

TEST_CASE("SARC4ProcessBuffer", "[crypto]") {
    SECTION("correctly processes given non-zero input for given non-zero key") {
        uint8_t key[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };

        SARC4Key rc4Key;
        SARC4PrepareKey(key, sizeof(key), &rc4Key);

        uint8_t input[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };
        uint8_t processed[] = { 0x75, 0xB7, 0x87, 0x80, 0x99, 0xE0, 0xC5, 0x96 };

        SARC4ProcessBuffer(input, sizeof(input), &rc4Key, &rc4Key);

        REQUIRE(!memcmp(input, processed, sizeof(input)));
    }

    SECTION("correctly processes given all-zero input for given non-zero key") {
        uint8_t key[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef };

        SARC4Key rc4Key;
        SARC4PrepareKey(key, sizeof(key), &rc4Key);

        uint8_t input[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        uint8_t processed[] = { 0x74, 0x94, 0xC2, 0xE7, 0x10, 0x4B, 0x08, 0x79 };

        SARC4ProcessBuffer(input, sizeof(input), &rc4Key, &rc4Key);

        REQUIRE(!memcmp(input, processed, sizeof(input)));
    }

    SECTION("correctly processes given all-zero input for given all-zero key") {
        uint8_t key[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

        SARC4Key rc4Key;
        SARC4PrepareKey(key, sizeof(key), &rc4Key);

        uint8_t input[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        uint8_t processed[] = { 0xDE, 0x18, 0x89, 0x41, 0xA3, 0x37, 0x5D, 0x3A };

        SARC4ProcessBuffer(input, sizeof(input), &rc4Key, &rc4Key);

        REQUIRE(!memcmp(input, processed, sizeof(input)));
    }
}
