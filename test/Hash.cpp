#include "storm/Hash.hpp"
#include "test/Test.hpp"

struct TestHashObject : TSHashObject<TestHashObject, HASHKEY_STRI> {
    uint32_t index = 255;
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

TEST_CASE("TSHashTableReuse", "[hash]") {
    SECTION("constructs correctly") {
        TSHashTableReuse<TestHashObject, HASHKEY_STRI> hashTable;
        REQUIRE(hashTable.Head() == nullptr);
    }
}

TEST_CASE("TSHashTableReuse::New", "[hash]") {
    SECTION("allocates new object correctly") {
        TSHashTableReuse<TestHashObject, HASHKEY_STRI> hashTable;
        auto object = hashTable.New("testKey1", 0, 0x0);

        REQUIRE(object != nullptr);
        REQUIRE(object->index == 255);
    }

    SECTION("recycles memory correctly") {
        TSHashTableReuse<TestHashObject, HASHKEY_STRI> hashTable;

        auto object1 = hashTable.New("testKey1", 0, 0x0);
        auto object1Ptr = reinterpret_cast<uintptr_t>(object1);

        hashTable.Clear();

        auto object2 = hashTable.New("testKey2", 0, 0x0);
        auto object2Ptr = reinterpret_cast<uintptr_t>(object2);

        hashTable.Clear();

        auto object3 = hashTable.New("testKey3", 0, 0x0);
        auto object3Ptr = reinterpret_cast<uintptr_t>(object3);

        REQUIRE(object1Ptr == object2Ptr);
        REQUIRE(object1Ptr == object3Ptr);
        REQUIRE(hashTable.Ptr("testKey1") == nullptr);
        REQUIRE(hashTable.Ptr("testKey2") == nullptr);
        REQUIRE(hashTable.Ptr("testKey3") != nullptr);
        REQUIRE(hashTable.Ptr("testKey3")->index == 255);
    }
}
