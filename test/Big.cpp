#include "BigTest.hpp"
#include <string>

TEST_CASE("SBigAdd", "[big]") {
    BigDataTest a, b, c;

    SECTION("adds 0 and 1") {
        SBigFromUnsigned(b, 0);
        SBigFromUnsigned(c, 1);

        SBigAdd(a, b, c);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 1);
    }

    SECTION("adds 1 and 2") {
        SBigFromUnsigned(b, 1);
        SBigFromUnsigned(c, 2);

        SBigAdd(a, b, c);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 3);
    }

    SECTION("adds 0x12345678 and 0x23456789") {
        SBigFromUnsigned(b, 0x12345678);
        SBigFromUnsigned(c, 0x23456789);

        SBigAdd(a, b, c);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 0x3579BE01);
    }

    SECTION("adds 0xFFFFFFFF and 0xF0F0F0F0") {
        SBigFromUnsigned(b, 0xFFFFFFFF);
        SBigFromUnsigned(c, 0xF0F0F0F0);

        SBigAdd(a, b, c);

        const std::vector<uint8_t> expected = {0xEF, 0xF0, 0xF0, 0xF0, 1};
        CHECK(a.ToBinaryBuffer() == expected);
    }
}

TEST_CASE("SBigAnd", "[big]") {
    BigDataTest a, b, c;

    SECTION("overwrites output") {
        SBigFromUnsigned(a, 123456);
        SBigFromUnsigned(b, 0);
        SBigFromUnsigned(c, 0);

        SBigAnd(a, b, c);

        CHECK(SBigIsZero(a));
    }

    SECTION("performs bitwise and on large nums") {
        uint8_t data[] = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 31, 12, 13, 14, 15
        };
        uint8_t data2[] = {
            0, 0, 0, 6, 6, 0, 6, 0, 0, 6, 0xFF, 6, 0, 6, 0
        };
        SBigFromBinary(b, data, sizeof(data));
        SBigFromBinary(c, data2, sizeof(data2));

        SBigAnd(a, b, c);

        const std::vector<uint8_t> expected = {0, 0, 0, 4, 4, 0, 6, 0, 0, 2, 31, 4, 0, 6};
        CHECK(a.ToBinaryBuffer() == expected);
    }
}

TEST_CASE("SBigBitLen", "[big]") {
    BigDataTest num;

    SECTION("returns bit length of 1") {
        SBigFromUnsigned(num, 1);

        uint32_t len;
        SBigBitLen(num, &len);

        CHECK(len == 1);
    }

    SECTION("returns bit length of 5") {
        SBigFromUnsigned(num, 5);

        uint32_t len;
        SBigBitLen(num, &len);

        CHECK(len == 3);
    }

    SECTION("returns bit length of 0xFFFF") {
        SBigFromUnsigned(num, 0xFFFF);

        uint32_t len;
        SBigBitLen(num, &len);

        CHECK(len == 16);
    }

    SECTION("returns bit length of 0xFFFFFFFF") {
        SBigFromUnsigned(num, 0xFFFFFFFF);

        uint32_t len;
        SBigBitLen(num, &len);

        CHECK(len == 32);
    }

    SECTION("returns bit length of 0x22222222AAAAAAAA") {
        uint64_t num_ = 0x22222222AAAAAAAA;
        SBigFromBinary(num, reinterpret_cast<uint8_t*>(&num_), sizeof(num_));

        uint32_t len;
        SBigBitLen(num, &len);

        CHECK(len == 62);
    }
}

TEST_CASE("SBigCompare", "[big]") {
    BigDataTest a, b;

    SECTION("compares 10 and 1") {
        SBigFromUnsigned(a, 10);
        SBigFromUnsigned(b, 1);

        CHECK(SBigCompare(a, b) == 1);
    }
}

TEST_CASE("SBigCopy", "[big]") {
    BigDataTest a, b;

    SECTION("copies data") {
        uint8_t num[] = { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 };
        SBigFromBinary(a, num, sizeof(num));
        SBigFromUnsigned(b, 42);

        CHECK(a.BitLen() == 108);

        SBigCopy(a, b);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 42);
    }
}

