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
        // SBigToUnsigned is not available in WoW so do the next best thing
        uint32_t result = 0;
        auto vec = this->ToBinaryBuffer();
        for (auto i = vec.rbegin(); i != vec.rend(); i++) {
            result *= 256;
            result += *i;
        }
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
