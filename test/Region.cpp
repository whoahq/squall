#include "RegionTest.hpp"
#include <cfloat>

static_assert(sizeof(RECT::left) == sizeof(RECTF::left), "RECT doesn't match RECTF");
static_assert(sizeof(RECT) == sizeof(RECTF), "RECT doesn't match RECTF");

TEST_CASE("SRgnClear", "[region]") {
    RgnDataTest region;

    SECTION("operates on an empty object") {
        SRgnClear(region);
        CHECK(region.GetResultRectsf().size() == 0);
    }

    SECTION("runs on an invalid pointer") {
        HSRGN inval = reinterpret_cast<HSRGN>(1234);
        CHECK_NOTHROW(SRgnClear(inval));
    }

    SECTION("clears rects out of a region object") {
        region.AddTestRectsi({{0, 0, 1, 1}});
        CHECK(region.GetResultRectsi().size() == 1);

        SRgnClear(region);

        CHECK(region.GetResultRectsi().size() == 0);
    }

    SECTION("clears params out of a region object") {
        uint32_t numParams = 1;
        void* params[1];

        RECTF baseRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &baseRect, &baseRect, SRGN_PARAMONLY);

        SRgnGetRectParamsf(region, &baseRect, &numParams, params);
        CHECK(numParams == 1);

        SRgnClear(region);

        SRgnGetRectParamsf(region, &baseRect, &numParams, params);
        CHECK(numParams == 0);
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
    RECTF baseRect = { 0.0f, 0.0f, 1.0f, 1.0f };
    RECTF testRect = { 0.5f, 0.6f, 1.8f, 1.4f };

    SECTION("does nothing with an invalid region object") {
        HSRGN inval = reinterpret_cast<HSRGN>(1234);
        SRgnCombineRectf(inval, &baseRect, nullptr, SRGN_OR);
    }

    SECTION("combines the region with a single given rect 1") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        RECTF boundingRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnGetBoundingRectf(region, &boundingRect);

        CHECK_THAT(boundingRect, MatchesRectf({ 0.0f, 0.0f, 1.0f, 1.0f }));
    }

    SECTION("combines the region with multiple given rects 1") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        RECTF newRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnCombineRectf(region, &newRect, nullptr, SRGN_DIFF);

        RECTF boundingRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnGetBoundingRectf(region, &boundingRect);

        CHECK_THAT(boundingRect, MatchesRectf({ 0.0f, 0.0f, 1.0f, 1.0f }));
    }

    SECTION("combines the region with multiple given rects 2") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        RECTF newRect = { 0.0f, 0.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &newRect, nullptr, SRGN_DIFF);

        RECTF boundingRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnGetBoundingRectf(region, &boundingRect);

        CHECK_THAT(boundingRect, MatchesRectf({ 0.0f, 0.0f, 0.0f, 0.0f }));
    }

    SECTION("combines the region with multiple given rects 3") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        RECTF newRect = { 0.0f, 1.0f, 1.0f, 1.0f };
        SRgnCombineRectf(region, &newRect, nullptr, SRGN_DIFF);

        RECTF boundingRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        SRgnGetBoundingRectf(region, &boundingRect);

        CHECK_THAT(boundingRect, MatchesRectf({ 0.0f, 0.0f, 1.0f, 1.0f }));
    }

    SECTION("OR operation combines rects") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &testRect, nullptr, SRGN_OR);

        //       ┌────────────┐
        //       |     2      |
        // ┌─────┼───┐┈┈┈┈┈┈┈┈│
        // |       1          │
        // |┈┈┈┈┈└───┼────────┘
        // │    0    │
        // └─────────┘
        auto result = region.GetResultRectsf();
        REQUIRE(result.size() == 3);
        CHECK_THAT(result[0], MatchesRectf({ 0.0f, 0.0f, 1.0f, 0.6f }));
        CHECK_THAT(result[1], MatchesRectf({ 0.0f, 0.6f, 1.8f, 1.0f }));
        CHECK_THAT(result[2], MatchesRectf({ 0.5f, 1.0f, 1.8f, 1.4f }));
    }

    SECTION("OR operation on self does nothing") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        auto result = region.GetResultRectsf();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRectf({ 0.0f, 0.0f, 1.0f, 1.0f }));
    }

    SECTION("OR operation merges vertical rects") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        RECTF rect2 = { 0.0f, 2.0f, 1.0f, 3.0f};
        RECTF rect3 = { 0.0f, 1.0f, 1.0f, 2.0f};
        SRgnCombineRectf(region, &rect3, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &rect2, nullptr, SRGN_OR);

        auto result = region.GetResultRectsf();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRectf({ 0.0f, 0.0f, 1.0f, 3.0f }));
    }

    SECTION("OR operation merges vertical rects 2") {
        RECTF rect2 = { 0.0f, 2.0f, 1.0f, 3.0f};
        RECTF rect3 = { 0.0f, 1.0f, 1.0f, 2.0f};
        SRgnCombineRectf(region, &rect3, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &rect2, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        auto result = region.GetResultRectsf();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRectf({ 0.0f, 0.0f, 1.0f, 3.0f }));
    }

    SECTION("OR operation merges horizontal rects") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        RECTF rect2 = { 2.0f, 0.0f, 3.0f, 1.0f};
        RECTF rect3 = { 1.0f, 0.0f, 2.0f, 1.0f};
        SRgnCombineRectf(region, &rect3, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &rect2, nullptr, SRGN_OR);

        auto result = region.GetResultRectsf();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRectf({ 0.0f, 0.0f, 3.0f, 1.0f }));
    }

    SECTION("OR operation merges horizontal rects 2") {
        RECTF rect2 = { 2.0f, 0.0f, 3.0f, 1.0f};
        RECTF rect3 = { 1.0f, 0.0f, 2.0f, 1.0f};
        SRgnCombineRectf(region, &rect3, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &rect2, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        auto result = region.GetResultRectsf();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRectf({ 0.0f, 0.0f, 3.0f, 1.0f }));
    }

    SECTION("AND operation intersects rects") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &testRect, nullptr, SRGN_AND);

        //       ┌────────────┐
        //       |╳╳╳╳╳╳╳╳╳╳╳╳|
        // ┌─────┼───┐╳╳╳╳╳╳╳╳│
        // |╳╳╳╳╳| 0 |╳╳╳╳╳╳╳╳│
        // |╳╳╳╳╳└───┼────────┘
        // │╳╳╳╳╳╳╳╳╳│
        // └─────────┘
        auto result = region.GetResultRectsf();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRectf({ 0.5f, 0.6f, 1.0f, 1.0f }));
    }

    SECTION("AND operation on self does nothing") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_AND);

        auto result = region.GetResultRectsf();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRectf({ 0.0f, 0.0f, 1.0f, 1.0f }));
    }

    SECTION("XOR operation takes exclusive differences of rects") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &testRect, nullptr, SRGN_XOR);

        //       ┌────────────┐
        //       |      3     |
        // ┌─────┼───┐┈┈┈┈┈┈┈┈│
        // |  1  |╳╳╳|   2    │
        // |┈┈┈┈┈└───┼────────┘
        // │    0    │
        // └─────────┘
        auto result = region.GetResultRectsf();
        REQUIRE(result.size() == 4);
        CHECK_THAT(result[0], MatchesRectf({ 0.0f, 0.0f, 1.0f, 0.6f }));
        CHECK_THAT(result[1], MatchesRectf({ 0.0f, 0.6f, 0.5f, 1.0f }));
        CHECK_THAT(result[2], MatchesRectf({ 1.0f, 0.6f, 1.8f, 1.0f }));
        CHECK_THAT(result[3], MatchesRectf({ 0.5f, 1.0f, 1.8f, 1.4f }));
    }

    SECTION("XOR operation on self erases rect") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_XOR);

        auto result = region.GetResultRectsf();
        CHECK(result.size() == 0);
    }

    SECTION("DIFF operation removes parts of rects") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &testRect, nullptr, SRGN_DIFF);

        //       ┌────────────┐
        //       |╳╳╳╳╳╳╳╳╳╳╳╳|
        // ┌─────┼╳╳╳╳╳╳╳╳╳╳╳╳│
        // |  1  |╳╳╳╳╳╳╳╳╳╳╳╳│
        // |┈┈┈┈┈└───┼────────┘
        // │    0    │
        // └─────────┘
        auto result = region.GetResultRectsf();
        REQUIRE(result.size() == 2);
        CHECK_THAT(result[0], MatchesRectf({ 0.0f, 0.0f, 1.0f, 0.6f }));
        CHECK_THAT(result[1], MatchesRectf({ 0.0f, 0.6f, 0.5f, 1.0f }));
    }

    SECTION("DIFF operation on self erases rect") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_DIFF);

        auto result = region.GetResultRectsf();
        CHECK(result.size() == 0);
    }

    SECTION("COPY operation splits intersecting rects") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &testRect, nullptr, SRGN_COPY);

        //       ┌────────────┐
        //       |╳╳╳╳╳╳╳╳╳╳╳╳|
        // ┌─────┼───┐╳╳╳╳╳╳╳╳│
        // |    1    │╳╳╳╳╳╳╳╳│
        // |┈┈┈┈┈└───┼────────┘
        // │    0    │
        // └─────────┘
        auto result = region.GetResultRectsf();
        REQUIRE(result.size() == 2);
        CHECK_THAT(result[0], MatchesRectf({ 0.0f, 0.0f, 1.0f, 0.6f }));
        CHECK_THAT(result[1], MatchesRectf({ 0.0f, 0.6f, 1.0f, 1.0f }));
    }

    SECTION("COPY operation splits intersecting rects 2") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        RECTF testRect2 = { 0.4f, 0.4f, 0.6f, 0.6f };
        SRgnCombineRectf(region, &testRect2, nullptr, SRGN_COPY);

        // ┌───────────┐
        // │     2     │
        // │┈┈┈┈┌┈┐┈┈┈┈│
        // │┈┈┈┈└┈┘┈┈1┈│
        // │     0     │
        // └───────────┘
        auto result = region.GetResultRectsf();
        REQUIRE(result.size() == 3);
        CHECK_THAT(result[0], MatchesRectf({ 0.0f, 0.0f, 1.0f, 0.4f }));
        CHECK_THAT(result[1], MatchesRectf({ 0.0f, 0.4f, 1.0f, 0.6f }));
        CHECK_THAT(result[2], MatchesRectf({ 0.0f, 0.6f, 1.0f, 1.0f }));
    }

    SECTION("COPY operation on self does nothing") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_COPY);

        auto result = region.GetResultRectsf();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRectf({ 0.0f, 0.0f, 1.0f, 1.0f }));
    }

    SECTION("operation doesn't work when operating rect width or height is <= 0") {
        int32_t combineMode = GENERATE(SRGN_OR, SRGN_XOR, SRGN_DIFF, SRGN_COPY, SRGN_PARAMONLY);
        RECTF testRects = GENERATE(
            RECTF{ 0.5f, 0.0f, 0.5f, 1.0f },
            RECTF{ 0.0f, 0.5f, 1.0f, 0.5f },
            RECTF{ 1.0f, 0.0f, 0.5f, 1.0f },
            RECTF{ 0.0f, 1.0f, 1.0f, 0.5f }
        );

        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &testRects, &testRects, combineMode);

        auto result = region.GetResultRectsf();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRectf(baseRect));

        uint32_t numParams = 1;
        SRgnGetRectParamsf(region, &testRects, &numParams, nullptr);
        CHECK(numParams == 0);

        SRgnGetRectParamsf(region, &baseRect, &numParams, nullptr);
        CHECK(numParams == 1);
    }

    SECTION("AND kills all rects when rect width or height is <= 0") {
        int32_t combineMode = SRGN_AND;
        RECTF testRects = GENERATE(
            RECTF{ 0.5f, 0.0f, 0.5f, 1.0f },
            RECTF{ 0.0f, 0.5f, 1.0f, 0.5f },
            RECTF{ 1.0f, 0.0f, 0.5f, 1.0f },
            RECTF{ 0.0f, 1.0f, 1.0f, 0.5f }
        );

        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &testRects, &testRects, combineMode);

        auto result = region.GetResultRectsf();
        CHECK(result.size() == 0);

        uint32_t numParams = 1;
        SRgnGetRectParamsf(region, &testRects, &numParams, nullptr);
        CHECK(numParams == 0);

        SRgnGetRectParamsf(region, &baseRect, &numParams, nullptr);
        CHECK(numParams == 0);
    }

    SECTION("PARAM operation doesn't influence rects") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &testRect, nullptr, SRGN_PARAMONLY);

        auto result = region.GetResultRectsf();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRectf(baseRect));
    }
}