TEST_CASE("SBigDec", "[big]") {
    BigDataTest a, b;

    SECTION("decrements value by 1") {
        SBigFromUnsigned(b, 5);

        SBigDec(a, b);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 4);
    }

#ifdef NDEBUG
    SECTION("decrements from 0") {
        SBigFromUnsigned(b, 0);

        SBigDec(a, b);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 0xFFFFFFFF);
    }
#endif
}

TEST_CASE("SBigDiv", "[big]") {
    BigDataTest a, b, c;

    SECTION("divides 2 by 1") {
        SBigFromUnsigned(b, 2);
        SBigFromUnsigned(c, 1);

        SBigDiv(a, b, c);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 2);
    }

    SECTION("divides 5 by 2") {
        SBigFromUnsigned(b, 5);
        SBigFromUnsigned(c, 2);

        SBigDiv(a, b, c);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 2);
    }

    SECTION("divides 7 by 4") {
        SBigFromUnsigned(b, 7);
        SBigFromUnsigned(c, 4);

        SBigDiv(a, b, c);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 1);
    }

    SECTION("divides 0x9999444488885555 by 0x2222") {
        SBigFromStr(b, std::to_string(0x9999444488885555ULL).c_str());
        SBigFromUnsigned(c, 0x2222);

        SBigDiv(a, b, c);

        const std::vector<uint8_t> expected = {2, 0, 4, 0, 2, 0x80, 4};
        CHECK(a.ToBinaryBuffer() == expected);
    }

    SECTION("divides 0x9999444488885555 by 0xFFFFFFFF") {
        SBigFromStr(b, std::to_string(0x9999444488885555ULL).c_str());
        SBigFromUnsigned(c, 0xFFFFFFFF);

        SBigDiv(a, b, c);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 0x99994445);
    }

    SECTION("divides 0x9999444488885555 by 0x1111222233334444 (buffer divisor)") {
        SBigFromStr(b, std::to_string(0x9999444488885555ULL).c_str());
        SBigFromStr(c, std::to_string(0x1111222233334444ULL).c_str());

        SBigDiv(a, b, c);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 8);
    }
}

TEST_CASE("SBigFromBinary", "[big]") {
    BigDataTest num;

    SECTION("creates bigdata from 0") {
        uint32_t data = 0;
        SBigFromBinary(num, reinterpret_cast<uint8_t*>(&data), sizeof(data));

        CHECK(SBigIsZero(num));
    }

    SECTION("creates bigdata from 0x123456789ABCDEF0") {
        uint64_t data = 0x123456789ABCDEF0;
        SBigFromBinary(num, reinterpret_cast<uint8_t*>(&data), sizeof(data));

        const std::vector<uint8_t> expected = {0xF0, 0xDE, 0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12};
        CHECK(num.ToBinaryBuffer() == expected);
    }
}

TEST_CASE("SBigFromStr", "[big]") {
    BigDataTest num;

    SECTION("with empty string") {
        SBigFromStr(num, "");

        CHECK(SBigIsZero(num));
    }

    SECTION("with string containing numbers") {
        SBigFromStr(num, "123456");

        CHECK(num.BitLen() <= 32);
        CHECK(num.ToUnsigned() == 123456);
    }

    SECTION("with string containing letters (original bug)") {
        SBigFromStr(num, "ABC");

        // 1700 + 180 + 19
        const unsigned int expected_num = ('A' - '0') * 100 + ('B' - '0') * 10 + ('C' - '0');
        CHECK(expected_num == 1899);

        CHECK(num.BitLen() <= 32);
        CHECK(num.ToUnsigned() == expected_num);
    }
}

