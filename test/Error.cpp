#include "test/Test.hpp"
#include "storm/Error.hpp"

TEST_CASE("SErrDisplayError", "[event]") {
    SECTION("does nothing if errors are suppressed") {
        uint32_t errorcode = GENERATE(0, STORM_ERROR_ASSERTION, ERROR_INVALID_PARAMETER);
        int32_t recoverable = GENERATE(0, 1);
        uint32_t exitcode = GENERATE(0, 1);
        uint32_t unkarg = GENERATE(0, 1, 0x11111111);

        SErrSuppressErrors(1);
        CHECK_FALSE(SErrDisplayError(errorcode, nullptr, 0, nullptr, recoverable, exitcode, unkarg));
    }
}
