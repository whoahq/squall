#include "storm/Hash.hpp"
#include "storm/Thread.hpp"
#include "test/Test.hpp"
#include <cstdint>

struct TestHashObject : TSHashObject<TestHashObject, HASHKEY_STRI> {
    uint32_t index = 255;
};

struct TestExportObject : TSHashObject<TestHashObject, HASHKEY_NONE> {
    uint32_t index = 255;
};

typedef void* TestExportObjectHandle;
typedef void* TestExportLockedHandle;

#define MAKEINTPTR(x) reinterpret_cast<void*>(static_cast<uintptr_t>(x));

TEST_CASE("HASHKEY_PTR", "[hash]") {
    SECTION("constructs correctly") {
        HASHKEY_PTR key1;
        REQUIRE(key1.GetPtr() == nullptr);

        void* ptr = MAKEINTPTR(0xDEAFBEEF);
        HASHKEY_PTR key2 = { ptr };
        REQUIRE(key2.GetPtr() == ptr);
    }
}

TEST_CASE("HASHKEY_PTR::operator=") {
    SECTION("assigns from another key") {
        void* ptr1 = MAKEINTPTR(0xDEAFBEEF);
        HASHKEY_PTR key1 = { ptr1 };
        void* ptr2 = MAKEINTPTR(0xFEEDFACE);
        HASHKEY_PTR key2 = { ptr2 };
        REQUIRE (!(key1 == key2));

        key1 = key2;
        REQUIRE(key1 == key2);
    }
}

TEST_CASE("HASHKEY_PTR::operator==") {
    SECTION("compares to another key") {
        void* ptr1 = MAKEINTPTR(0xDEAFBEEF);
        HASHKEY_PTR key1 = { ptr1 };
        HASHKEY_PTR key2 = { ptr1 };
        REQUIRE(key1 == key2);

        void* ptr2 = MAKEINTPTR(0xFEEDFACE);
        HASHKEY_PTR key3 = { ptr2 };
        REQUIRE(!(key1 == key3));
    }
}

TEST_CASE("HASHKEY_STR", "[hash]") {
    SECTION("constructs correctly") {
        HASHKEY_STR key1;
        REQUIRE(key1.GetString() == nullptr);

        HASHKEY_STR key2 = { "foo" };
        REQUIRE(SStrCmp(key2.GetString(), "foo") == 0);
    }
}

TEST_CASE("HASHKEY_STR::operator=") {
    SECTION("assigns from another string") {
        HASHKEY_STR key;
        key = "foo";
        REQUIRE(SStrCmp(key.GetString(), "foo") == 0);
    }

    SECTION("assigns from another key") {
        HASHKEY_STR key1 = { "foo" };
        HASHKEY_STR key2 = { "bar" };
        REQUIRE(!(key1 == key2));

        key1 = key2;
        REQUIRE(key1 == key2);
    }
}

TEST_CASE("HASHKEY_STR::operator==") {
    SECTION("compares to another string") {
        HASHKEY_STR key = { "foo" };
        REQUIRE(key == "foo");
        REQUIRE(!(key == "FOO"));
    }

    SECTION("compares to another key") {
        HASHKEY_STR key1 = { "foo" };
        HASHKEY_STR key2 = { "foo" };
        REQUIRE(key1 == key2);
    }
}

TEST_CASE("HASHKEY_STRI", "[hash]") {
    SECTION("constructs correctly") {
        HASHKEY_STRI key1;
        REQUIRE(key1.GetString() == nullptr);

        HASHKEY_STRI key2 = { "foo" };
        REQUIRE(SStrCmp(key2.GetString(), "foo") == 0);
    }
}

TEST_CASE("HASHKEY_STRI::operator=") {
    SECTION("assigns from another string") {
        HASHKEY_STRI key;
        key = "foo";
        REQUIRE(SStrCmp(key.GetString(), "foo") == 0);
    }

    SECTION("assigns from another key") {
        HASHKEY_STRI key1 = { "foo" };
        HASHKEY_STRI key2 = { "bar" };
        REQUIRE(!(key1 == key2));

        key1 = key2;
        REQUIRE(key1 == key2);
    }
}

