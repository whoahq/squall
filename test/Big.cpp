#include "storm/Big.hpp"
#include "test/Test.hpp"
#include <string>

TEST_CASE("SBigAdd", "[big]") {
    SECTION("adds 0 and 1") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 0);

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 1);

        SBigAdd(a, b, c);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);

        SBigDel(a);
        SBigDel(b);
        SBigDel(c);
    }

    SECTION("adds 1 and 2") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 1);

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 2);

        SBigAdd(a, b, c);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 3);

        SBigDel(a);
        SBigDel(b);
        SBigDel(c);
    }

    SECTION("adds 0x12345678 and 0x23456789") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 0x12345678);

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 0x23456789);

        SBigAdd(a, b, c);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 0x3579BE01);

        SBigDel(a);
        SBigDel(b);
        SBigDel(c);
    }

    SECTION("adds 0xFFFFFFFF and 0xF0F0F0F0") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 0xFFFFFFFF);

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 0xF0F0F0F0);

        SBigAdd(a, b, c);

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0xF0F0F0EF);
        CHECK(a->Primary()[1] == 0x1);

        SBigDel(a);
        SBigDel(b);
        SBigDel(c);
    }
}

TEST_CASE("SBigAnd", "[big]") {
    BigDataTest a;
    BigDataTest b;
    BigDataTest c;

    SECTION("overwrites output") {
        SBigFromUnsigned(a, 123456);
        SBigFromUnsigned(b, 0);
        SBigFromUnsigned(c, 0);

        SBigAnd(a, b, c);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 0);
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

        CHECK(a->Primary().Count() == 4);
        CHECK(a->Primary()[0] == 0x04000000);
        CHECK(a->Primary()[1] == 0x00060004);
        CHECK(a->Primary()[2] == 0x041F0200);
        CHECK(a->Primary()[3] == 0x00000600);
    }
}

TEST_CASE("SBigBitLen", "[big]") {
    SECTION("returns bit length of 1") {
        BigData* num;
        SBigNew(&num);
        SBigFromUnsigned(num, 1);

        uint32_t len;
        SBigBitLen(num, &len);

        CHECK(len == 1);

        SBigDel(num);
    }

    SECTION("returns bit length of 5") {
        BigData* num;
        SBigNew(&num);
        SBigFromUnsigned(num, 5);

        uint32_t len;
        SBigBitLen(num, &len);

        CHECK(len == 3);

        SBigDel(num);
    }

    SECTION("returns bit length of 0xFFFF") {
        BigData* num;
        SBigNew(&num);
        SBigFromUnsigned(num, 0xFFFF);

        uint32_t len;
        SBigBitLen(num, &len);

        CHECK(len == 16);

        SBigDel(num);
    }

    SECTION("returns bit length of 0xFFFFFFFF") {
        BigData* num;
        SBigNew(&num);
        SBigFromUnsigned(num, 0xFFFFFFFF);

        uint32_t len;
        SBigBitLen(num, &len);

        CHECK(len == 32);

        SBigDel(num);
    }

    SECTION("returns bit length of 0x22222222AAAAAAAA") {
        BigData* num;
        SBigNew(&num);
        uint64_t num_ = 0x22222222AAAAAAAA;
        SBigFromBinary(num, reinterpret_cast<uint8_t*>(&num_), sizeof(num_));

        uint32_t len;
        SBigBitLen(num, &len);

        CHECK(len == 62);

        SBigDel(num);
    }
}

TEST_CASE("SBigCompare", "[big]") {
    SECTION("compares 10 and 1") {
        BigData* a;
        SBigNew(&a);
        SBigFromUnsigned(a, 10);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 1);

        CHECK(SBigCompare(a, b) == 1);

        SBigDel(a);
        SBigDel(b);
    }
}

TEST_CASE("SBigCopy", "[big]") {
    BigDataTest a, b;

    SECTION("copies data") {
        uint8_t num[] = { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 };
        SBigFromBinary(a, num, sizeof(num));
        SBigFromUnsigned(b, 42);

        CHECK(a->Primary().Count() == 4);

        SBigCopy(a, b);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 42);
    }
}

