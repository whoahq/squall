#include "storm/Region.hpp"
#include "test/Test.hpp"

TEST_CASE("SRgnCreate", "[region]") {
    SECTION("sets handle pointer to new region handle") {
        HSRGN region;
        SRgnCreate(&region, 0);

        REQUIRE(region != nullptr);

        SRgnDelete(region);
    }
}

TEST_CASE("SRgnCombineRectf", "[region]") {
    SECTION("combines the region with a single given rect 1") {
        HSRGN region;
        SRgnCreate(&region, 0);

        RECTF baseRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        RECTF boundingRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnGetBoundingRectf(region, &boundingRect);

        REQUIRE(boundingRect.left == 0.0f);
        REQUIRE(boundingRect.bottom == 0.0f);
        REQUIRE(boundingRect.right == 1.0f);
        REQUIRE(boundingRect.top == 1.0f);

        SRgnDelete(region);
    }

    SECTION("combines the region with multiple given rects 1") {
        HSRGN region;
        SRgnCreate(&region, 0);

        RECTF baseRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        RECTF newRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnCombineRectf(region, &newRect, nullptr, SRGN_DIFF);

        RECTF boundingRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnGetBoundingRectf(region, &boundingRect);

        REQUIRE(boundingRect.left == 0.0f);
        REQUIRE(boundingRect.bottom == 0.0f);
        REQUIRE(boundingRect.right == 1.0f);
        REQUIRE(boundingRect.top == 1.0f);

        SRgnDelete(region);
    }

    SECTION("combines the region with multiple given rects 2") {
        HSRGN region;
        SRgnCreate(&region, 0);

        RECTF baseRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        RECTF newRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &newRect, nullptr, SRGN_DIFF);

        RECTF boundingRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnGetBoundingRectf(region, &boundingRect);

        REQUIRE(boundingRect.left == 0.0f);
        REQUIRE(boundingRect.bottom == 0.0f);
        REQUIRE(boundingRect.right == 0.0f);
        REQUIRE(boundingRect.top == 0.0f);

        SRgnDelete(region);
    }

    SECTION("combines the region with multiple given rects 3") {
        HSRGN region;
        SRgnCreate(&region, 0);

        RECTF baseRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        RECTF newRect = { 0.0f, 1.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &newRect, nullptr, SRGN_DIFF);

        RECTF boundingRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnGetBoundingRectf(region, &boundingRect);

        REQUIRE(boundingRect.left == 0.0f);
        REQUIRE(boundingRect.bottom == 0.0f);
        REQUIRE(boundingRect.right == 1.0f);
        REQUIRE(boundingRect.top == 1.0f);

        SRgnDelete(region);
    }
}

TEST_CASE("SRgnGetRectsf", "[region]") {
    SECTION("retrieves empty list if nothing put in") {
        HSRGN rgn;
        SRgnCreate(&rgn, 0);

        uint32_t numrects = 1;
        RECTF buffer[1];

        SRgnGetRectsf(rgn, &numrects, buffer);

        CHECK(numrects == 0);

        SRgnDelete(rgn);
    }

    SECTION("retrieves all rects that were put in") {
        HSRGN rgn;
        SRgnCreate(&rgn, 0);

        RECTF rct1 = { 5, 5, 10, 10 };
        RECTF rct2 = { 0, 0, 1, 1 };
        SRgnCombineRectf(rgn, &rct1, nullptr, SRGN_OR);
        SRgnCombineRectf(rgn, &rct2, nullptr, SRGN_OR);

        uint32_t numrects = 0;
        RECTF buffer[2];
        SRgnGetRectsf(rgn, &numrects, nullptr);

        REQUIRE(numrects == 2);
        SRgnGetRectsf(rgn, &numrects, buffer);

        CHECK(buffer[0].left == rct2.left);
        CHECK(buffer[0].bottom == rct2.bottom);
        CHECK(buffer[0].right == rct2.right);
        CHECK(buffer[0].top == rct2.top);

        CHECK(buffer[1].left == rct1.left);
        CHECK(buffer[1].bottom == rct1.bottom);
        CHECK(buffer[1].right == rct1.right);
        CHECK(buffer[1].top == rct1.top);

        SRgnDelete(rgn);
    }

    SECTION("automatically merges overlapping rects") {
        HSRGN rgn;
        SRgnCreate(&rgn, 0);

        RECTF rct1 = { -10, -10, 10, 10 };
        RECTF rct2 = { 0, 0, 1, 1 };
        SRgnCombineRectf(rgn, &rct1, nullptr, SRGN_OR);
        SRgnCombineRectf(rgn, &rct2, nullptr, SRGN_OR);

        uint32_t numrects = 0;
        RECTF buffer[1];
        SRgnGetRectsf(rgn, &numrects, nullptr);

        REQUIRE(numrects == 1);
        SRgnGetRectsf(rgn, &numrects, buffer);

        CHECK(buffer[0].left == -10);
        CHECK(buffer[0].bottom == -10);
        CHECK(buffer[0].right == 10);
        CHECK(buffer[0].top == 10);

        SRgnDelete(rgn);
    }
}