TEST_CASE("SBigFromUnsigned", "[big]") {
    BigDataTest num;

    SECTION("creates bigdata from 0") {
        SBigFromUnsigned(num, 0);

        CHECK(SBigIsZero(num));
    }

    SECTION("creates bigdata from 0x12345678") {
        SBigFromUnsigned(num, 0x12345678);

        CHECK(num.BitLen() <= 32);
        CHECK(num.ToUnsigned() == 0x12345678);
    }

    SECTION("creates bigdata from max int") {
        SBigFromUnsigned(num, UINT32_MAX);

        CHECK(num.BitLen() <= 32);
        CHECK(num.ToUnsigned() == UINT32_MAX);
    }
}
TEST_CASE("SBigInc", "[big]") {
    BigDataTest a, b;

    SECTION("increments from 0") {
        SBigFromUnsigned(b, 0);

        SBigInc(a, b);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 1);
    }

    SECTION("increments from max uint") {
        SBigFromUnsigned(b, UINT32_MAX);

        SBigInc(a, b);

        const std::vector<uint8_t> expected = {0, 0, 0, 0, 1};
        CHECK(a.ToBinaryBuffer() == expected);
    }

    SECTION("increments from a number") {
        SBigFromUnsigned(b, 1337);

        SBigInc(a, b);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 1338);
    }
}

TEST_CASE("SBigIsEven", "[big]") {
    BigDataTest a;

    SECTION("unset zero is even") {
        CHECK(SBigIsEven(a));
    }

    SECTION("numbers are even") {
        auto v = GENERATE(0ULL, 2ULL, 10ULL, 10000ULL, 0xFFFFFFFEULL, 0x9999888877776666ULL);

        SBigFromStr(a, std::to_string(v).c_str());
        CHECK(SBigIsEven(a));
    }

    SECTION("numbers are not even") {
        auto v = GENERATE(1ULL, 3ULL, 37ULL, 999999999ULL, 0xFFFFFFFFFULL, 0x9999888877776667ULL);

        SBigFromStr(a, std::to_string(v).c_str());
        CHECK_FALSE(SBigIsEven(a));
    }
}

TEST_CASE("SBigIsOdd", "[big]") {
    BigDataTest a;

    SECTION("unset zero is not odd") {
        CHECK_FALSE(SBigIsOdd(a));
    }

    SECTION("numbers are not odd") {
        auto v = GENERATE(0ULL, 2ULL, 10ULL, 10000ULL, 0xFFFFFFFEULL, 0x9999888877776666ULL);

        SBigFromStr(a, std::to_string(v).c_str());
        CHECK_FALSE(SBigIsOdd(a));
    }

    SECTION("numbers are odd") {
        auto v = GENERATE(1ULL, 3ULL, 37ULL, 999999999ULL, 0xFFFFFFFFFULL, 0x9999888877776667ULL);

        SBigFromStr(a, std::to_string(v).c_str());
        CHECK(SBigIsOdd(a));
    }
}

TEST_CASE("SBigIsOne", "[big]") {
    BigDataTest a;

    SECTION("1 is 1") {
        SBigFromUnsigned(a, 1);
        CHECK(SBigIsOne(a));
    }

    SECTION("numbers are not 1") {
        auto v = GENERATE(0ULL, 2ULL, 10ULL, 11ULL, 10000000000001ULL, 0xFF00000000000001ULL);

        SBigFromStr(a, std::to_string(v).c_str());
        CHECK_FALSE(SBigIsOne(a));
    }
}

TEST_CASE("SBigIsZero", "[big]") {
    BigDataTest a;

    SECTION("unset is zero") {
        CHECK(SBigIsZero(a));
    }

    SECTION("0 is 0") {
        SBigFromUnsigned(a, 0);
        CHECK(SBigIsZero(a));
    }

    SECTION("numbers are not 0") {
        auto v = GENERATE(1ULL, 2ULL, 10ULL, 0xFFFFFFFFULL, 10000000000000ULL, 0xFF00000000000000ULL);

        SBigFromStr(a, std::to_string(v).c_str());
        CHECK_FALSE(SBigIsZero(a));
    }
}

