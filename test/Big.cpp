#include "storm/Big.hpp"
#include "storm/big/Ops.hpp"
#include "test/Test.hpp"

TEST_CASE("Add", "[big]") {
    SECTION("adds 0 and 1") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 0);

        uint64_t c = 1;

        Add(a->Primary(), b->Primary(), c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);

        SBigDel(a);
        SBigDel(b);
    }

    SECTION("adds 2 and 4") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 2);

        uint64_t c = 4;

        Add(a->Primary(), b->Primary(), c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 6);

        SBigDel(a);
        SBigDel(b);
    }

    SECTION("adds 0xFFFFFFFF and 0xCCCCCCCC") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 0xFFFFFFFF);

        uint64_t c = 0xCCCCCCCC;

        Add(a->Primary(), b->Primary(), c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0xCCCCCCCB);
        CHECK(a->Primary()[1] == 0x1);

        SBigDel(a);
        SBigDel(b);
    }
}

TEST_CASE("Compare", "[big]") {
    SECTION("compares 0 and 1") {
        BigData* a;
        SBigNew(&a);
        SBigFromUnsigned(a, 0);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 1);

        CHECK(Compare(a->Primary(), b->Primary()) == -1);

        SBigDel(a);
        SBigDel(b);
    }

    SECTION("compares 1 and 1") {
        BigData* a;
        SBigNew(&a);
        SBigFromUnsigned(a, 1);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 1);

        CHECK(Compare(a->Primary(), b->Primary()) == 0);

        SBigDel(a);
        SBigDel(b);
    }

    SECTION("compares 10 and 1") {
        BigData* a;
        SBigNew(&a);
        SBigFromUnsigned(a, 10);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 1);

        CHECK(Compare(a->Primary(), b->Primary()) == 1);

        SBigDel(a);
        SBigDel(b);
    }

    SECTION("compares 0x1111111111111111 and 0x22222222") {
        BigData* a;
        SBigNew(&a);
        uint64_t data = 0x1111111111111111;
        SBigFromBinary(a, reinterpret_cast<uint8_t*>(&data), sizeof(data));

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 0x22222222);

        CHECK(Compare(a->Primary(), b->Primary()) == 1);

        SBigDel(a);
        SBigDel(b);
    }

    SECTION("compares 0x11111111 and 0x2222222222222222") {
        BigData* a;
        SBigNew(&a);
        SBigFromUnsigned(a, 0x11111111);

        BigData* b;
        SBigNew(&b);
        uint64_t data = 0x2222222222222222;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&data), sizeof(data));

        CHECK(Compare(a->Primary(), b->Primary()) == -1);

        SBigDel(a);
        SBigDel(b);
    }
}

TEST_CASE("Div", "[big]") {
    SECTION("divides 2 by 1") {
        BigData* a;
        SBigNew(&a);

        uint32_t b;

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 2);

        uint64_t d = 1;

        Div(a->Primary(), &b, c->Primary(), d);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 2);
        CHECK(b == 0);

        SBigDel(a);
        SBigDel(c);
    }

    SECTION("divides 5 by 2") {
        BigData* a;
        SBigNew(&a);

        uint32_t b;

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 5);

        uint64_t d = 2;

        Div(a->Primary(), &b, c->Primary(), d);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 2);
        CHECK(b == 1);

        SBigDel(a);
        SBigDel(c);
    }

    SECTION("divides 7 by 4") {
        BigData* a;
        SBigNew(&a);

        uint32_t b;

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 7);

        uint64_t d = 4;

        Div(a->Primary(), &b, c->Primary(), d);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
        CHECK(b == 3);

        SBigDel(a);
        SBigDel(c);
    }

    SECTION("divides 0x9999444488885555 by 0x2222") {
        BigData* a;
        SBigNew(&a);

        uint32_t b;

        BigData* c;
        SBigNew(&c);
        uint64_t c_ = 0x9999444488885555;
        SBigFromBinary(c, reinterpret_cast<uint8_t*>(&c_), sizeof(c_));

        uint64_t d = 0x2222;

        Div(a->Primary(), &b, c->Primary(), d);

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0x00040002);
        CHECK(a->Primary()[1] == 0x48002);
        CHECK(b == 0x1111);

        SBigDel(a);
        SBigDel(c);
    }

    SECTION("divides 0x9999444488885555 by 0xFFFFFFFF") {
        BigData* a;
        SBigNew(&a);

        uint32_t b;

        BigData* c;
        SBigNew(&c);
        uint64_t c_ = 0x9999444488885555;
        SBigFromBinary(c, reinterpret_cast<uint8_t*>(&c_), sizeof(c_));

        uint64_t d = 0xFFFFFFFF;

        Div(a->Primary(), &b, c->Primary(), d);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 0x99994445);
        CHECK(b == 0x2221999A);

        SBigDel(a);
        SBigDel(c);
    }
}

