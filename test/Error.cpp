#include "test/Test.hpp"
#include "storm/Error.hpp"

TEST_CASE("SErrDisplayError", "[error]") {
#if !defined(WHOA_TEST_WOWREAL)
    SECTION("does nothing if errors are suppressed") {
        uint32_t errorcode = GENERATE(0, STORM_ERROR_ASSERTION, ERROR_INVALID_PARAMETER);
        int32_t recoverable = GENERATE(0, 1);
        uint32_t exitcode = GENERATE(0, 1);

        SErrSuppressErrors(1);
        CHECK_FALSE(SErrDisplayError(errorcode, nullptr, 0, nullptr, recoverable, exitcode));
    }
#endif
}

TEST_CASE("SErrDisplayErrorFmt", "[error]") {
#if !defined(WHOA_TEST_WOWREAL)
    SECTION("does nothing if errors are suppressed") {
        uint32_t errorcode = GENERATE(0, STORM_ERROR_ASSERTION, ERROR_INVALID_PARAMETER);
        int32_t recoverable = GENERATE(0, 1);
        uint32_t exitcode = GENERATE(0, 1);

        SErrSuppressErrors(1);
        CHECK_FALSE(SErrDisplayErrorFmt(errorcode, nullptr, 0, recoverable, exitcode, ""));
    }
#endif
}

TEST_CASE("SErrIsDisplayingError", "[error]") {
    SECTION("returns false by default") {
        CHECK_FALSE(SErrIsDisplayingError());
    }
}
