#include "RegionTest.hpp"

TEST_CASE("SRgnClear", "[region]") {
    RgnDataTest region;

    SECTION("operates on an empty object") {
        uint32_t numrects = 0;

        SRgnClear(region);

        SRgnGetRectsf(region, &numrects, nullptr);
        CHECK(numrects == 0);
    }

    SECTION("clears rects out of a region object") {
        uint32_t numrects = 0;

        RECTF baseRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        SRgnGetRectsf(region, &numrects, nullptr);
        CHECK(numrects == 1);

        SRgnClear(region);

        SRgnGetRectsf(region, &numrects, nullptr);
        CHECK(numrects == 0);
    }
}

TEST_CASE("SRgnCreate", "[region]") {
    SECTION("sets handle pointer to new region handle") {
        HSRGN region = nullptr;
        SRgnCreate(&region);

        REQUIRE(region != nullptr);

        SRgnDelete(region);
    }
}

TEST_CASE("SRgnCombineRectf", "[region]") {
    RgnDataTest region;

    SECTION("combines the region with a single given rect 1") {
        RECTF baseRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        RECTF boundingRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnGetBoundingRectf(region, &boundingRect);

        CHECK_THAT(boundingRect, MatchesRect({ 0.0f, 0.0f, 1.0f, 1.0f }));
    }

    SECTION("combines the region with multiple given rects 1") {
        RECTF baseRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        RECTF newRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnCombineRectf(region, &newRect, nullptr, SRGN_DIFF);

        RECTF boundingRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnGetBoundingRectf(region, &boundingRect);

        CHECK_THAT(boundingRect, MatchesRect({ 0.0f, 0.0f, 1.0f, 1.0f }));
    }

    SECTION("combines the region with multiple given rects 2") {
        RECTF baseRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        RECTF newRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &newRect, nullptr, SRGN_DIFF);

        RECTF boundingRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnGetBoundingRectf(region, &boundingRect);

        CHECK_THAT(boundingRect, MatchesRect({ 0.0f, 0.0f, 0.0f, 0.0f }));
    }

    SECTION("combines the region with multiple given rects 3") {
        RECTF baseRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        RECTF newRect = { 0.0f, 1.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &newRect, nullptr, SRGN_DIFF);

        RECTF boundingRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnGetBoundingRectf(region, &boundingRect);

        CHECK_THAT(boundingRect, MatchesRect({ 0.0f, 0.0f, 1.0f, 1.0f }));
    }
}

TEST_CASE("SRgnDelete", "[region]") {
    SECTION("deletes region data") {
        HSRGN region = nullptr;
        SRgnCreate(&region);

        RECTF baseRect = { -1.0f, 1.0f, 1.0f, 2.0f };
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        uint32_t numrects = 0;

        SRgnGetRectsf(region, &numrects, nullptr);
        CHECK(numrects == 1);

        SRgnDelete(region);

        SRgnGetRectsf(region, &numrects, nullptr);
        CHECK(numrects == 0);
    }
}

TEST_CASE("SRgnDuplicate", "[region]") {
    RgnDataTest region;

    SECTION("creates an independent copy of a region") {
        RECTF baseRect = { -1.0f, 1.0f, 1.0f, 2.0f };
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        HSRGN newrgn = nullptr;
        SRgnDuplicate(region, &newrgn);

        REQUIRE(newrgn != nullptr);
        SRgnClear(region);

        uint32_t numrects = 1;
        RECTF buffer[1];
        SRgnGetRectsf(newrgn, &numrects, buffer);

        REQUIRE(numrects == 1);
        CHECK_THAT(buffer[0], MatchesRect(baseRect));
    }

    SECTION("sets handle to null when using an invalid region object") {
        HSRGN inval = reinterpret_cast<HSRGN>(1234);

        HSRGN newrgn = reinterpret_cast<HSRGN>(12345);
        SRgnDuplicate(inval, &newrgn);

        CHECK(newrgn == nullptr);
    }
}

TEST_CASE("SRgnGetRectParamsf", "[region]") {
    RgnDataTest region;

    SECTION("retrieves empty list if nothing put in") {
        uint32_t numParams = 1;
        void* buffer[1];

        RECTF rect = { 0, 0, 1, 1 };
        SRgnGetRectParamsf(region, &rect, &numParams, buffer);

        CHECK(numParams == 0);
    }

    SECTION("retrieves 0 when using an invalid region object") {
        HSRGN inval = reinterpret_cast<HSRGN>(1234);

        uint32_t numParams = 1;
        RECTF rect = { 0, 0, 1, 1 };
        SRgnGetRectParamsf(inval, &rect, &numParams, nullptr);

        CHECK(numParams == 0);
    }

    SECTION("retrieves only overlapping params") {
        RECTF rect1 = { 0, 0, 1, 1 };
        RECTF rect2 = { 2, 2, 4, 4 };
        RECTF rect3 = { 2.5, 2.5, 5, 5 };
        RECTF queryRect = { 3, 3, 5, 5 };
        int param1 = 5;
        int param2 = 10;
        int param3 = 11;

        SRgnCombineRectf(region, &rect1, &param1, SRGN_PARAMONLY);
        SRgnCombineRectf(region, &rect2, &param2, SRGN_PARAMONLY);
        SRgnCombineRectf(region, &rect3, &param3, SRGN_PARAMONLY);

        uint32_t numParams = 0;
        SRgnGetRectParamsf(region, &queryRect, &numParams, nullptr);
        REQUIRE(numParams == 2);

        void* buffer[2];
        SRgnGetRectParamsf(region, &queryRect, &numParams, buffer);
        CHECK(numParams == 2);
        CHECK(buffer[0] == &param2);
        CHECK(buffer[1] == &param3);
    }
}

