#include "storm/Hash.hpp"
#include "test/Test.hpp"

struct TestHashObject : TSHashObject<TestHashObject, HASHKEY_STRI> {
    uint32_t index = 0;
};

TEST_CASE("TSHashTable", "[hash]") {
    SECTION("constructs correctly") {
        TSHashTable<TestHashObject, HASHKEY_STRI> hashTable;
        REQUIRE(hashTable.Head() == nullptr);
    }
}
