#include "storm/Big.hpp"
#include "storm/big/Ops.hpp"
#include "test/Test.hpp"
#include <vector>

TEST_CASE("Add", "[big]") {
    BigDataTest a, b;

    SECTION("adds 0 and 1") {
        SBigFromUnsigned(b, 0);

        uint32_t c = 1;

        Add(a->Primary(), b->Primary(), c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
    }

    SECTION("adds 2 and 4") {
        SBigFromUnsigned(b, 2);

        uint32_t c = 4;

        Add(a->Primary(), b->Primary(), c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 6);
    }

    SECTION("adds 0xFFFFFFFF and 0xCCCCCCCC") {
        SBigFromUnsigned(b, 0xFFFFFFFF);

        uint32_t c = 0xCCCCCCCC;

        Add(a->Primary(), b->Primary(), c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0xCCCCCCCB);
        CHECK(a->Primary()[1] == 0x1);
    }
}

TEST_CASE("And", "[big]") {
    BigDataTest a, b, c;

    SECTION("overwrites output") {
        SBigFromUnsigned(a, 123456);
        SBigFromUnsigned(b, 0);
        SBigFromUnsigned(c, 0);

        And(a->Primary(), b->Primary(), c->Primary());

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

        And(a->Primary(), b->Primary(), c->Primary());

        CHECK(a->Primary().Count() == 4);
        CHECK(a->Primary()[0] == 0x04000000);
        CHECK(a->Primary()[1] == 0x00060004);
        CHECK(a->Primary()[2] == 0x041F0200);
        CHECK(a->Primary()[3] == 0x00000600);
    }
}

TEST_CASE("Compare", "[big]") {
    BigDataTest a, b;

    SECTION("compares 0 and 1") {
        SBigFromUnsigned(a, 0);
        SBigFromUnsigned(b, 1);

        CHECK(Compare(a->Primary(), b->Primary()) == -1);
    }

    SECTION("compares 1 and 1") {
        SBigFromUnsigned(a, 1);
        SBigFromUnsigned(b, 1);

        CHECK(Compare(a->Primary(), b->Primary()) == 0);
    }

    SECTION("compares 10 and 1") {
        SBigFromUnsigned(a, 10);
        SBigFromUnsigned(b, 1);

        CHECK(Compare(a->Primary(), b->Primary()) == 1);
    }

    SECTION("compares 0x1111111111111111 and 0x22222222") {
        uint64_t data = 0x1111111111111111;
        SBigFromBinary(a, reinterpret_cast<uint8_t*>(&data), sizeof(data));
        SBigFromUnsigned(b, 0x22222222);

        CHECK(Compare(a->Primary(), b->Primary()) == 1);
    }

    SECTION("compares 0x11111111 and 0x2222222222222222") {
        SBigFromUnsigned(a, 0x11111111);
        
        uint64_t data = 0x2222222222222222;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&data), sizeof(data));

        CHECK(Compare(a->Primary(), b->Primary()) == -1);
    }
}

