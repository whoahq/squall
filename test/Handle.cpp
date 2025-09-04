#include "storm/Handle.hpp"
#include "test/Test.hpp"

TEST_CASE("DECLARE_STORM_HANDLE", "[handle]") {
    SECTION("declares handle") {
        DECLARE_STORM_HANDLE(HTEST);
        HTEST test;

        SUCCEED();
    }
}

TEST_CASE("DECLARE_STORM_CHILD_HANDLE", "[handle]") {
    SECTION("declares child handle") {
        DECLARE_STORM_HANDLE(HPARENT);
        DECLARE_STORM_CHILD_HANDLE(HCHILD, HPARENT);

        HPARENT parent;
        HCHILD child;

        SUCCEED();
    }
}