TEST_CASE("SBigDec", "[big]") {
    BigDataTest a, b;

    SECTION("decrements value by 1") {
        SBigFromUnsigned(b, 5);

        SBigDec(a, b);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 4);
    }

    SECTION("decrements from 0") {
        SBigFromUnsigned(b, 0);

        SBigDec(a, b);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 0xFFFFFFFF);
    }
}

TEST_CASE("SBigDiv", "[big]") {
    BigDataTest a, b, c;

    SECTION("divides 2 by 1") {
        SBigFromUnsigned(b, 2);
        SBigFromUnsigned(c, 1);

        SBigDiv(a, b, c);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 2);
    }

    SECTION("divides 5 by 2") {
        SBigFromUnsigned(b, 5);
        SBigFromUnsigned(c, 2);

        SBigDiv(a, b, c);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 2);
    }

    SECTION("divides 7 by 4") {
        SBigFromUnsigned(b, 7);
        SBigFromUnsigned(c, 4);

        SBigDiv(a, b, c);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
    }

    SECTION("divides 0x9999444488885555 by 0x2222") {
        SBigFromStr(b, std::to_string(0x9999444488885555ULL).c_str());
        SBigFromUnsigned(c, 0x2222);

        SBigDiv(a, b, c);

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0x00040002);
        CHECK(a->Primary()[1] == 0x48002);
    }

    SECTION("divides 0x9999444488885555 by 0xFFFFFFFF") {
        SBigFromStr(b, std::to_string(0x9999444488885555ULL).c_str());
        SBigFromUnsigned(c, 0xFFFFFFFF);

        SBigDiv(a, b, c);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 0x99994445);
    }

    SECTION("divides 0x9999444488885555 by 0x1111222233334444 (buffer divisor)") {
        SBigFromStr(b, std::to_string(0x9999444488885555ULL).c_str());
        SBigFromStr(c, std::to_string(0x1111222233334444ULL).c_str());

        SBigDiv(a, b, c);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 8);
    }
}

TEST_CASE("SBigFromBinary", "[big]") {
    SECTION("creates bigdata from 0") {
        BigData* num;
        SBigNew(&num);
        uint32_t data = 0;
        SBigFromBinary(num, reinterpret_cast<uint8_t*>(&data), sizeof(data));

        CHECK(num->Primary().Count() == 1);
        CHECK(num->Primary()[0] == 0);

        SBigDel(num);
    }

    SECTION("creates bigdata from 0x123456789ABCDEF0") {
        BigData* num;
        SBigNew(&num);
        uint64_t data = 0x123456789ABCDEF0;
        SBigFromBinary(num, reinterpret_cast<uint8_t*>(&data), sizeof(data));

        CHECK(num->Primary().Count() == 2);
        CHECK(num->Primary()[0] == 0x9ABCDEF0);
        CHECK(num->Primary()[1] == 0x12345678);

        SBigDel(num);
    }
}

TEST_CASE("SBigFromStr", "[big]") {
    BigDataTest num;

    SECTION("with empty string") {
        SBigFromStr(num, "");

        CHECK(num->Primary().Count() == 0);
    }

    SECTION("with string containing numbers") {
        SBigFromStr(num, "123456");

        CHECK(num->Primary().Count() == 1);
        CHECK(num->Primary()[0] == 123456);
    }

    SECTION("with string containing letters (original bug)") {
        SBigFromStr(num, "ABC");

        const unsigned int expected_num = ('A' - '0') * 100 + ('B' - '0') * 10 + ('C' - '0');

        CHECK(num->Primary().Count() == 1);
        CHECK(num->Primary()[0] == expected_num);
    }
}