TEST_CASE("Div", "[big]") {
    BigDataTest a, b, c, d;
    SECTION("divides 2 by 1") {
        uint32_t b;
        SBigFromUnsigned(c, 2);

        uint64_t d = 1;

        Div(a->Primary(), &b, c->Primary(), d);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 2);
        CHECK(b == 0);
    }

    SECTION("divides 2 by 1 (buffer divisor)") {
        SBigFromUnsigned(c, 2);
        SBigFromUnsigned(d, 1);

        Div(a->Primary(), b->Primary(), c->Primary(), d->Primary(), a->Stack());

        a->Primary().Trim();
        b->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 2);
        CHECK(b->Primary().Count() == 0);
    }

    SECTION("divides 5 by 2") {
        uint32_t b;

        SBigFromUnsigned(c, 5);

        uint64_t d = 2;

        Div(a->Primary(), &b, c->Primary(), d);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 2);
        CHECK(b == 1);
    }

    SECTION("divides 7 by 4") {
        uint32_t b;

        SBigFromUnsigned(c, 7);

        uint64_t d = 4;

        Div(a->Primary(), &b, c->Primary(), d);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
        CHECK(b == 3);
    }

    SECTION("divides 7 by 4 (buffer divisor)") {
        SBigFromUnsigned(c, 7);
        SBigFromUnsigned(d, 4);

        Div(a->Primary(), b->Primary(), c->Primary(), d->Primary(), a->Stack());

        a->Primary().Trim();
        b->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
        CHECK(b->Primary().Count() == 1);
        CHECK(b->Primary()[0] == 3);
    }

    SECTION("divides 0x9999444488885555 by 0x2222") {
        uint32_t b;

        uint64_t c_ = 0x9999444488885555;
        SBigFromBinary(c, reinterpret_cast<uint8_t*>(&c_), sizeof(c_));

        uint64_t d = 0x2222;

        Div(a->Primary(), &b, c->Primary(), d);

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0x00040002);
        CHECK(a->Primary()[1] == 0x48002);
        CHECK(b == 0x1111);
    }

    SECTION("divides 0x9999444488885555 by 0x2222 (buffer divisor)") {
        uint64_t c_ = 0x9999444488885555;
        SBigFromBinary(c, reinterpret_cast<uint8_t*>(&c_), sizeof(c_));

        SBigFromUnsigned(d, 0x2222);

        Div(a->Primary(), b->Primary(), c->Primary(), d->Primary(), a->Stack());

        a->Primary().Trim();
        b->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0x00040002);
        CHECK(a->Primary()[1] == 0x48002);
        CHECK(b->Primary().Count() == 1);
        CHECK(b->Primary()[0] == 0x1111);
    }

    SECTION("divides 0x9999444488885555 by 0xFFFFFFFF") {
        uint32_t b;

        uint64_t c_ = 0x9999444488885555;
        SBigFromBinary(c, reinterpret_cast<uint8_t*>(&c_), sizeof(c_));

        uint64_t d = 0xFFFFFFFF;

        Div(a->Primary(), &b, c->Primary(), d);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 0x99994445);
        CHECK(b == 0x2221999A);
    }

    SECTION("divides 0x9999444488885555 by 0xFFFFFFFF (buffer divisor)") {
        uint64_t c_ = 0x9999444488885555;
        SBigFromBinary(c, reinterpret_cast<uint8_t*>(&c_), sizeof(c_));

        SBigFromUnsigned(d, 0xFFFFFFFF);

        Div(a->Primary(), b->Primary(), c->Primary(), d->Primary(), a->Stack());

        a->Primary().Trim();
        b->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 0x99994445);
        CHECK(b->Primary().Count() == 1);
        CHECK(b->Primary()[0] == 0x2221999A);
    }

    SECTION("divides 0x9999444488885555 by 0x1111222233334444 (buffer divisor)") {
        uint64_t c_ = 0x9999444488885555;
        SBigFromBinary(c, reinterpret_cast<uint8_t*>(&c_), sizeof(c_));

        uint64_t d_ = 0x1111222233334444;
        SBigFromBinary(d, reinterpret_cast<uint8_t*>(&d_), sizeof(d_));

        Div(a->Primary(), b->Primary(), c->Primary(), d->Primary(), a->Stack());

        a->Primary().Trim();
        b->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 8);
        CHECK(b->Primary().Count() == 2);
        CHECK(b->Primary()[0] == 0xEEEE3335);
        CHECK(b->Primary()[1] == 0x11103332);
    }
}

