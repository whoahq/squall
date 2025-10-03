#include "storm/Region.hpp"
#include "test/Test.hpp"
#include <sstream>
#include <initializer_list>
#include <vector>


// Flips top and bottom when not using screen coordinates.
// Avoids ifdefs everywhere inside each test.
#if defined(WHOA_RECT_USES_SCREEN_COORDINATES)
#define DEP_RECT(left, top, right, bottom) { left, top, right, bottom }
#else
#define DEP_RECT(left, top, right, bottom) { left, bottom, right, top }
#endif

// Fixture for repetitive handling of Rgn objects.
struct RgnDataTest {
    HSRGN rgn = nullptr;

    RgnDataTest() {
      SRgnCreate(&rgn, 0);
    }

    ~RgnDataTest() {
      SRgnDelete(rgn);
      rgn = nullptr;
    }

    HSRGN* operator &() { return &rgn; }
    operator HSRGN() const { return rgn; }
    HSRGN operator->() const { return rgn; }

    void AddTestRectsi(std::initializer_list<RECT> rects) {
        for (RECT rct : rects) {
            SRgnCombineRecti(this->rgn, &rct, nullptr, SRGN_OR);
        }
    }

    void AddTestRectsf(std::initializer_list<RECTF> rects) {
        for (RECTF rct : rects) {
            SRgnCombineRectf(this->rgn, &rct, nullptr, SRGN_OR);
        }
    }

    std::vector<RECT> GetResultRectsi() {
        uint32_t numRects = 0;
        SRgnGetRectsi(this->rgn, &numRects, nullptr);

        std::vector<RECT> result(numRects);
        SRgnGetRectsi(this->rgn, &numRects, result.data());

        return result;
    }

    std::vector<RECTF> GetResultRectsf() {
        uint32_t numRects = 0;
        SRgnGetRectsf(this->rgn, &numRects, nullptr);

        std::vector<RECTF> result(numRects);
        SRgnGetRectsf(this->rgn, &numRects, result.data());

        return result;
    }
};

// Helpers for comparing RECTF structs
std::ostream& operator <<(std::ostream& os, RECTF const& value) {
#if defined(WHOA_RECT_USES_SCREEN_COORDINATES)
    os << "{ " << value.left << ", " << value.top << ", " << value.right << ", " << value.bottom << " }";
#else
    os << "{ " << value.left << ", " << value.bottom << ", " << value.right << ", " << value.top << " }";
#endif
    return os;
}

// Helpers for comparing RECTF structs
std::ostream& operator <<(std::ostream& os, RECT const& value) {
    os << "{ " << value.left << ", " << value.top << ", " << value.right << ", " << value.bottom << " }";
    return os;
}

template <class T>
class RECTMatcher : public Catch::MatcherBase<T> {
    private:
    T cmp;

    public:
    RECTMatcher(T arg) : cmp(arg) {}

    bool match(T const& in) const override {
        return cmp.left == in.left && cmp.bottom == in.bottom && cmp.right == in.right && cmp.top == in.top;
    }

    std::string describe() const override {
        std::ostringstream ss;
        ss << "equals " << cmp;
        return ss.str();
    }
};

RECTMatcher<RECTF> MatchesRectf(RECTF arg) {
    return { arg };
}

RECTMatcher<RECT> MatchesRecti(RECT arg) {
    return { arg };
}