TEST_CASE("ExtractLowPart", "[big]") {
    SECTION("extracts low part of 0") {
        uint64_t value = 0;
        auto low = ExtractLowPart(value);

        REQUIRE(low == 0);
        REQUIRE(value == 0);
    }

    SECTION("extracts low part of 0x12345678") {
        uint64_t value = 0x12345678;
        auto low = ExtractLowPart(value);

        REQUIRE(low == 0x12345678);
        REQUIRE(value == 0);
    }

    SECTION("extracts low part of 0xAAAABBBBCCCCDDDD") {
        uint64_t value = 0xAAAABBBBCCCCDDDD;
        auto low = ExtractLowPart(value);

        REQUIRE(low == 0xCCCCDDDD);
        REQUIRE(value == 0xAAAABBBB);
    }
}

TEST_CASE("ExtractLowPartLargeSum", "[big]") {
    SECTION("extracts low part after adding 2 and 4") {
        uint64_t value = 2;
        uint64_t add = 4;
        auto low = ExtractLowPartLargeSum(value, add);

        CHECK(low == 6);
        CHECK(value == 0);
    }

    SECTION("extracts low part after adding 0x1111111122222222 and 0x3333333344444444") {
        uint64_t value = 0x1111111122222222;
        uint64_t add = 0x3333333344444444;
        auto low = ExtractLowPartLargeSum(value, add);

        CHECK(low == 0x66666666);
        CHECK(value == 0x44444444);
    }

    SECTION("extracts low part after adding 0xCCCCCCCCCCCCCCCC and 0x5555555555555555") {
        uint64_t value = 0xCCCCCCCCCCCCCCCC;
        uint64_t add = 0x5555555555555555;
        auto low = ExtractLowPartLargeSum(value, add);

        CHECK(low == 0x22222221);
        CHECK(value == 0x122222222);
    }

    SECTION("extracts low part after adding 0xFFFFFFFFFFFFFFFF and 0") {
        uint64_t value = 0xFFFFFFFFFFFFFFFF;
        uint64_t add = 0;
        auto low = ExtractLowPartLargeSum(value, add);

        CHECK(low == 0xFFFFFFFF);
        CHECK(value == 0xFFFFFFFF);
    }

    SECTION("extracts low part after adding 0xFFFFFFFFFFFFFFFF and 0xFFFFFFFFFFFFFFFF") {
        uint64_t value = 0xFFFFFFFFFFFFFFFF;
        uint64_t add = 0xFFFFFFFFFFFFFFFF;
        auto low = ExtractLowPartLargeSum(value, add);

        CHECK(low == 0xFFFFFFFE);
        CHECK(value == 0x1FFFFFFFF);
    }
}

TEST_CASE("ExtractLowPartSx", "[big]") {
    SECTION("extracts low part of 0") {
        uint64_t value = 0;
        auto low = ExtractLowPartSx(value);

        REQUIRE(low == 0);
        REQUIRE(value == 0);
    }

    SECTION("extracts low part of 0x12345678") {
        uint64_t value = 0x12345678;
        auto low = ExtractLowPartSx(value);

        REQUIRE(low == 0x12345678);
        REQUIRE(value == 0);
    }

    SECTION("extracts low part of 0xAAAABBBBCCCCDDDD") {
        uint64_t value = 0xAAAABBBBCCCCDDDD;
        auto low = ExtractLowPartSx(value);

        REQUIRE(low == 0xCCCCDDDD);
        REQUIRE(value == 0xFFFFFFFFAAAABBBB);
    }
}

TEST_CASE("HighBitPos", "[big]") {
    SECTION("returns position of high bit for 0") {
        BigData* num;
        SBigNew(&num);
        SBigFromUnsigned(num, 0);

        CHECK(HighBitPos(num->Primary()) == 0);

        SBigDel(num);
    }

    SECTION("returns position of high bit for 0x1000") {
        BigData* num;
        SBigNew(&num);
        SBigFromUnsigned(num, 0x1000);

        CHECK(HighBitPos(num->Primary()) == 12);

        SBigDel(num);
    }

    SECTION("returns position of high bit for 0x1111") {
        BigData* num;
        SBigNew(&num);
        SBigFromUnsigned(num, 0x1111);

        CHECK(HighBitPos(num->Primary()) == 12);

        SBigDel(num);
    }

    SECTION("returns position of high bit for 0xFFFF") {
        BigData* num;
        SBigNew(&num);
        SBigFromUnsigned(num, 0xFFFF);

        CHECK(HighBitPos(num->Primary()) == 15);

        SBigDel(num);
    }

    SECTION("returns position of high bit for 0xFFFFFFFF") {
        BigData* num;
        SBigNew(&num);
        SBigFromUnsigned(num, 0xFFFFFFFF);

        CHECK(HighBitPos(num->Primary()) == 31);

        SBigDel(num);
    }

    SECTION("returns position of high bit for 0x123456789ABCDEF0") {
        BigData* num;
        SBigNew(&num);
        uint64_t data = 0x123456789ABCDEF0;
        SBigFromBinary(num, reinterpret_cast<uint8_t*>(&data), sizeof(data));

        CHECK(HighBitPos(num->Primary()) == 60);

        SBigDel(num);
    }
}