TEST_CASE("EncodeDataBytes", "[big]") {
    struct EncodeTestCase {
        uint32_t input;
        std::vector<uint8_t> output;
    };

    SECTION("encodes values to array") {
        auto v = GENERATE(
            EncodeTestCase{ 0, { 255 } },
            EncodeTestCase{ 1, { 1, 255 } },
            EncodeTestCase{ 255, { 0, 1, 255 } },
            EncodeTestCase{ UINT32_MAX, { 0, 4, 6, 4, 1, 255 } }
        );

        TSGrowableArray<uint8_t> arr;
        EncodeDataBytes(arr, v.input);

        CHECK(arr.Count() == v.output.size());
        CHECK(std::vector<uint8_t>(arr.m_data, arr.m_data + arr.Count()) == v.output);
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

    SECTION("extracts low part of 0x80000000") {
        uint64_t value = 0x80000000;
        auto low = ExtractLowPartSx(value);

        REQUIRE(low == 0x80000000);
        REQUIRE(value == 0);
    }

    SECTION("extracts low part of 0xFFFFFFFF") {
        uint64_t value = 0xFFFFFFFF;
        auto low = ExtractLowPartSx(value);

        REQUIRE(low == 0xFFFFFFFF);
        REQUIRE(value == 0);
    }

    SECTION("extracts low part of 0xAAAABBBBCCCCDDDD") {
        uint64_t value = 0xAAAABBBBCCCCDDDD;
        auto low = ExtractLowPartSx(value);

        REQUIRE(low == 0xCCCCDDDD);
        REQUIRE(value == 0xFFFFFFFFAAAABBBB);
    }

    SECTION("extracts low part of 0xFFFFFFFF85B10E3A") {
        uint64_t value = 0xFFFFFFFF85B10E3A;
        auto low = ExtractLowPartSx(value);

        REQUIRE(low == 0x85B10E3A);
        REQUIRE(value == 0xFFFFFFFFFFFFFFFF);
    }
}

TEST_CASE("HighBitPos", "[big]") {
    BigDataTest num;
    SECTION("returns position of high bit for 0") {
        SBigFromUnsigned(num, 0);

        CHECK(HighBitPos(num->Primary()) == 0);
    }

    SECTION("returns position of high bit for 0x1000") {
        SBigFromUnsigned(num, 0x1000);

        CHECK(HighBitPos(num->Primary()) == 12);
    }

    SECTION("returns position of high bit for 0x1111") {
        SBigFromUnsigned(num, 0x1111);

        CHECK(HighBitPos(num->Primary()) == 12);
    }

    SECTION("returns position of high bit for 0xFFFF") {
        SBigFromUnsigned(num, 0xFFFF);

        CHECK(HighBitPos(num->Primary()) == 15);
    }

    SECTION("returns position of high bit for 0xFFFFFFFF") {
        SBigFromUnsigned(num, 0xFFFFFFFF);

        CHECK(HighBitPos(num->Primary()) == 31);
    }

    SECTION("returns position of high bit for 0x123456789ABCDEF0") {
        uint64_t data = 0x123456789ABCDEF0;
        SBigFromBinary(num, reinterpret_cast<uint8_t*>(&data), sizeof(data));

        CHECK(HighBitPos(num->Primary()) == 60);
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
    BigDataTest a, b;

    SECTION("multiplies 0 and 1") {
        SBigFromUnsigned(b, 0);

        uint64_t c = 1;

        Mul(a->Primary(), b->Primary(), c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 0);
    }

    SECTION("multiplies 2 and 4") {
        SBigFromUnsigned(b, 2);

        uint64_t c = 4;

        Mul(a->Primary(), b->Primary(), c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 8);
    }

    SECTION("multiplies 0xFFFFFFFF and 0x100") {
        SBigFromUnsigned(b, 0xFFFFFFFF);

        uint64_t c = 0x100;

        Mul(a->Primary(), b->Primary(), c);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0xFFFFFF00);
        CHECK(a->Primary()[1] == 0xFF);
    }
}

TEST_CASE("MulMod", "[big]") {
    BigDataTest a, b, c, d;

    SECTION("multiplies 0xFFFFFFFF by 0x100 and mods the result by 0xABC") {
        SBigFromUnsigned(b, 0xFFFFFFFF);
        SBigFromUnsigned(c, 0x100);
        SBigFromUnsigned(d, 0xABC);

        MulMod(a->Primary(), b->Primary(), c->Primary(), d->Primary(), a->Stack());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 0x624);
    }

    SECTION("multiplies 0x123456789ABCDEF0 by 0xFEDCBA9876543210 and mods the result by 0x10000000") {
        uint64_t b_ = 0x123456789ABCDEF0;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&b_), sizeof(b_));

        uint64_t c_ = 0xFEDCBA9876543210;
        SBigFromBinary(c, reinterpret_cast<uint8_t*>(&c_), sizeof(c_));

        SBigFromUnsigned(d, 0x10000000);

        MulMod(a->Primary(), b->Primary(), c->Primary(), d->Primary(), a->Stack());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 0x618CF00);
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

