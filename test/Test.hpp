#include "vendor/catch-2.13.10/catch.hpp"

class BigData;

// Fixture for repetitive handling of BigData objects.
struct BigDataTest {
    using BigDataPtr=BigData*;

    BigData *num;

    BigDataTest();
    ~BigDataTest();

    BigData **operator &() { return &num; }
    operator BigDataPtr() const { return num; }
    BigData *operator->() const { return num; }
};