TEST_CASE("InsertLowPart", "[big]") {
    SECTION("inserts low part 0xABCD1111 into value 0") {
        uint64_t value = 0;
        uint32_t low = 0xABCD1111;
        InsertLowPart(value, low);

        CHECK(value == 0xABCD1111);
    }

    SECTION("inserts low part 0xABCD1111 into value 0xCCCCCCCC") {
        uint64_t value = 0xCCCCCCCC;
        uint32_t low = 0xABCD1111;
        InsertLowPart(value, low);

        CHECK(value == 0xCCCCCCCCABCD1111);
    }

    SECTION("inserts low part 0x12345678 into value 0xA0B1C2D3E4F5A6B7") {
        uint64_t value = 0xA0B1C2D3E4F5A6B7;
        uint32_t low = 0x12345678;
        InsertLowPart(value, low);

        CHECK(value == 0xE4F5A6B712345678);
    }
}

TEST_CASE("Mul", "[big]") {
    SECTION("multiplies 0 and 1") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 0);

        uint64_t c = 1;

        Mul(a->Primary(), b->Primary(), c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 0);

        SBigDel(a);
        SBigDel(b);
    }

    SECTION("multiplies 2 and 4") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 2);

        uint64_t c = 4;

        Mul(a->Primary(), b->Primary(), c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 8);

        SBigDel(a);
        SBigDel(b);
    }

    SECTION("multiplies 0xFFFFFFFF and 0x100") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 0xFFFFFFFF);

        uint64_t c = 0x100;

        Mul(a->Primary(), b->Primary(), c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0xFFFFFF00);
        CHECK(a->Primary()[1] == 0xFF);

        SBigDel(a);
        SBigDel(b);
    }
}

TEST_CASE("MakeLarge", "[big]") {
    SECTION("creates uint64_t out of 0xAABBCCDD and 0x11223344") {
        uint64_t value = MakeLarge(0xAABBCCDD, 0x11223344);

        REQUIRE(value == 0x11223344AABBCCDD);
    }

    SECTION("creates uint64_t out of 0 and 0x11223344") {
        uint64_t value = MakeLarge(0x00000000, 0x11223344);

        REQUIRE(value == 0x1122334400000000);
    }
}

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

TEST_CASE("SetOne", "[big]") {
    SECTION("sets buffer to one") {
        BigData* num;
        SBigNew(&num);
        uint64_t data = 0x123456789ABCDEF0;
        SBigFromBinary(num, reinterpret_cast<uint8_t*>(&data), sizeof(data));

        CHECK(num->Primary().Count() == 2);

        SetOne(num->Primary());

        CHECK(num->Primary().Count() == 1);
        CHECK(num->Primary()[0] == 1);

        SBigDel(num);
    }
}

TEST_CASE("SetZero", "[big]") {
    SECTION("sets buffer to zero") {
        BigData* num;
        SBigNew(&num);
        SBigFromUnsigned(num, 0x12345678);

        CHECK(num->Primary().Count() == 1);

        SetZero(num->Primary());

        CHECK(num->Primary().Count() == 0);

        SBigDel(num);
    }
}

TEST_CASE("Sub", "[big]") {
    SECTION("subtracts 0 from 1") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 1);

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 0);

        Sub(a->Primary(), b->Primary(), c->Primary());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);

        SBigDel(a);
        SBigDel(b);
    }

    SECTION("subtracts 1 from 2") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        SBigFromUnsigned(b, 2);

        BigData* c;
        SBigNew(&c);
        SBigFromUnsigned(c, 1);

        Sub(a->Primary(), b->Primary(), c->Primary());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);

        SBigDel(a);
        SBigDel(b);
    }

    SECTION("subtracts 0x1111111111111111 from 0x9999999999999999") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        uint64_t b_ = 0x9999999999999999;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&b_), sizeof(b_));

        BigData* c;
        SBigNew(&c);
        uint64_t c_ = 0x1111111111111111;
        SBigFromBinary(c, reinterpret_cast<uint8_t*>(&c_), sizeof(c_));

        Sub(a->Primary(), b->Primary(), c->Primary());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0x88888888);
        CHECK(a->Primary()[1] == 0x88888888);

        SBigDel(a);
        SBigDel(b);
    }

    SECTION("subtracts 0x123456789ABCDEF0 from 0xFEDCBA9876543210") {
        BigData* a;
        SBigNew(&a);

        BigData* b;
        SBigNew(&b);
        uint64_t b_ = 0xFEDCBA9876543210;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&b_), sizeof(b_));

        BigData* c;
        SBigNew(&c);
        uint64_t c_ = 0x123456789ABCDEF0;
        SBigFromBinary(c, reinterpret_cast<uint8_t*>(&c_), sizeof(c_));

        Sub(a->Primary(), b->Primary(), c->Primary());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0xDB975320);
        CHECK(a->Primary()[1] == 0xECA8641F);

        SBigDel(a);
        SBigDel(b);
    }
}