TEST_CASE("SBigMod", "[big]") {
    BigDataTest a, b, c;
    SECTION("mods 7 by 4") {
        SBigFromUnsigned(b, 7);
        SBigFromUnsigned(c, 4);

        SBigMod(a, b, c);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 3);
    }

    SECTION("mods 7 by 4 then mods 9 by 5") {
        BigDataTest b1, c1, b2, c2;

        SBigFromUnsigned(b1, 7);
        SBigFromUnsigned(c1, 4);
        SBigFromUnsigned(b2, 9);
        SBigFromUnsigned(c2, 5);

        SBigMod(a, b1, c1);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 3);

        SBigMod(a, b2, c2);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 4);
    }

    SECTION("mods 0x9999444488885555 by 0xFFFFFFFF") {
        uint64_t b_ = 0x9999444488885555;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&b_), sizeof(b_));
        SBigFromUnsigned(c, 0xFFFFFFFF);

        SBigMod(a, b, c);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 0x2221999A);
    }
}

TEST_CASE("SBigMul", "[big]") {
    BigDataTest a, b, c;

    SECTION("multiplies 0 and 1") {
        SBigFromUnsigned(b, 0);
        SBigFromUnsigned(c, 1);

        SBigMul(a, b, c);

        CHECK(SBigIsZero(a));
    }

    SECTION("multiplies 2 and 4") {
        SBigFromUnsigned(b, 2);
        SBigFromUnsigned(c, 4);

        SBigMul(a, b, c);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 8);
    }

    SECTION("multiplies 0xFFFFFFFF and 0x100") {
        SBigFromUnsigned(b, 0xFFFFFFFF);
        SBigFromUnsigned(c, 0x100);

        SBigMul(a, b, c);

        const std::vector<uint8_t> expected = {0, 0xFF, 0xFF, 0xFF, 0xFF};
        CHECK(a.ToBinaryBuffer() == expected);
    }

    SECTION("multiplies 0xFFFFFF and 0x11223344") {
        SBigFromUnsigned(b, 0xFFFFFF);
        SBigFromUnsigned(c, 0x11223344);

        SBigMul(a, b, c);

        const std::vector<uint8_t> expected = {0xBC, 0xCC, 0xDD, 0x32, 0x33, 0x22, 0x11};
        CHECK(a.ToBinaryBuffer() == expected);
    }
}

TEST_CASE("SBigPowMod", "[big]") {
    BigDataTest a, b, c, d;
    SECTION("takes 256 to the 8th power and mods the result by 999") {
        SBigFromUnsigned(b, 256);
        SBigFromUnsigned(c, 8);
        SBigFromUnsigned(d, 999);

        SBigPowMod(a, b, c, d);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 160);
    }
}

TEST_CASE("SBigNot", "[big]") {
    BigDataTest a, b;

    SECTION("bitwise negates small values") {
        auto v = GENERATE(uint32_t(1), 2, 10, 0xFFFFFFFF);

        SBigFromUnsigned(b, v);
        SBigNot(a, b);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == ~v);
    }

    SECTION("bitwise negates large values") {
        auto v = GENERATE(
            std::make_pair(10000000000000ULL, std::vector<uint8_t>{ 0xFF, 0x5F, 0x8D, 0xB1, 0xE7, 0xF6, 0xFF, 0xFF }),
            std::make_pair(0xFF00000000000000ULL, std::vector<uint8_t>{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF })
        );

        SBigFromStr(b, std::to_string(v.first).c_str());
        SBigNot(a, b);

        CHECK(a.ToBinaryBuffer() == v.second);
    }

    SECTION("bitwise negates huge value") {
        uint8_t data[] = {
            0xF0, 0x0D, 0xFE, 0xED, 0x00, 0x00, 0xBA, 0x1D,
            0xBE, 0xEB, 0xBE, 0xEB, 0x12, 0x34, 0x56, 0x78,
            0x09, 0xAB, 0xCD, 0xEF, 0x0D, 0xEA, 0xDC, 0xAD,
            0x00, 0x0D, 0x01, 0x1A,
        };
        
        SBigFromBinary(b, data, sizeof(data));
        SBigNot(a, b);

        const std::vector<uint8_t> expected = {
            0x0F, 0xF2, 0x01, 0x12, 0xFF, 0xFF, 0x45, 0xE2,
            0x41, 0x14, 0x41, 0x14, 0xED, 0xCB, 0xA9, 0x87,
            0xF6, 0x54, 0x32, 0x10, 0xF2, 0x15, 0x23, 0x52,
            0xFF, 0xF2, 0xFE, 0xE5,
        };
        CHECK(a.ToBinaryBuffer() == expected);
    }
}

