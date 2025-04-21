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
        SRgnCreate(&region, 0);

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

TEST_CASE("SRgnGetRectsf", "[region]") {
    RgnDataTest region;

    SECTION("retrieves empty list if nothing put in") {
        uint32_t numrects = 1;
        RECTF buffer[1];

        SRgnGetRectsf(region, &numrects, buffer);

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
