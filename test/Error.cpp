#include "test/Test.hpp"
#include "storm/Error.hpp"

TEST_CASE("SErrDisplayError", "[error]") {
    SECTION("does nothing if errors are suppressed") {
        uint32_t errorcode = GENERATE(0, STORM_ERROR_ASSERTION, ERROR_INVALID_PARAMETER);
        int32_t recoverable = GENERATE(0, 1);
        uint32_t exitcode = GENERATE(0, 1);

        SErrSuppressErrors(1);
        CHECK_FALSE(SErrDisplayError(errorcode, nullptr, 0, nullptr, recoverable, exitcode));
    }
}

TEST_CASE("SErrDisplayErrorFmt", "[error]") {
    SECTION("does nothing if errors are suppressed") {
        uint32_t errorcode = GENERATE(0, STORM_ERROR_ASSERTION, ERROR_INVALID_PARAMETER);
        int32_t recoverable = GENERATE(0, 1);
        uint32_t exitcode = GENERATE(0, 1);

        SErrSuppressErrors(1);
        CHECK_FALSE(SErrDisplayErrorFmt(errorcode, nullptr, 0, recoverable, exitcode, ""));
    }
}

TEST_CASE("SErrGetErrorStr", "[error]") {
    SECTION("clears result buffer") {
        // Use a fake error to almost guarantee it won't translate
        char buffer[32] = "memes";
        CHECK(SErrGetErrorStr(0x12345678, buffer, sizeof(buffer)) == 0);
        CHECK(std::string(buffer) == "");
    }
}

TEST_CASE("SErrIsDisplayingError", "[error]") {
    SECTION("returns false by default") {
        CHECK_FALSE(SErrIsDisplayingError());
    }
}
