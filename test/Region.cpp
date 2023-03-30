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
    SECTION("combines the region with the given rect using combine mode 2") {
        HSRGN region;
        SRgnCreate(&region, 0);

        RECTF baseRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &baseRect, nullptr, 2);

        RECTF boundingRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnGetBoundingRectf(region, &boundingRect);

        REQUIRE(boundingRect.left == 0.0f);
        REQUIRE(boundingRect.bottom == 0.0f);
        REQUIRE(boundingRect.right == 1.0f);
        REQUIRE(boundingRect.top == 1.0f);

        SRgnDelete(region);
    }

    SECTION("combines the region with the given rects using combine modes 2 and 4") {
        HSRGN region;
        SRgnCreate(&region, 0);

        RECTF baseRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &baseRect, nullptr, 2);

        RECTF newRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnCombineRectf(region, &newRect, nullptr, 4);

        RECTF boundingRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnGetBoundingRectf(region, &boundingRect);

        REQUIRE(boundingRect.left == 0.0f);
        REQUIRE(boundingRect.bottom == 0.0f);
        REQUIRE(boundingRect.right == 1.0f);
        REQUIRE(boundingRect.top == 1.0f);

        SRgnDelete(region);
    }
}