TEST_CASE("HASHKEY_STRI::operator==") {
    SECTION("compares to another string") {
        HASHKEY_STRI key = { "foo" };
        REQUIRE(key == "FOO");
    }

    SECTION("compares to another key") {
        HASHKEY_STRI key1 = { "foo" };
        HASHKEY_STRI key2 = { "Foo" };
        REQUIRE(key1 == key2);
    }
}

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

TEST_CASE("TSExportTableSimpleReuse", "[hash]") {
    SECTION("constructs correctly") {
        TSExportTableSimpleReuse<TestExportObject, TestExportObjectHandle> exportTable;
        REQUIRE(exportTable.Head() == nullptr);
    }
}

TEST_CASE("TSExportTableSimpleReuse::New", "[hash]") {
    SECTION("returns a new object and handle") {
        TSExportTableSimpleReuse<TestExportObject, TestExportObjectHandle> exportTable;
        TestExportObjectHandle handle;
        auto object = exportTable.New(&handle);

        REQUIRE(handle != nullptr);
        REQUIRE(object != nullptr);
        REQUIRE(object != handle);
        REQUIRE(exportTable.Head() == object);
        REQUIRE(exportTable.Ptr(handle) == object);
    }
}

TEST_CASE("TSExportTableSimpleReuse::Delete", "[hash]") {
    SECTION("deletes object from export table") {
        TSExportTableSimpleReuse<TestExportObject, TestExportObjectHandle> exportTable;
        TestExportObjectHandle handle;
        auto object = exportTable.New(&handle);
        exportTable.Delete(object);

        REQUIRE(handle != nullptr);
        REQUIRE(object != nullptr);
        REQUIRE(object != handle);
        REQUIRE(exportTable.Head() == nullptr);
        REQUIRE(exportTable.Ptr(handle) == nullptr);
    }
}

TEST_CASE("TSExportTableSyncReuse", "[hash]") {
    SECTION("constructs correctly") {
        TSExportTableSyncReuse<TestExportObject, TestExportObjectHandle, TestExportLockedHandle, CCritSect> exportTable;
        REQUIRE(exportTable.Head() == nullptr);
    }
}

TEST_CASE("TSExportTableSyncReuse::NewLock", "[hash]") {
    SECTION("returns a new object, handle, and locked handle") {
        TSExportTableSyncReuse<TestExportObject, TestExportObjectHandle, TestExportLockedHandle, CCritSect> exportTable;
        TestExportObjectHandle handle;
        TestExportLockedHandle lockedHandle;
        auto object = exportTable.NewLock(&handle, &lockedHandle);
        exportTable.Unlock(lockedHandle);

        REQUIRE(handle != nullptr);
        REQUIRE(lockedHandle != nullptr);
        REQUIRE(object != nullptr);
        REQUIRE(object != handle);
        REQUIRE(exportTable.Head() == object);
        REQUIRE(exportTable.Ptr(handle) == object);
    }
}

TEST_CASE("TSExportTableSyncReuse::Delete", "[hash]") {
    SECTION("deletes object from export table") {
        TSExportTableSyncReuse<TestExportObject, TestExportObjectHandle, TestExportLockedHandle, CCritSect> exportTable;
        TestExportObjectHandle handle;
        TestExportLockedHandle lockedHandle;
        auto object = exportTable.NewLock(&handle, &lockedHandle);
        exportTable.Unlock(lockedHandle);
        exportTable.Delete(handle);

        REQUIRE(handle != nullptr);
        REQUIRE(object != nullptr);
        REQUIRE(object != handle);
        REQUIRE(exportTable.Head() == nullptr);
        REQUIRE(exportTable.Ptr(handle) == nullptr);
    }
}
