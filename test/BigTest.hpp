#include "Test.hpp"
#include "storm/Big.hpp"
#include <vector>

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

    uint32_t ToUnsigned() {
        uint32_t result;
        SBigToUnsigned(num, &result);
        return result;
    }

    std::vector<uint8_t> ToBinaryBuffer() {
        uint32_t size = 1024 * 1024;
        std::vector<uint8_t> data(size);

        uint32_t bytes = 0;
        SBigToBinaryBuffer(this->num, data.data(), size, &bytes);
        data.resize(bytes);

        return data;
    }
};
