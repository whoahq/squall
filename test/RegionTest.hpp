#include "storm/Region.hpp"
#include "test/Test.hpp"
#include <sstream>

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
};

// Helpers for comparing RECTF structs
std::ostream& operator <<(std::ostream& os, RECTF const& value) {
    os << "{ " << value.left << ", " << value.bottom << ", " << value.right << ", " << value.top << " }";
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

RECTMatcher<RECTF> MatchesRect(RECTF arg) {
    return { arg };
}
