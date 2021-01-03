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

TEST_CASE("TSHashTable::Clear", "[hash]") {
    SECTION("clears empty hash table correctly") {
        TSHashTable<TestHashObject, HASHKEY_STRI> hashTable;
        hashTable.Clear();
        REQUIRE(hashTable.Head() == nullptr);
    }

    SECTION("clears hash table with entry correctly") {
        TSHashTable<TestHashObject, HASHKEY_STRI> hashTable;
        hashTable.New("testKey", 0, 0x0);
        hashTable.Clear();
        REQUIRE(hashTable.Head() == nullptr);
    }

    SECTION("clears hash table with multiple entries correctly") {
        TSHashTable<TestHashObject, HASHKEY_STRI> hashTable;
        hashTable.New("testKey1", 0, 0x0);
        hashTable.New("testKey2", 0, 0x0);
        hashTable.Clear();
        REQUIRE(hashTable.Head() == nullptr);
    }
}