TEST_CASE("PowMod", "[big]") {
    BigDataTest a, b, c, d;

    SECTION("takes 0 to the 0th power and mods the result by 1") {
        SBigFromUnsigned(b, 0);
        SBigFromUnsigned(c, 0);
        SBigFromUnsigned(d, 1);

        PowMod(a->Primary(), b->Primary(), c->Primary(), d->Primary(), a->Stack());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
    }

    SECTION("takes 2 to the 0th power and mods the result by 7") {
        SBigFromUnsigned(b, 2);
        SBigFromUnsigned(c, 0);
        SBigFromUnsigned(d, 7);

        PowMod(a->Primary(), b->Primary(), c->Primary(), d->Primary(), a->Stack());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
    }

    SECTION("takes 2 to the 4th power and mods the result by 7") {
        SBigFromUnsigned(b, 2);
        SBigFromUnsigned(c, 4);
        SBigFromUnsigned(d, 7);

        PowMod(a->Primary(), b->Primary(), c->Primary(), d->Primary(), a->Stack());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 2);
    }

    SECTION("takes 256 to the 8th power and mods the result by 999") {
        SBigFromUnsigned(b, 256);
        SBigFromUnsigned(c, 8);
        SBigFromUnsigned(d, 999);

        PowMod(a->Primary(), b->Primary(), c->Primary(), d->Primary(), a->Stack());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 160);
    }

    SECTION("takes 0x100000000 to the 2nd power and mods the result by 0xAAAAFFFF") {
        uint64_t b_ = 0x100000000;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&b_), sizeof(b_));

        SBigFromUnsigned(c, 2);
        SBigFromUnsigned(d, 0xAAAAFFFF);

        PowMod(a->Primary(), b->Primary(), c->Primary(), d->Primary(), a->Stack());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 0x6AA94002);
    }

    SECTION("takes 0xABCDEF1234567890 to the 16th power and mods the result by 0xEEEE000000000001") {
        uint64_t b_ = 0xABCDEF1234567890;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&b_), sizeof(b_));

        SBigFromUnsigned(c, 16);

        uint64_t d_ = 0xEEEE000000000001;
        SBigFromBinary(d, reinterpret_cast<uint8_t*>(&d_), sizeof(d_));

        PowMod(a->Primary(), b->Primary(), c->Primary(), d->Primary(), a->Stack());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0x950A5465);
        CHECK(a->Primary()[1] == 0xA0CB742F);
    }
}

TEST_CASE("SetOne", "[big]") {
    BigDataTest num;

    SECTION("sets buffer to one") {
        uint64_t data = 0x123456789ABCDEF0;
        SBigFromBinary(num, reinterpret_cast<uint8_t*>(&data), sizeof(data));

        CHECK(num->Primary().Count() == 2);

        SetOne(num->Primary());

        CHECK(num->Primary().Count() == 1);
        CHECK(num->Primary()[0] == 1);
    }
}

TEST_CASE("SetZero", "[big]") {
    BigDataTest num;
    
    SECTION("sets buffer to zero") {
        SBigFromUnsigned(num, 0x12345678);

        CHECK(num->Primary().Count() == 1);

        SetZero(num->Primary());

        CHECK(num->Primary().Count() == 0);
    }
}