TEST_CASE("SBigFromUnsigned", "[big]") {
    SECTION("creates bigdata from 0") {
        BigData* num;
        SBigNew(&num);
        SBigFromUnsigned(num, 0);

        CHECK(num->Primary().Count() == 1);
        CHECK(num->Primary()[0] == 0);

        SBigDel(num);
    }

    SECTION("creates bigdata from 0x12345678") {
        BigData* num;
        SBigNew(&num);
        SBigFromUnsigned(num, 0x12345678);

        CHECK(num->Primary().Count() == 1);
        CHECK(num->Primary()[0] == 0x12345678);

        SBigDel(num);
    }
}
TEST_CASE("SBigInc", "[big]") {
    BigDataTest a, b;

    SECTION("increments from 0") {
        SBigFromUnsigned(b, 0);

        SBigInc(a, b);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
    }

    SECTION("increments from max uint") {
        SBigFromUnsigned(b, UINT32_MAX);

        SBigInc(a, b);

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0);
        CHECK(a->Primary()[1] == 1);
    }

    SECTION("increments from a number") {
        SBigFromUnsigned(b, 1337);

        SBigInc(a, b);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1338);
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
    SECTION("mods 7 by 4") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 7);

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 4);

        SBigMod(a, b, c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 3);

        SBigDel(a);
        SBigDel(b);
        SBigDel(c);
    }

    SECTION("mods 7 by 4 then mods 9 by 5") {
        BigData* a;
        SBigNew(&a);

        BigData* b1;
        SBigNew(&b1);
        SBigFromUnsigned(b1, 7);

        BigData* c1;
        SBigNew(&c1);
        SBigFromUnsigned(c1, 4);

        BigData* b2;
        SBigNew(&b2);
        SBigFromUnsigned(b2, 9);

        BigData* c2;
        SBigNew(&c2);
        SBigFromUnsigned(c2, 5);

        SBigMod(a, b1, c1);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 3);

        SBigMod(a, b2, c2);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 4);

        SBigDel(a);
        SBigDel(b1);
        SBigDel(c1);
        SBigDel(b2);
        SBigDel(c2);
    }

    SECTION("mods 0x9999444488885555 by 0xFFFFFFFF") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        uint64_t b_ = 0x9999444488885555;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&b_), sizeof(b_));

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 0xFFFFFFFF);

        SBigMod(a, b, c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 0x2221999A);

        SBigDel(a);
        SBigDel(b);
        SBigDel(c);
    }
}

TEST_CASE("SBigMul", "[big]") {
    SECTION("multiplies 0 and 1") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 0);

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 1);

        SBigMul(a, b, c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 0);

        SBigDel(a);
        SBigDel(b);
        SBigDel(c);
    }

    SECTION("multiplies 2 and 4") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 2);

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 4);

        SBigMul(a, b, c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 8);

        SBigDel(a);
        SBigDel(b);
        SBigDel(c);
    }

    SECTION("multiplies 0xFFFFFFFF and 0x100") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 0xFFFFFFFF);

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 0x100);

        SBigMul(a, b, c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0xFFFFFF00);
        CHECK(a->Primary()[1] == 0xFF);

        SBigDel(a);
        SBigDel(b);
        SBigDel(c);
    }

    SECTION("multiplies 0xFFFFFF and 0x11223344") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 0xFFFFFF);

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 0x11223344);

        SBigMul(a, b, c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0x32DDCCBC);
        CHECK(a->Primary()[1] == 0x112233);

        SBigDel(a);
        SBigDel(b);
        SBigDel(c);
    }
}

TEST_CASE("SBigPowMod", "[big]") {
    SECTION("takes 256 to the 8th power and mods the result by 999") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 256);

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 8);

        BigData* d;
        SBigNew(&d);
        SBigFromUnsigned(d, 999);

        SBigPowMod(a, b, c, d);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 160);

        SBigDel(a);
        SBigDel(b);
        SBigDel(c);
        SBigDel(d);
    }
}