TEST_CASE("SRgnCombineRecti", "[region]") {
    RgnDataTest region;
    RECT baseRect = { 0, 0, 10, 10 };
    RECT testRect = { 5, 6, 18, 14 };

    SECTION("does nothing with an invalid region object") {
        HSRGN inval = reinterpret_cast<HSRGN>(1234);
        SRgnCombineRecti(inval, &baseRect, nullptr, SRGN_OR);
    }

    SECTION("combines the region with a single given rect 1") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);

        RECT boundingRect = { 0, 0, 0, 0 };
        SRgnGetBoundingRecti(region, &boundingRect);

        CHECK_THAT(boundingRect, MatchesRecti({ 0, 0, 10, 10 }));
    }

    SECTION("combines the region with multiple given rects 1") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);

        RECT newRect = { 0, 0, 0, 0 };
        SRgnCombineRecti(region, &newRect, nullptr, SRGN_DIFF);

        RECT boundingRect = { 0, 0, 0, 0 };
        SRgnGetBoundingRecti(region, &boundingRect);

        CHECK_THAT(boundingRect, MatchesRecti({ 0, 0, 10, 10 }));
    }

    SECTION("combines the region with multiple given rects 2") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);

        RECT newRect = { 0, 0, 10, 10 };
        SRgnCombineRecti(region, &newRect, nullptr, SRGN_DIFF);

        RECT boundingRect = { 0, 0, 0, 0 };
        SRgnGetBoundingRecti(region, &boundingRect);

        CHECK_THAT(boundingRect, MatchesRecti({ 0, 0, 0, 0 }));
    }

    SECTION("combines the region with multiple given rects 3") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);

        RECT newRect = { 0, 1, 1, 1 };
        SRgnCombineRecti(region, &newRect, nullptr, SRGN_DIFF);

        RECT boundingRect = { 0, 0, 0, 0 };
        SRgnGetBoundingRecti(region, &boundingRect);

        CHECK_THAT(boundingRect, MatchesRecti({ 0, 0, 10, 10 }));
    }

    SECTION("OR operation combines rects") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &testRect, nullptr, SRGN_OR);

        //       ┌────────────┐
        //       |     2      |
        // ┌─────┼───┐┈┈┈┈┈┈┈┈│
        // |       1          │
        // |┈┈┈┈┈└───┼────────┘
        // │    0    │
        // └─────────┘
        auto result = region.GetResultRectsi();
        REQUIRE(result.size() == 3);
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(0, 0, 10, 6)));
        CHECK_THAT(result[1], MatchesRecti(DEP_RECT(0, 6, 18, 10)));
        CHECK_THAT(result[2], MatchesRecti(DEP_RECT(5, 10, 18, 14)));
    }

    SECTION("OR operation on self does nothing") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);

        auto result = region.GetResultRectsi();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(0, 0, 10, 10)));
    }

    SECTION("OR operation merges vertical rects") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);
        RECT rect2 = { 0, 20, 10, 30 };
        RECT rect3 = { 0, 10, 10, 20 };
        SRgnCombineRecti(region, &rect3, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &rect2, nullptr, SRGN_OR);

        auto result = region.GetResultRectsi();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(0, 0, 10, 30)));
    }

    SECTION("OR operation merges vertical rects 2") {
        RECT rect2 = { 0, 20, 10, 30 };
        RECT rect3 = { 0, 10, 10, 20 };
        SRgnCombineRecti(region, &rect3, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &rect2, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);

        auto result = region.GetResultRectsi();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(0, 0, 10, 30)));
    }

    SECTION("OR operation merges horizontal rects") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);
        RECT rect2 = { 20, 0, 30, 10 };
        RECT rect3 = { 10, 0, 20, 10 };
        SRgnCombineRecti(region, &rect3, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &rect2, nullptr, SRGN_OR);

        auto result = region.GetResultRectsi();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(0, 0, 30, 10)));
    }

    SECTION("OR operation merges horizontal rects 2") {
        RECT rect2 = { 20, 0, 30, 10 };
        RECT rect3 = { 10, 0, 20, 10 };
        SRgnCombineRecti(region, &rect3, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &rect2, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);

        auto result = region.GetResultRectsi();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(0, 0, 30, 10)));
    }

    SECTION("AND operation intersects rects") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &testRect, nullptr, SRGN_AND);

        //       ┌────────────┐
        //       |╳╳╳╳╳╳╳╳╳╳╳╳|
        // ┌─────┼───┐╳╳╳╳╳╳╳╳│
        // |╳╳╳╳╳| 0 |╳╳╳╳╳╳╳╳│
        // |╳╳╳╳╳└───┼────────┘
        // │╳╳╳╳╳╳╳╳╳│
        // └─────────┘
        auto result = region.GetResultRectsi();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(5, 6, 10, 10)));
    }

    SECTION("AND operation on self does nothing") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_AND);

        auto result = region.GetResultRectsi();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(0, 0, 10, 10)));
    }

    SECTION("XOR operation takes exclusive differences of rects") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &testRect, nullptr, SRGN_XOR);

        //       ┌────────────┐
        //       |      3     |
        // ┌─────┼───┐┈┈┈┈┈┈┈┈│
        // |  1  |╳╳╳|   2    │
        // |┈┈┈┈┈└───┼────────┘
        // │    0    │
        // └─────────┘
        auto result = region.GetResultRectsi();
        REQUIRE(result.size() == 4);
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(0, 0, 10, 6)));
        CHECK_THAT(result[1], MatchesRecti(DEP_RECT(0, 6, 5, 10)));
        CHECK_THAT(result[2], MatchesRecti(DEP_RECT(10, 6, 18, 10)));
        CHECK_THAT(result[3], MatchesRecti(DEP_RECT(5, 10, 18, 14)));
    }

    SECTION("XOR operation on self erases rect") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_XOR);

        auto result = region.GetResultRectsi();
        CHECK(result.size() == 0);
    }

    SECTION("DIFF operation removes parts of rects") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &testRect, nullptr, SRGN_DIFF);

        //       ┌────────────┐
        //       |╳╳╳╳╳╳╳╳╳╳╳╳|
        // ┌─────┼╳╳╳╳╳╳╳╳╳╳╳╳│
        // |  1  |╳╳╳╳╳╳╳╳╳╳╳╳│
        // |┈┈┈┈┈└───┼────────┘
        // │    0    │
        // └─────────┘
        auto result = region.GetResultRectsi();
        REQUIRE(result.size() == 2);
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(0, 0, 10, 6)));
        CHECK_THAT(result[1], MatchesRecti(DEP_RECT(0, 6, 5, 10)));
    }

    SECTION("DIFF operation on self erases rect") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_DIFF);

        auto result = region.GetResultRectsi();
        CHECK(result.size() == 0);
    }

    SECTION("COPY operation splits intersecting rects") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &testRect, nullptr, SRGN_COPY);

        //       ┌────────────┐
        //       |╳╳╳╳╳╳╳╳╳╳╳╳|
        // ┌─────┼───┐╳╳╳╳╳╳╳╳│
        // |    1    │╳╳╳╳╳╳╳╳│
        // |┈┈┈┈┈└───┼────────┘
        // │    0    │
        // └─────────┘
        auto result = region.GetResultRectsi();
        REQUIRE(result.size() == 2);
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(0, 0, 10, 6)));
        CHECK_THAT(result[1], MatchesRecti(DEP_RECT(0, 6, 10, 10)));
    }

    SECTION("COPY operation splits intersecting rects 2") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);
        RECT testRect2 = { 4, 4, 6, 6 };
        SRgnCombineRecti(region, &testRect2, nullptr, SRGN_COPY);

        // ┌───────────┐
        // │     2     │
        // │┈┈┈┈┌┈┐┈┈┈┈│
        // │┈┈┈┈└┈┘┈┈1┈│
        // │     0     │
        // └───────────┘
        auto result = region.GetResultRectsi();
        REQUIRE(result.size() == 3);
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(0, 0, 10, 4)));
        CHECK_THAT(result[1], MatchesRecti(DEP_RECT(0, 4, 10, 6)));
        CHECK_THAT(result[2], MatchesRecti(DEP_RECT(0, 6, 10, 10)));
    }

    SECTION("COPY operation on self does nothing") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_COPY);

        auto result = region.GetResultRectsi();
        CHECK(result.size() == 1);
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(0, 0, 10, 10)));
    }

    SECTION("operation doesn't work when operating rect width or height is <= 0") {
        int32_t combineMode = GENERATE(SRGN_OR, SRGN_XOR, SRGN_DIFF, SRGN_COPY, SRGN_PARAMONLY);
        RECT testRects = GENERATE(
            RECT{ 5, 0, 5, 10 },
            RECT{ 0, 5, 10, 5 },
            RECT{ 10, 0, 5, 10 },
            RECT{ 0, 10, 10, 5 }
        );

        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &testRects, &testRects, combineMode);

        auto result = region.GetResultRectsi();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(0, 0, 10, 10)));

        uint32_t numParams = 1;
        SRgnGetRectParamsi(region, &testRects, &numParams, nullptr);
        CHECK(numParams == 0);

        SRgnGetRectParamsi(region, &baseRect, &numParams, nullptr);
        CHECK(numParams == 1);
    }

    SECTION("AND kills all rects when rect width or height is <= 0") {
        int32_t combineMode = SRGN_AND;
        RECT testRects = GENERATE(
            RECT{ 5, 0, 5, 10 },
            RECT{ 0, 5, 10, 5 },
            RECT{ 10, 0, 5, 10 },
            RECT{ 0, 10, 10, 5 }
        );

        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &testRects, &testRects, combineMode);

        auto result = region.GetResultRectsi();
        CHECK(result.size() == 0);

        uint32_t numParams = 1;
        SRgnGetRectParamsi(region, &testRects, &numParams, nullptr);
        CHECK(numParams == 0);

        SRgnGetRectParamsi(region, &baseRect, &numParams, nullptr);
        CHECK(numParams == 0);
    }

    SECTION("PARAM operation doesn't influence rects") {
        SRgnCombineRecti(region, &baseRect, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &testRect, nullptr, SRGN_PARAMONLY);

        auto result = region.GetResultRectsi();
        REQUIRE(result.size() == 1);
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(0, 0, 10, 10)));
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
    RECTF baseRect = { -1.0f, 1.0f, 1.0f, 2.0f };

    SECTION("creates an independent copy of a region") {
        SRgnCombineRectf(region, &baseRect, nullptr, SRGN_OR);

        HSRGN newrgn = nullptr;
        SRgnDuplicate(region, &newrgn);

        REQUIRE(newrgn != nullptr);
        SRgnClear(region);

        uint32_t numrects = 1;
        RECTF buffer[1];
        SRgnGetRectsf(newrgn, &numrects, buffer);

        REQUIRE(numrects == 1);
        CHECK_THAT(buffer[0], MatchesRectf(baseRect));
    }

    SECTION("copies params") {
        SRgnCombineRectf(region, &baseRect, &baseRect, SRGN_PARAMONLY);

        uint32_t numParams = 2;
        void* buffer[2];
        SRgnGetRectParamsf(region, &baseRect, &numParams, buffer);

        CHECK(numParams == 1);
        CHECK(buffer[0] == &baseRect);

        HSRGN newrgn = nullptr;
        SRgnDuplicate(region, &newrgn);

        REQUIRE(newrgn != nullptr);
        SRgnClear(region);

        SRgnGetRectParamsf(newrgn, &baseRect, &numParams, buffer);

        CHECK(numParams == 1);
        CHECK(buffer[0] == &baseRect);
    }

    SECTION("sets handle to null when using an invalid region object") {
        HSRGN inval = reinterpret_cast<HSRGN>(1234);

        HSRGN newrgn = reinterpret_cast<HSRGN>(12345);
        SRgnDuplicate(inval, &newrgn);

        CHECK(newrgn == nullptr);
    }
}