TEST_CASE("Shl", "[big]") {
    BigDataTest a, b;

    SECTION("shifts 0 left 1 bit") {
        SBigFromUnsigned(b, 0);

        Shl(a->Primary(), b->Primary(), 1);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 0);
    }

    SECTION("shifts 1 left 0 bits") {
        SBigFromUnsigned(b, 1);

        Shl(a->Primary(), b->Primary(), 0);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
    }

    SECTION("shifts 1 left 1 bit") {
        SBigFromUnsigned(b, 1);

        Shl(a->Primary(), b->Primary(), 1);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 2);
    }

    SECTION("shifts 2 left 1 bit") {
        SBigFromUnsigned(b, 2);

        Shl(a->Primary(), b->Primary(), 1);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 4);
    }

    SECTION("shifts 256 left 7 bits") {
        SBigFromUnsigned(b, 256);

        Shl(a->Primary(), b->Primary(), 7);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 32768);
    }

    SECTION("shifts 0xFFFFFFFF left 10 bits") {
        uint64_t b_ = 0xFFFFFFFF;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&b_), sizeof(b_));

        Shl(a->Primary(), b->Primary(), 10);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0xFFFFFC00);
        CHECK(a->Primary()[1] == 0x3FF);
    }

    SECTION("shifts 0x1000000000000000 left 1 bit") {
        uint64_t b_ = 0x1000000000000000;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&b_), sizeof(b_));

        Shl(a->Primary(), b->Primary(), 1);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0);
        CHECK(a->Primary()[1] == 0x20000000);
    }

    SECTION("shifts 0x1000000000000000 left 16 bits") {
        uint64_t b_ = 0x1000000000000000;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&b_), sizeof(b_));

        Shl(a->Primary(), b->Primary(), 16);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 3);
        CHECK(a->Primary()[0] == 0);
        CHECK(a->Primary()[1] == 0);
        CHECK(a->Primary()[2] == 0x1000);
    }
}

TEST_CASE("Shr", "[big]") {
    BigDataTest a, b;

    SECTION("shifts 0 right 1 bit") {
        SBigFromUnsigned(b, 0);

        Shr(a->Primary(), b->Primary(), 1);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 0);
    }

    SECTION("shifts 1 right 0 bits") {
        SBigFromUnsigned(b, 1);

        Shr(a->Primary(), b->Primary(), 0);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
    }

    SECTION("shifts 1 right 1 bit") {
        SBigFromUnsigned(b, 1);

        Shr(a->Primary(), b->Primary(), 1);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 0);
    }

    SECTION("shifts 2 right 1 bit") {
        SBigFromUnsigned(b, 2);

        Shr(a->Primary(), b->Primary(), 1);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
    }

    SECTION("shifts 2 right 1 bit") {
        SBigFromUnsigned(b, 2);

        Shr(a->Primary(), b->Primary(), 1);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
    }

    SECTION("shifts 256 right 7 bits") {
        SBigFromUnsigned(b, 256);

        Shr(a->Primary(), b->Primary(), 7);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 2);
    }

    SECTION("shifts 0x1000000000000000 right 2 bits") {
        uint64_t b_ = 0x1000000000000000;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&b_), sizeof(b_));

        Shr(a->Primary(), b->Primary(), 2);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0);
        CHECK(a->Primary()[1] == 0x4000000);
    }

    SECTION("shifts 0x1000000000000000 right 32 bits") {
        uint64_t b_ = 0x1000000000000000;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&b_), sizeof(b_));

        Shr(a->Primary(), b->Primary(), 32);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 0x10000000);
    }
}

TEST_CASE("Square", "[big]") {
    BigDataTest a, b;

    SECTION("squares 0") {
        SBigFromUnsigned(b, 0);

        Square(a->Primary(), b->Primary(), a->Stack());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 0);
    }

    SECTION("squares 1") {
        SBigFromUnsigned(b, 1);

        Square(a->Primary(), b->Primary(), a->Stack());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
    }

    SECTION("squares 2") {
        SBigFromUnsigned(b, 2);

        Square(a->Primary(), b->Primary(), a->Stack());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 4);
    }

    SECTION("squares 2") {
        SBigFromUnsigned(b, 2);

        Square(a->Primary(), b->Primary(), a->Stack());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 4);
    }

    SECTION("squares 0xFFFFFFFF") {
        SBigFromUnsigned(b, 0xFFFFFFFF);

        Square(a->Primary(), b->Primary(), a->Stack());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0x1);
        CHECK(a->Primary()[1] == 0xFFFFFFFE);
    }

    SECTION("squares 0x1111111111111111") {
        uint64_t b_ = 0x1111111111111111;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&b_), sizeof(b_));

        Square(a->Primary(), b->Primary(), a->Stack());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 4);
        CHECK(a->Primary()[0] == 0x87654321);
        CHECK(a->Primary()[1] == 0xfedcba9);
        CHECK(a->Primary()[2] == 0x89abcdf0);
        CHECK(a->Primary()[3] == 0x1234567);
    }
}