TEST_CASE("SBigOr", "[big]") {
    BigDataTest a, b, c;

    SECTION("performs bitwise or on small numbers") {
        auto v = GENERATE(
            std::make_pair(0UL, 0UL),
            std::make_pair(0UL, 123UL),
            std::make_pair(41689UL, 786740UL)
        );

        SBigFromUnsigned(b, v.first);
        SBigFromUnsigned(c, v.second);
        SBigOr(a, b, c);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == (v.first | v.second));
    }

    SECTION("performs bitwise or on large numbers") {
        auto v = GENERATE(
            std::make_pair(0xFF00FF00FF00FF00ULL, 0xFF00FF00FF00FFULL)
        );

        SBigFromStr(b, std::to_string(v.first).c_str());
        SBigFromStr(c, std::to_string(v.second).c_str());
        SBigOr(a, b, c);

        const std::vector<uint8_t> expected = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        CHECK(a.ToBinaryBuffer() == expected);
    }

    SECTION("performs bitwise or on huge value") {
        uint8_t data[] = {
            0xF0, 0x0D, 0xFE, 0xED, 0xBA, 0x1D, 0xBE, 0xEB,
            0xBE, 0xEB, 0x12, 0x34, 0x56, 0x78, 0x09, 0xAB,
            0xCD, 0xEF, 0x0D, 0xEA, 0xDC, 0xAD, 0x00, 0x0D,
            0x01, 0x1A,
        };
        SBigFromBinary(b, data, sizeof(data));

        uint8_t orwith[] = {
            0x11, 0x11, 0x11, 0x11, 0x12, 0x34, 0x56, 0x78, 0xFF
        };
        SBigFromBinary(c, orwith, sizeof(orwith));

        SBigOr(a, b, c);

        const std::vector<uint8_t> expected = {
            0xF1, 0x1D, 0xFF, 0xFD, 0xBA, 0x3D, 0xFE, 0xFB,
            0xFF, 0xEB, 0x12, 0x34, 0x56, 0x78, 0x09, 0xAB,
            0xCD, 0xEF, 0x0D, 0xEA, 0xDC, 0xAD, 0x00, 0x0D,
            0x01, 0x1A,
        };
        CHECK(a.ToBinaryBuffer() == expected);
    }
}

TEST_CASE("SBigShl", "[big]") {
    BigDataTest a, b;

    SECTION("shifts 256 left 7 bits") {
        SBigFromUnsigned(b, 256);

        SBigShl(a, b, 7);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 32768);
    }
}

TEST_CASE("SBigShr", "[big]") {
    BigDataTest a, b;

    SECTION("shifts 256 right 7 bits") {
        SBigFromUnsigned(b, 256);

        SBigShr(a, b, 7);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 2);
    }
}

TEST_CASE("SBigSquare", "[big]") {
    BigDataTest a, b;

    SECTION("squares 0xFFFFFFFF") {
        SBigFromUnsigned(b, 0xFFFFFFFF);

        SBigSquare(a, b);

        const std::vector<uint8_t> expected = {1, 0, 0, 0, 0xFE, 0xFF, 0xFF, 0xFF};
        CHECK(a.ToBinaryBuffer() == expected);
    }
}

TEST_CASE("SBigSub", "[big]") {
    BigDataTest a, b, c;

    SECTION("subtracts 1 from 2") {
        SBigFromUnsigned(b, 2);
        SBigFromUnsigned(c, 1);

        SBigSub(a, b, c);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 1);
    }

#ifdef NDEBUG
    SECTION("subtracts 1 from 0") {
        SBigFromUnsigned(b, 0);
        SBigFromUnsigned(c, 1);

        SBigSub(a, b, c);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == 0xFFFFFFFF);
    }
#endif
}