TEST_CASE("SRgnGetBoundingRectf", "[region]") {
    RgnDataTest region;
    RECTF result = {};

    SECTION("returns null rect when there are no rects") {
        // make the result dirty so we test it being set
        result = { 123, 456, 666, 9 };

        SRgnGetBoundingRectf(region, &result);
        CHECK_THAT(result, MatchesRectf({ 0, 0, 0, 0 }));
    }

    SECTION("returns invalid rect when using an invalid region object") {
        HSRGN inval = reinterpret_cast<HSRGN>(1234);

        SRgnGetBoundingRectf(inval, &result);
        CHECK_THAT(result, MatchesRectf({ FLT_MAX, FLT_MAX, FLT_MIN, FLT_MIN }));
    }

    SECTION("returns the dimensions of 1 rect") {
        region.AddTestRectsf({ { 0, 0, 1, 1 } });

        SRgnGetBoundingRectf(region, &result);
        CHECK_THAT(result, MatchesRectf({ 0.0f, 0.0f, 1.0f, 1.0f }));
    }

    SECTION("returns the combined dimensions of multiple rects") {
        region.AddTestRectsf({
            { 0, 0, 1, 1 },
            { 0.5f, 0.5f, 1.5f, 1.5f },
        });

        SRgnGetBoundingRectf(region, &result);
        CHECK_THAT(result, MatchesRectf({ 0.0f, 0.0f, 1.5f, 1.5f }));

        region.AddTestRectsf({{ -0.5f, -0.5f, 0.5f, 0.5f }});

        SRgnGetBoundingRectf(region, &result);
        CHECK_THAT(result, MatchesRectf({ -0.5f, -0.5f, 1.5f, 1.5f }));
    }

    SECTION("excludes param rects") {
        region.AddTestRectsf({ { 0, 0, 1, 1 } });

        RECTF testRect = { 0.5f, 0.5f, 1.5f, 1.5f };
        int param1 = 0;
        SRgnCombineRectf(region, &testRect, &param1, SRGN_PARAMONLY);

        SRgnGetBoundingRectf(region, &result);
        CHECK_THAT(result, MatchesRectf({ 0.0f, 0.0f, 1.0f, 1.0f }));
    }
}