TEST_CASE("SRgnGetRectsf", "[region]") {
    RgnDataTest region;

    SECTION("retrieves empty list if nothing put in") {
        uint32_t numrects = 1;
        RECTF buffer[1];

        SRgnGetRectsf(region, &numrects, buffer);

        CHECK(numrects == 0);
    }

    SECTION("retrieves 0 when using an invalid region object") {
        HSRGN inval = reinterpret_cast<HSRGN>(1234);

        uint32_t numrects = 1;
        SRgnGetRectsf(inval, &numrects, nullptr);

        CHECK(numrects == 0);
    }

    SECTION("retrieves all rects that were put in") {
        RECTF rct1 = { 5, 5, 10, 10 };
        RECTF rct2 = { 0, 0, 1, 1 };
        SRgnCombineRectf(region, &rct1, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &rct2, nullptr, SRGN_OR);

        uint32_t numrects = 0;
        RECTF buffer[2];
        SRgnGetRectsf(region, &numrects, nullptr);

        REQUIRE(numrects == 2);
        SRgnGetRectsf(region, &numrects, buffer);

        CHECK_THAT(buffer[0], MatchesRect(rct2));
        CHECK_THAT(buffer[1], MatchesRect(rct1));
    }

    SECTION("automatically merges overlapping rects") {
        RECTF rct1 = { -10, -10, 10, 10 };
        RECTF rct2 = { 0, 0, 1, 1 };
        SRgnCombineRectf(region, &rct1, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &rct2, nullptr, SRGN_OR);

        uint32_t numrects = 0;
        RECTF buffer[1];
        SRgnGetRectsf(region, &numrects, nullptr);

        REQUIRE(numrects == 1);
        SRgnGetRectsf(region, &numrects, buffer);

        CHECK_THAT(buffer[0], MatchesRect(rct1));
    }
}

TEST_CASE("SRgnIsPointInRegionf", "[region]") {
    RgnDataTest region;

    SECTION("false if region has no rects") {
        auto x = GENERATE(0.0f, 1.0f, INFINITY);
        auto y = GENERATE(0.0f, -1.0f, -INFINITY);

        CHECK_FALSE(SRgnIsPointInRegionf(region, x, y));
    }

    SECTION("false if using an invalid region object") {
        HSRGN inval = reinterpret_cast<HSRGN>(1234);

        CHECK_FALSE(SRgnIsPointInRegionf(inval, 0.0f, 0.0f));
    }

    SECTION("reports if points are inside a region") {
        RECTF rct1 = { -10, -10, -1, -1 };
        RECTF rct2 = { 1, 1, 5, 5 };
        SRgnCombineRectf(region, &rct1, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &rct2, nullptr, SRGN_OR);

        CHECK_FALSE(SRgnIsPointInRegionf(region, 0.0f, 0.0f));
        CHECK_FALSE(SRgnIsPointInRegionf(region, -11.0f, -5.0f));
        CHECK_FALSE(SRgnIsPointInRegionf(region, 4.0f, 6.0f));
        CHECK_FALSE(SRgnIsPointInRegionf(region, 1.0f, 0.0f));
        CHECK_FALSE(SRgnIsPointInRegionf(region, 0.0f, -1.0f));
        CHECK(SRgnIsPointInRegionf(region, -10.0f, -10.0f));
        CHECK(SRgnIsPointInRegionf(region, -1.1f, -1.1f));
        CHECK(SRgnIsPointInRegionf(region, 3.0f, 2.0f));
    }

    SECTION("ignores param only rects") {
        RECTF rect = { -1, -1, 1, 1 };
        SRgnCombineRectf(region, &rect, &rect, SRGN_PARAMONLY);

        CHECK_FALSE(SRgnIsPointInRegionf(region, 0.0f, 0.0f));
    }

    SECTION("excludes upper bounds of rect") {
        RECTF rect = { 1, 1, 5, 5 };
        SRgnCombineRectf(region, &rect, nullptr, SRGN_OR);

        CHECK_FALSE(SRgnIsPointInRegionf(region, 5.0f, 5.0f));
        CHECK_FALSE(SRgnIsPointInRegionf(region, 5.0f, 4.0f));
        CHECK_FALSE(SRgnIsPointInRegionf(region, 4.0f, 5.0f));
        CHECK(SRgnIsPointInRegionf(region, 4.9999f, 4.9999f));
        CHECK(SRgnIsPointInRegionf(region, 1.0f, 1.0f));
        CHECK_FALSE(SRgnIsPointInRegionf(region, 0.9999f, 1.0f));
    }
}