TEST_CASE("SBigNot", "[big]") {
    BigDataTest a, b;

    SECTION("bitwise negates small values") {
        auto v = GENERATE(uint32_t(1), 2, 10, 0xFFFFFFFF);

        SBigFromUnsigned(b, v);
        SBigNot(a, b);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == ~v);
    }

    SECTION("bitwise negates large values") {
        auto v = GENERATE(10000000000000ULL, 0xFF00000000000000ULL);

        SBigFromStr(b, std::to_string(v).c_str());
        SBigNot(a, b);

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == ~uint32_t(v));
        CHECK(a->Primary()[1] == ~uint32_t(v >> 32));
    }

    SECTION("bitwise negates huge value") {
        uint32_t data[] = { 0xF00DFEED, 0xBA1D, 0xBEEBBEEB, 0x12345678, 0x9ABCDEF, 0xDEADCAD, 0xD011A };
        
        SBigFromBinary(b, data, sizeof(data));
        SBigNot(a, b);

        CHECK(a->Primary().Count() == 7);
        CHECK(a->Primary()[0] == ~data[0]);
        CHECK(a->Primary()[1] == ~data[1]);
        CHECK(a->Primary()[2] == ~data[2]);
        CHECK(a->Primary()[3] == ~data[3]);
        CHECK(a->Primary()[4] == ~data[4]);
        CHECK(a->Primary()[5] == ~data[5]);
        CHECK(a->Primary()[6] == ~data[6]);
    }
}

TEST_CASE("SBigShl", "[big]") {
    SECTION("shifts 256 left 7 bits") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 256);

        SBigShl(a, b, 7);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 32768);

        SBigDel(a);
        SBigDel(b);
    }
}

TEST_CASE("SBigShr", "[big]") {
    SECTION("shifts 256 right 7 bits") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 256);

        SBigShr(a, b, 7);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 2);

        SBigDel(a);
        SBigDel(b);
    }
}

TEST_CASE("SBigSquare", "[big]") {
    SECTION("squares 0xFFFFFFFF") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 0xFFFFFFFF);

        SBigSquare(a, b);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0x1);
        CHECK(a->Primary()[1] == 0xFFFFFFFE);

        SBigDel(a);
        SBigDel(b);
    }
}

TEST_CASE("SBigSub", "[big]") {
    BigDataTest a, b, c;

    SECTION("subtracts 1 from 2") {
        SBigFromUnsigned(b, 2);
        SBigFromUnsigned(c, 1);

        SBigSub(a, b, c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
    }

    SECTION("subtracts 1 from 0") {
        SBigFromUnsigned(b, 0);
        SBigFromUnsigned(c, 1);

        SBigSub(a, b, c);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 0xFFFFFFFF);
    }
}

TEST_CASE("SBigToBinaryBuffer", "[big]") {
    SECTION("returns expected buffer for bigdata representing 0") {
        BigData* num;
        SBigNew(&num);
        SBigFromUnsigned(num, 0);

        uint8_t buffer[4];
        uint32_t bytes;
        SBigToBinaryBuffer(num, buffer, sizeof(buffer), &bytes);

        REQUIRE(bytes == 0);

        SBigDel(num);
    }

    SECTION("returns expected buffer for bigdata representing 0x12345678") {
        BigData* num;
        SBigNew(&num);
        SBigFromUnsigned(num, 0x12345678);

        uint8_t buffer[4];
        uint32_t bytes;
        SBigToBinaryBuffer(num, buffer, sizeof(buffer), &bytes);

        CHECK(bytes == 4);
        CHECK(*reinterpret_cast<uint32_t*>(buffer) == 0x12345678);

        SBigDel(num);
    }

    SECTION("returns expected buffer for bigdata representing 0x123456789ABCDEF0") {
        BigData* num;
        SBigNew(&num);
        uint64_t data = 0x123456789ABCDEF0;
        SBigFromBinary(num, reinterpret_cast<uint8_t*>(&data), sizeof(data));

        uint8_t buffer[8];
        uint32_t bytes;
        SBigToBinaryBuffer(num, buffer, sizeof(buffer), &bytes);

        CHECK(bytes == 8);
        CHECK(*reinterpret_cast<uint64_t*>(buffer) == 0x123456789ABCDEF0);

        SBigDel(num);
    }
}