TEST_CASE("SRgnGetBoundingRecti", "[region]") {
    RgnDataTest region;
    RECT result = {};

    SECTION("returns null rect when there are no rects") {
        // make the result dirty so we test it being set
        result = { 123, 456, 666, 9 };

        SRgnGetBoundingRecti(region, &result);
        CHECK_THAT(result, MatchesRecti({ 0, 0, 0, 0 }));
    }

    SECTION("returns invalid rect when using an invalid region object") {
        HSRGN inval = reinterpret_cast<HSRGN>(1234);

        SRgnGetBoundingRecti(inval, &result);
        // Casting huge float values to int is inconsistent between compilers
        // It is also inconsistent between compile and runtime with the same compiler
        // So don't even bother comparing with a value
        CHECK_THAT(result, !MatchesRecti({ 0, 0, 0, 0 }));
    }

    SECTION("returns the dimensions of 1 rect (pass-through)") {
        region.AddTestRectsi({ { 0, 0, 10, 10 } });

        SRgnGetBoundingRecti(region, &result);
        CHECK_THAT(result, MatchesRecti({ 0, 0, 10, 10 }));
    }

    SECTION("returns the combined dimensions of multiple rects") {
        region.AddTestRectsi({
            { 0, 0, 10, 10 },
            { 5, 5, 15, 15 },
        });

        SRgnGetBoundingRecti(region, &result);
        CHECK_THAT(result, MatchesRecti({ 0, 0, 15, 15 }));

        region.AddTestRectsi({ { -5, -5, 5, 5 } });

        SRgnGetBoundingRecti(region, &result);
        CHECK_THAT(result, MatchesRecti({ -5, -5, 15, 15 }));
    }

    SECTION("excludes param rects") {
        region.AddTestRectsi({ { 0, 0, 10, 10 } });

        RECT testRect = { 5, 5, 15, 15 };
        int param1 = 0;
        SRgnCombineRecti(region, &testRect, &param1, SRGN_PARAMONLY);

        SRgnGetBoundingRecti(region, &result);
        CHECK_THAT(result, MatchesRecti({ 0, 0, 10, 10 }));
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

    SECTION("can have multiple params for same rect") {
        RECTF rect1 = { 0, 0, 1, 1 };
        RECTF queryRect = { 0, 0, 1, 1 };
        int param1 = 5;
        int param2 = 8;

        SRgnCombineRectf(region, &rect1, &param1, SRGN_PARAMONLY);
        SRgnCombineRectf(region, &rect1, &param2, SRGN_PARAMONLY);

        uint32_t numParams = 0;
        SRgnGetRectParamsf(region, &queryRect, &numParams, nullptr);
        REQUIRE(numParams == 2);

        void* buffer[2];
        SRgnGetRectParamsf(region, &queryRect, &numParams, buffer);
        CHECK(numParams == 2);
        CHECK(buffer[0] == &param1);
        CHECK(buffer[1] == &param2);
    }

    SECTION("can duplicate the same param") {
        RECTF rect1 = { 0, 0, 1, 1 };
        RECTF queryRect = { 0, 0, 1, 1 };
        int param1 = 5;

        SRgnCombineRectf(region, &rect1, &param1, SRGN_PARAMONLY);
        SRgnCombineRectf(region, &rect1, &param1, SRGN_PARAMONLY);

        uint32_t numParams = 0;
        SRgnGetRectParamsf(region, &queryRect, &numParams, nullptr);
        REQUIRE(numParams == 2);

        void* buffer[2];
        SRgnGetRectParamsf(region, &queryRect, &numParams, buffer);
        CHECK(numParams == 2);
        CHECK(buffer[0] == &param1);
        CHECK(buffer[1] == &param1);
    }

    SECTION("orders params by sequence") {
        RECTF rect1 = { 0, 0, 1, 1 };
        RECTF queryRect = { 0, 0, 1, 1 };
        int param1 = 11;
        int param2 = 10;
        int param3 = 5;
        int param4 = 55;

        SRgnCombineRectf(region, &rect1, &param1, SRGN_PARAMONLY);
        SRgnCombineRectf(region, &rect1, &param2, SRGN_PARAMONLY);
        SRgnCombineRectf(region, &rect1, &param3, SRGN_PARAMONLY);
        SRgnCombineRectf(region, &rect1, &param4, SRGN_PARAMONLY);

        uint32_t numParams = 0;
        SRgnGetRectParamsf(region, &queryRect, &numParams, nullptr);
        REQUIRE(numParams == 4);

        void* buffer[4];
        SRgnGetRectParamsf(region, &queryRect, &numParams, buffer);
        CHECK(numParams == 4);
        CHECK(buffer[0] == &param1);
        CHECK(buffer[1] == &param2);
        CHECK(buffer[2] == &param3);
        CHECK(buffer[3] == &param4);
    }
}

TEST_CASE("SRgnGetRectParamsi", "[region]") {
    RgnDataTest region;

    SECTION("retrieves empty list if nothing put in") {
        uint32_t numParams = 1;
        void* buffer[1];

        RECT rect = { 0, 0, 1, 1 };
        SRgnGetRectParamsi(region, &rect, &numParams, buffer);

        CHECK(numParams == 0);
    }

    SECTION("retrieves 0 when using an invalid region object") {
        HSRGN inval = reinterpret_cast<HSRGN>(1234);

        uint32_t numParams = 1;
        RECT rect = { 0, 0, 1, 1 };
        SRgnGetRectParamsi(inval, &rect, &numParams, nullptr);

        CHECK(numParams == 0);
    }

    SECTION("retrieves only overlapping params") {
        RECT rect1 = { 0, 0, 10, 10 };
        RECT rect2 = { 20, 20, 40, 40 };
        RECT rect3 = { 25, 25, 50, 50 };
        RECT queryRect = { 30, 30, 50, 50 };
        int param1 = 5;
        int param2 = 10;
        int param3 = 11;

        SRgnCombineRecti(region, &rect1, &param1, SRGN_PARAMONLY);
        SRgnCombineRecti(region, &rect2, &param2, SRGN_PARAMONLY);
        SRgnCombineRecti(region, &rect3, &param3, SRGN_PARAMONLY);

        uint32_t numParams = 0;
        SRgnGetRectParamsi(region, &queryRect, &numParams, nullptr);
        REQUIRE(numParams == 2);

        void* buffer[2];
        SRgnGetRectParamsi(region, &queryRect, &numParams, buffer);
        CHECK(numParams == 2);
        CHECK(buffer[0] == &param2);
        CHECK(buffer[1] == &param3);
    }

    SECTION("can have multiple params for same rect") {
        RECT rect1 = { 0, 0, 1, 1 };
        RECT queryRect = { 0, 0, 1, 1 };
        int param1 = 5;
        int param2 = 8;

        SRgnCombineRecti(region, &rect1, &param1, SRGN_PARAMONLY);
        SRgnCombineRecti(region, &rect1, &param2, SRGN_PARAMONLY);

        uint32_t numParams = 0;
        SRgnGetRectParamsi(region, &queryRect, &numParams, nullptr);
        REQUIRE(numParams == 2);

        void* buffer[2];
        SRgnGetRectParamsi(region, &queryRect, &numParams, buffer);
        CHECK(numParams == 2);
        CHECK(buffer[0] == &param1);
        CHECK(buffer[1] == &param2);
    }

    SECTION("can duplicate the same param") {
        RECT rect1 = { 0, 0, 1, 1 };
        RECT queryRect = { 0, 0, 1, 1 };
        int param1 = 5;

        SRgnCombineRecti(region, &rect1, &param1, SRGN_PARAMONLY);
        SRgnCombineRecti(region, &rect1, &param1, SRGN_PARAMONLY);

        uint32_t numParams = 0;
        SRgnGetRectParamsi(region, &queryRect, &numParams, nullptr);
        REQUIRE(numParams == 2);

        void* buffer[2];
        SRgnGetRectParamsi(region, &queryRect, &numParams, buffer);
        CHECK(numParams == 2);
        CHECK(buffer[0] == &param1);
        CHECK(buffer[1] == &param1);
    }

    SECTION("orders params by sequence") {
        RECT rect1 = { 0, 0, 1, 1 };
        RECT queryRect = { 0, 0, 1, 1 };
        int param1 = 11;
        int param2 = 10;
        int param3 = 5;
        int param4 = 55;

        SRgnCombineRecti(region, &rect1, &param1, SRGN_PARAMONLY);
        SRgnCombineRecti(region, &rect1, &param2, SRGN_PARAMONLY);
        SRgnCombineRecti(region, &rect1, &param3, SRGN_PARAMONLY);
        SRgnCombineRecti(region, &rect1, &param4, SRGN_PARAMONLY);

        uint32_t numParams = 0;
        SRgnGetRectParamsi(region, &queryRect, &numParams, nullptr);
        REQUIRE(numParams == 4);

        void* buffer[4];
        SRgnGetRectParamsi(region, &queryRect, &numParams, buffer);
        CHECK(numParams == 4);
        CHECK(buffer[0] == &param1);
        CHECK(buffer[1] == &param2);
        CHECK(buffer[2] == &param3);
        CHECK(buffer[3] == &param4);
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
        region.AddTestRectsf({
            { 5, 5, 10, 10 },
            { 0, 0, 1, 1 },
        });

        uint32_t numrects = 0;
        RECTF buffer[2];
        SRgnGetRectsf(region, &numrects, nullptr);

        REQUIRE(numrects == 2);
        SRgnGetRectsf(region, &numrects, buffer);

        CHECK_THAT(buffer[0], MatchesRectf({ 0, 0, 1, 1 }));
        CHECK_THAT(buffer[1], MatchesRectf({ 5, 5, 10, 10 }));
    }

    SECTION("automatically merges overlapping rects") {
        region.AddTestRectsf({
            { -10, -10, 10, 10 },
            { 0, 0, 1, 1 },
        });

        uint32_t numrects = 0;
        RECTF buffer[1];
        SRgnGetRectsf(region, &numrects, nullptr);

        REQUIRE(numrects == 1);
        SRgnGetRectsf(region, &numrects, buffer);

        CHECK_THAT(buffer[0], MatchesRectf({ -10, -10, 10, 10 }));
    }

    SECTION("retrieves rects in order of last value") {
        // The last value in WoW is top, in WC2/WC3/D1/D2/SC is bottom
        region.AddTestRectsf({
            { 0, 0, 1, 1 },
            { 4, -2, 5, 5 },
            { -2, 2, -1, 3 },
            { 2, -3, 3, -1 },
        });

        uint32_t numrects = 5;
        RECTF buffer[5];
        SRgnGetRectsf(region, &numrects, buffer);

        CHECK(numrects == 4);
        CHECK_THAT(buffer[0], MatchesRectf({ 2, -3, 3, -1 }));
        CHECK_THAT(buffer[1], MatchesRectf({ 0, 0, 1, 1 }));
        CHECK_THAT(buffer[2], MatchesRectf({ -2, 2, -1, 3 }));
        CHECK_THAT(buffer[3], MatchesRectf({ 4, -2, 5, 5 }));
    }

    SECTION("retrieves rects in order of left value when last are equal") {
        region.AddTestRectsf({
            { 0, 0, 1, 1 },
            { 4, 0, 5, 1 },
            { -2, 0, -1, 1 },
            { 2, 0, 3, 1 },
        });

        uint32_t numrects = 5;
        RECTF buffer[5];
        SRgnGetRectsf(region, &numrects, buffer);

        CHECK(numrects == 4);
        CHECK_THAT(buffer[0], MatchesRectf({ -2, 0, -1, 1 }));
        CHECK_THAT(buffer[1], MatchesRectf({ 0, 0, 1, 1 }));
        CHECK_THAT(buffer[2], MatchesRectf({ 2, 0, 3, 1 }));
        CHECK_THAT(buffer[3], MatchesRectf({ 4, 0, 5, 1 }));
    }
}

TEST_CASE("SRgnGetRectsi", "[region]") {
    RgnDataTest region;

    SECTION("retrieves empty list if nothing put in") {
        uint32_t numrects = 1;
        RECT buffer[1];

        SRgnGetRectsi(region, &numrects, buffer);

        CHECK(numrects == 0);
    }

    SECTION("retrieves 0 when using an invalid region object") {
        HSRGN inval = reinterpret_cast<HSRGN>(1234);

        uint32_t numrects = 1;
        SRgnGetRectsi(inval, &numrects, nullptr);

        CHECK(numrects == 0);
    }

    SECTION("retrieves all rects that were put in") {
        region.AddTestRectsi({
            { 5, 5, 10, 10 },
            { 0, 0, 1, 1 },
        });

        uint32_t numrects = 0;
        RECT buffer[2];
        SRgnGetRectsi(region, &numrects, nullptr);

        REQUIRE(numrects == 2);
        SRgnGetRectsi(region, &numrects, buffer);

        CHECK_THAT(buffer[0], MatchesRecti(DEP_RECT(0, 0, 1, 1)));
        CHECK_THAT(buffer[1], MatchesRecti(DEP_RECT(5, 5, 10, 10)));
    }

    SECTION("automatically merges overlapping rects") {
        region.AddTestRectsi({
            { -10, -10, 10, 10 },
            { 0, 0, 1, 1 },
        });

        uint32_t numrects = 0;
        RECT buffer[1];
        SRgnGetRectsi(region, &numrects, nullptr);

        REQUIRE(numrects == 1);
        SRgnGetRectsi(region, &numrects, buffer);

        CHECK_THAT(buffer[0], MatchesRecti(DEP_RECT(-10, -10, 10, 10)));
    }

    SECTION("retrieves rects in order of last value") {
        // Last value in WoW is top, in WC2/WC3/D1/D2/SC is bottom
        region.AddTestRectsi({
            { 0, 0, 1, 1 },
            { 4, -2, 5, 5 },
            { -2, 2, -1, 3 },
            { 2, -3, 3, -1 },
        });

        uint32_t numrects = 5;
        RECT buffer[5];
        SRgnGetRectsi(region, &numrects, buffer);

        CHECK(numrects == 4);
        CHECK_THAT(buffer[0], MatchesRecti(DEP_RECT(2, -3, 3, -1)));
        CHECK_THAT(buffer[1], MatchesRecti(DEP_RECT(0, 0, 1, 1)));
        CHECK_THAT(buffer[2], MatchesRecti(DEP_RECT(-2, 2, -1, 3)));
        CHECK_THAT(buffer[3], MatchesRecti(DEP_RECT(4, -2, 5, 5)));
    }

    SECTION("retrieves rects in order of left value when last are equal") {
        region.AddTestRectsi({
            { 0, 0, 10, 10 },
            { 40, 0, 50, 10 },
            { -20, 0, -10, 10 },
            { 20, 0, 30, 10 },
        });

        uint32_t numrects = 5;
        RECT buffer[5];
        SRgnGetRectsi(region, &numrects, buffer);

        CHECK(numrects == 4);
        CHECK_THAT(buffer[0], MatchesRecti(DEP_RECT(-20, 0, -10, 10)));
        CHECK_THAT(buffer[1], MatchesRecti(DEP_RECT(0, 0, 10, 10)));
        CHECK_THAT(buffer[2], MatchesRecti(DEP_RECT(20, 0, 30, 10)));
        CHECK_THAT(buffer[3], MatchesRecti(DEP_RECT(40, 0, 50, 10)));
    }

    SECTION("truncates floating point values") {
        region.AddTestRectsf({
            { 5.9f, 5.1f, 10.6f, 10.6f },
            { 0.6f, 0.2f, 1.9f, 1.9f },
        });

        uint32_t numrects = 0;
        RECT buffer[2];
        SRgnGetRectsi(region, &numrects, nullptr);

        REQUIRE(numrects == 2);
        SRgnGetRectsi(region, &numrects, buffer);

        CHECK_THAT(buffer[0], MatchesRecti(DEP_RECT(0, 0, 1, 1)));
        CHECK_THAT(buffer[1], MatchesRecti(DEP_RECT(5, 5, 10, 10)));
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
        region.AddTestRectsf({
            { -10, -10, -1, -1 },
            { 1, 1, 5, 5 }
        });

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
        region.AddTestRectsf({{ 1, 1, 5, 5 }});

        CHECK_FALSE(SRgnIsPointInRegionf(region, 5.0f, 5.0f));
        CHECK_FALSE(SRgnIsPointInRegionf(region, 5.0f, 4.0f));
        CHECK_FALSE(SRgnIsPointInRegionf(region, 4.0f, 5.0f));
        CHECK(SRgnIsPointInRegionf(region, 4.9999f, 4.9999f));
        CHECK(SRgnIsPointInRegionf(region, 1.0f, 1.0f));
        CHECK_FALSE(SRgnIsPointInRegionf(region, 0.9999f, 1.0f));
    }
}

TEST_CASE("SRgnIsPointInRegioni", "[region]") {
    RgnDataTest region;

    SECTION("false if region has no rects") {
        auto x = GENERATE(0, 1, INT32_MAX);
        auto y = GENERATE(0, -1, INT32_MIN);

        CHECK_FALSE(SRgnIsPointInRegioni(region, x, y));
    }

    SECTION("false if using an invalid region object") {
        HSRGN inval = reinterpret_cast<HSRGN>(1234);

        CHECK_FALSE(SRgnIsPointInRegioni(inval, 0, 0));
    }

    SECTION("reports if points are inside a region") {
        region.AddTestRectsi({
            { -10, -10, -1, -1 },
            { 1, 1, 5, 5 }
        });

        CHECK_FALSE(SRgnIsPointInRegioni(region, 0, 0));
        CHECK_FALSE(SRgnIsPointInRegioni(region, -11, -5));
        CHECK_FALSE(SRgnIsPointInRegioni(region, 4, 6));
        CHECK_FALSE(SRgnIsPointInRegioni(region, 1, 0));
        CHECK_FALSE(SRgnIsPointInRegioni(region, 0, -1));
        CHECK(SRgnIsPointInRegioni(region, -10, -10));
        CHECK(SRgnIsPointInRegioni(region, -2, -2));
        CHECK(SRgnIsPointInRegioni(region, 3, 2));
    }

    SECTION("ignores param only rects") {
        RECT rect = { -1, -1, 1, 1 };
        SRgnCombineRecti(region, &rect, &rect, SRGN_PARAMONLY);

        CHECK_FALSE(SRgnIsPointInRegioni(region, 0, 0));
    }

    SECTION("excludes upper bounds of rect") {
        region.AddTestRectsi({{ 10, 10, 50, 50 }});

        CHECK_FALSE(SRgnIsPointInRegioni(region, 50, 50));
        CHECK_FALSE(SRgnIsPointInRegioni(region, 50, 40));
        CHECK_FALSE(SRgnIsPointInRegioni(region, 40, 50));
        CHECK(SRgnIsPointInRegioni(region, 49, 49));
        CHECK(SRgnIsPointInRegioni(region, 10, 10));
        CHECK_FALSE(SRgnIsPointInRegioni(region, 9, 10));
    }
}

TEST_CASE("SRgnIsRectInRegionf", "[region]") {
    RgnDataTest region;
    RECTF rect = { 0.0f, 0.0f, 1.0f, 1.0f };

    SECTION("false if region has no rects") {
        CHECK_FALSE(SRgnIsRectInRegionf(region, &rect));
    }

    SECTION("false if using an invalid region object") {
        HSRGN inval = reinterpret_cast<HSRGN>(1234);
        CHECK_FALSE(SRgnIsRectInRegionf(inval, &rect));
    }

    SECTION("true if rects overlap a region") {
        RECTF checkRect = GENERATE(
            RECTF{ 0.5f, 0.5f, 0.5f, 0.5f },
            RECTF{ 0.00001f, 0.00001f, 0.999999f, 0.999999f },
            RECTF{ 11.0f, 11.0f, 12.0f, 19.0f },
            RECTF{ 14.0f, 14.0f, 20.0f, 20.0f },
            RECTF{ -100.0f, -100.0f, 0.000001f, 0.0000001f }
        );

        RECTF secondRect = { 10.0f, 10.0f, 15.0f, 20.0f };
        SRgnCombineRectf(region, &rect, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &secondRect, nullptr, SRGN_OR);

        CHECK(SRgnIsRectInRegionf(region, &checkRect));
    }

    SECTION("false if rects are outside a region") {
        RECTF checkRect = GENERATE(
            RECTF{ 2.0f, 2.0f, 2.0f, 2.0f },
            RECTF{ 1.0f, 1.0f, 2.0f, 2.0f },
            RECTF{ -1.0f, -1.0f, 0.0f, 0.0f },
            RECTF{ 9.0f, 9.0f, 10.0f, 10.0f }
        );

        RECTF secondRect = { 10.0f, 10.0f, 15.0f, 20.0f };
        SRgnCombineRectf(region, &rect, nullptr, SRGN_OR);
        SRgnCombineRectf(region, &secondRect, nullptr, SRGN_OR);

        CHECK_FALSE(SRgnIsRectInRegionf(region, &checkRect));
    }

    SECTION("ignores param only rects") {
        SRgnCombineRectf(region, &rect, &rect, SRGN_PARAMONLY);
        CHECK_FALSE(SRgnIsRectInRegionf(region, &rect));
    }

    SECTION("excludes matching on rect bounds") {
        RECTF checkRect = GENERATE(
            RECTF{ -0.1f, -0.1f, 0.0f, 1.0f },
            RECTF{ -0.1f, -0.1f, 1.0f, 0.0f },
            RECTF{ 1.0f, 1.0f, 1.0f, 1.0f },
            RECTF{ 0.0f, 0.0f, 0.0f, 0.0f },
            RECTF{ 0.5f, 0.0f, 0.5f, 0.0f },
            RECTF{ 0.0f, 0.5f, 0.0f, 0.5f }
        );
        SRgnCombineRectf(region, &rect, nullptr, SRGN_OR);

        CHECK_FALSE(SRgnIsRectInRegionf(region, &checkRect));
    }
}

TEST_CASE("SRgnIsRectInRegioni", "[region]") {
    RgnDataTest region;
    RECT rect = { 0, 0, 10, 10 };

    SECTION("false if region has no rects") {
        CHECK_FALSE(SRgnIsRectInRegioni(region, &rect));
    }

    SECTION("false if using an invalid region object") {
        HSRGN inval = reinterpret_cast<HSRGN>(1234);
        CHECK_FALSE(SRgnIsRectInRegioni(inval, &rect));
    }

    SECTION("true if rects overlap a region") {
        RECT checkRect = GENERATE(
            RECT{ 5, 5, 5, 5 },
            RECT{ 0, 0, 10, 10 },
            RECT{ 110, 110, 120, 190 },
            RECT{ 140, 140, 200, 200 },
            RECT{ -1000, -1000, 1, 1 }
        );

        RECT secondRect = { 100, 100, 150, 200 };
        SRgnCombineRecti(region, &rect, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &secondRect, nullptr, SRGN_OR);

        CHECK(SRgnIsRectInRegioni(region, &checkRect));
    }

    SECTION("false if rects are outside a region") {
        RECT checkRect = GENERATE(
            RECT{ 20, 20, 20, 20 },
            RECT{ 10, 10, 20, 20 },
            RECT{ -10, -10, 0, 0 },
            RECT{ 90, 90, 100, 100 }
        );

        RECT secondRect = { 100, 100, 150, 200 };
        SRgnCombineRecti(region, &rect, nullptr, SRGN_OR);
        SRgnCombineRecti(region, &secondRect, nullptr, SRGN_OR);

        CHECK_FALSE(SRgnIsRectInRegioni(region, &checkRect));
    }

    SECTION("ignores param only rects") {
        SRgnCombineRecti(region, &rect, &rect, SRGN_PARAMONLY);
        CHECK_FALSE(SRgnIsRectInRegioni(region, &rect));
    }

    SECTION("excludes matching on rect bounds") {
        RECT checkRect = GENERATE(
            RECT{ -1, -1, 0, 10 },
            RECT{ -1, -1, 10, 0 },
            RECT{ 10, 10, 10, 10 },
            RECT{ 0, 0, 0, 0 },
            RECT{ 5, 0, 5, 0 },
            RECT{ 0, 5, 0, 5 }
        );
        SRgnCombineRecti(region, &rect, nullptr, SRGN_OR);

        CHECK_FALSE(SRgnIsRectInRegioni(region, &checkRect));
    }
}

TEST_CASE("SRgnOffsetf", "[region]") {
    RgnDataTest region;

    SECTION("does nothing when using an invalid region object") {
        // doesn't crash
        HSRGN inval = reinterpret_cast<HSRGN>(1234);
        SRgnOffsetf(inval, 10.0f, 10.0f);
    }

    SECTION("shifts rects by given amount") {
        region.AddTestRectsf({
            { -200.0f, -200.0f, -100.0f, -100.0f },
            { 0.0f, 0.0f, 100.0f, 100.0f }
        });

        SRgnOffsetf(region, 100.0f, 50.0f);

        auto result = region.GetResultRectsf();
        CHECK_THAT(result[0], MatchesRectf({ -100.0f, -150.0f, 0.0f, -50.0f }));
        CHECK_THAT(result[1], MatchesRectf({ 100.0f, 50.0f, 200.0f, 150.0f }));
    }

    SECTION("shifts rects back to their original positions with opposite amounts") {
        region.AddTestRectsf({
            { -200.0f, -200.0f, -100.0f, -100.0f },
            { 0.0f, 0.0f, 100.0f, 100.0f }
        });

        SRgnOffsetf(region, 100.0f, 50.0f);
        SRgnOffsetf(region, 5.0f, 10.0f);
        SRgnOffsetf(region, -105.0f, -60.0f);

        auto result = region.GetResultRectsf();
        CHECK_THAT(result[0], MatchesRectf({ -200.0f, -200.0f, -100.0f, -100.0f }));
        CHECK_THAT(result[1], MatchesRectf({ 0.0f, 0.0f, 100.0f, 100.0f }));
    }

    SECTION("doesn't shift anything with 0") {
        region.AddTestRectsf({
            { -200.0f, -200.0f, -100.0f, -100.0f },
            { 0.0f, 0.0f, 100.0f, 100.0f }
        });

        SRgnOffsetf(region, 0.0f, 0.0f);

        auto result = region.GetResultRectsf();
        CHECK_THAT(result[0], MatchesRectf({ -200.0f, -200.0f, -100.0f, -100.0f }));
        CHECK_THAT(result[1], MatchesRectf({ 0.0f, 0.0f, 100.0f, 100.0f }));
    }
}

TEST_CASE("SRgnOffseti", "[region]") {
    RgnDataTest region;

    SECTION("does nothing when using an invalid region object") {
        // doesn't crash
        HSRGN inval = reinterpret_cast<HSRGN>(1234);
        SRgnOffseti(inval, 100, 100);
    }

    SECTION("shifts rects by given amount") {
        region.AddTestRectsi({
            { -2000, -2000, -1000, -1000 },
            { 0, 0, 1000, 1000 }
        });

        SRgnOffseti(region, 1000, 500);

        auto result = region.GetResultRectsi();
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(-1000, -1500, 0, -500)));
        CHECK_THAT(result[1], MatchesRecti(DEP_RECT(1000, 500, 2000, 1500)));
    }

    SECTION("shifts rects back to their original positions with opposite amounts") {
        region.AddTestRectsi({
            { -2000, -2000, -1000, -1000 },
            { 0, 0, 1000, 1000 }
        });

        SRgnOffseti(region, 1000, 500);
        SRgnOffseti(region, 50, 100);
        SRgnOffseti(region, -1050, -600);

        auto result = region.GetResultRectsi();
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(-2000, -2000, -1000, -1000)));
        CHECK_THAT(result[1], MatchesRecti(DEP_RECT(0, 0, 1000, 1000)));
    }

    SECTION("doesn't shift anything with 0") {
        region.AddTestRectsi({
            { -2000, -2000, -1000, -1000 },
            { 0, 0, 1000, 1000 }
        });

        SRgnOffseti(region, 0, 0);

        auto result = region.GetResultRectsi();
        CHECK_THAT(result[0], MatchesRecti(DEP_RECT(-2000, -2000, -1000, -1000)));
        CHECK_THAT(result[1], MatchesRecti(DEP_RECT(0, 0, 1000, 1000)));
    }
}