TEST_CASE("Sub", "[big]") {
    BigDataTest a, b, c;

    SECTION("subtracts 0 from 1") {
        SBigFromUnsigned(b, 1);
        SBigFromUnsigned(c, 0);

        Sub(a->Primary(), b->Primary(), c->Primary());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
    }

    SECTION("subtracts int 0 from bigint 1") {
        SBigFromUnsigned(b, 1);

        Sub(a->Primary(), b->Primary(), 0);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
    }

#ifdef NDEBUG
    SECTION("subtracts 1 from 0") {
        SBigFromUnsigned(b, 0);
        SBigFromUnsigned(c, 1);

        Sub(a->Primary(), b->Primary(), c->Primary());

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 0xFFFFFFFF);
    }

    SECTION("subtracts int 1 from bigint 0") {
        SBigFromUnsigned(b, 0);

        Sub(a->Primary(), b->Primary(), 1);

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 0xFFFFFFFF);
    }
#endif

    SECTION("subtracts 1 from 2") {
        SBigFromUnsigned(b, 2);
        SBigFromUnsigned(c, 1);

        Sub(a->Primary(), b->Primary(), c->Primary());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
    }

    SECTION("subtracts int 1 from bigint 2") {
        SBigFromUnsigned(b, 2);

        Sub(a->Primary(), b->Primary(), 1);

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 1);
        CHECK(a->Primary()[0] == 1);
    }

    SECTION("subtracts 0x1111111111111111 from 0x9999999999999999") {
        uint64_t b_ = 0x9999999999999999;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&b_), sizeof(b_));

        uint64_t c_ = 0x1111111111111111;
        SBigFromBinary(c, reinterpret_cast<uint8_t*>(&c_), sizeof(c_));

        Sub(a->Primary(), b->Primary(), c->Primary());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0x88888888);
        CHECK(a->Primary()[1] == 0x88888888);
    }

    SECTION("subtracts 0x123456789ABCDEF0 from 0xFEDCBA9876543210") {
        uint64_t b_ = 0xFEDCBA9876543210;
        SBigFromBinary(b, reinterpret_cast<uint8_t*>(&b_), sizeof(b_));

        uint64_t c_ = 0x123456789ABCDEF0;
        SBigFromBinary(c, reinterpret_cast<uint8_t*>(&c_), sizeof(c_));

        Sub(a->Primary(), b->Primary(), c->Primary());

        a->Primary().Trim();

        CHECK(a->Primary().Count() == 2);
        CHECK(a->Primary()[0] == 0xDB975320);
        CHECK(a->Primary()[1] == 0xECA8641F);
    }
}

TEST_CASE("ToStream", "[big]") {
    BigDataTest a;

    struct ToStreamTestCase {
        uint64_t input;
        std::vector<uint8_t> output;
    };

    SECTION("streams buffers to array") {
        auto v = GENERATE(
            ToStreamTestCase{ 0, { 255 } },
            ToStreamTestCase{ 1, { 1, 1, 255, 1 } },
            ToStreamTestCase{ 255, { 0xFF, 1, 255, 0xFF } },
            ToStreamTestCase{ UINT32_MAX, { 0xFF, 0xFF, 0xFF, 0xFF, 4, 255, 0xFF, 0xFF, 0xFF, 0xFF } },
            ToStreamTestCase{ 0x123456789ABCDEF0, { 0xF0, 0xDE, 0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12, 8, 255, 0xF0, 0xDE, 0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12 } }
        );

        TSGrowableArray<uint8_t> arr;
        SBigFromStr(a, std::to_string(v.input).c_str());
        ToStream(arr, a->Primary());

        CHECK(std::vector<uint8_t>(arr.m_data, arr.m_data + arr.Count()) == v.output);
    }
}
