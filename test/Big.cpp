#include "storm/Big.hpp"
#include "test/Test.hpp"

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
    SECTION("subtracts 1 from 2") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 2);

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 1);

        SBigSub(a, b, c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);

        SBigDel(a);
        SBigDel(b);
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
