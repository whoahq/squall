#include "Test.hpp"
#include "storm/Big.hpp"

class BigData;

// Fixture for repetitive handling of BigData objects.
struct BigDataTest {
    using BigDataPtr = BigData*;

    BigData* num;

    BigDataTest() { SBigNew(&num); }
    ~BigDataTest() { SBigDel(num); }

    BigData** operator &() { return &num; }
    operator BigDataPtr() const { return num; }
    BigData* operator->() const { return num; }
};
