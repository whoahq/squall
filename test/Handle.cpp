#include "storm/Handle.hpp"
#include "test/Test.hpp"

#define UNUSED(x) ((void)x)

TEST_CASE("DECLARE_STORM_HANDLE", "[handle]") {
    SECTION("declares handle") {
        DECLARE_STORM_HANDLE(HTEST);
        HTEST test;

        UNUSED(test);
        SUCCEED();
    }
}

TEST_CASE("DECLARE_STORM_CHILD_HANDLE", "[handle]") {
    SECTION("declares child handle") {
        DECLARE_STORM_HANDLE(HPARENT);
        DECLARE_STORM_CHILD_HANDLE(HCHILD, HPARENT);

        HPARENT parent;
        HCHILD child;

        UNUSED(parent);
        UNUSED(child);
        SUCCEED();
    }
}