TEST_CASE("SBigToBinaryBuffer", "[big]") {
    BigDataTest num;

    SECTION("returns expected buffer for bigdata representing 0") {
        SBigFromUnsigned(num, 0);

        uint8_t buffer[4];
        uint32_t bytes;
        SBigToBinaryBuffer(num, buffer, sizeof(buffer), &bytes);

        REQUIRE(bytes == 0);
    }

    SECTION("returns expected buffer for bigdata representing 0x12345678") {
        SBigFromUnsigned(num, 0x12345678);

        uint8_t buffer[4];
        uint32_t bytes;
        SBigToBinaryBuffer(num, buffer, sizeof(buffer), &bytes);

        CHECK(bytes == 4);
        CHECK(*reinterpret_cast<uint32_t*>(buffer) == 0x12345678);
    }

    SECTION("returns expected buffer for bigdata representing 0x123456789ABCDEF0") {
        uint64_t data = 0x123456789ABCDEF0;
        SBigFromBinary(num, reinterpret_cast<uint8_t*>(&data), sizeof(data));

        uint8_t buffer[8];
        uint32_t bytes;
        SBigToBinaryBuffer(num, buffer, sizeof(buffer), &bytes);

        CHECK(bytes == 8);
        CHECK(*reinterpret_cast<uint64_t*>(buffer) == 0x123456789ABCDEF0);
    }
}

TEST_CASE("SBigToUnsigned", "[big]") {
    BigDataTest num;

    SECTION("converts bignum values to uint") {
        auto v = GENERATE(0UL, 1UL, 1000UL, UINT32_MAX);

        SBigFromUnsigned(num, v);

        uint32_t result;
        SBigToUnsigned(num, &result);

        CHECK(result == v);
    }

    SECTION("truncates large values") {
        SBigFromStr(num, std::to_string(0x123456789ABCDEFULL).c_str());

        uint32_t result;
        SBigToUnsigned(num, &result);

        CHECK(num.BitLen() > 32);
        CHECK(result == 0x89ABCDEF);
    }
}

TEST_CASE("SBigXor", "[big]") {
    BigDataTest a, b, c;

    SECTION("performs bitwise xor on small numbers") {
        auto v = GENERATE(
            std::make_pair(0UL, 0UL),
            std::make_pair(0UL, 123UL),
            std::make_pair(41689UL, 786740UL)
        );

        SBigFromUnsigned(b, v.first);
        SBigFromUnsigned(c, v.second);
        SBigXor(a, b, c);

        CHECK(a.BitLen() <= 32);
        CHECK(a.ToUnsigned() == (v.first ^ v.second));
    }

    SECTION("performs bitwise xor on large number") {
        SBigFromStr(b, std::to_string(0xFF00FF00FF00FF00ULL).c_str());
        SBigFromStr(c, std::to_string(   0xFF00FF00FF00FULL).c_str());
        SBigXor(a, b, c);

        const std::vector<uint8_t> expected = { 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF };
        CHECK(a.ToBinaryBuffer() == expected);
    }

    SECTION("performs bitwise xor on huge value") {
        uint8_t data[] = {
            0xF0, 0x0D, 0xFE, 0xED, 0xBA, 0x1D, 0xBE, 0xEB, 0xBE,
            0xEB, 0x12, 0x34, 0x56, 0x78, 0x09, 0xAB, 0xCD, 0xEF,
            0x0D, 0xEA, 0xDC, 0xAD, 0x00, 0x0D, 0x01, 0x1A,
        };

        SBigFromBinary(b, data, sizeof(data));

        uint8_t xorwith[] = { 0x11, 0x11, 0x11, 0x11, 0x12, 0x34, 0x56, 0x78, 0xFF };
        SBigFromBinary(c, xorwith, sizeof(xorwith));

        SBigXor(a, b, c);

        const std::vector<uint8_t> expected = {
            0xE1, 0x1C, 0xEF, 0xFC, 0xA8, 0x29, 0xE8, 0x93, 0x41,
            0xEB, 0x12, 0x34, 0x56, 0x78, 0x09, 0xAB, 0xCD, 0xEF,
            0x0D, 0xEA, 0xDC, 0xAD, 0x00, 0x0D, 0x01, 0x1A,
        };
        CHECK(a.ToBinaryBuffer() == expected);
    }
}
