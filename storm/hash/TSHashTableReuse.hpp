#ifndef STORM_HASH_TS_HASH_TABLE_REUSE_HPP
#define STORM_HASH_TS_HASH_TABLE_REUSE_HPP

#include "storm/hash/TSHashObjectChunk.hpp"
#include "storm/hash/TSHashTable.hpp"
#include "storm/Error.hpp"
#include "storm/List.hpp"
#include <cstddef>
#include <cstdint>

template <class T, class TKey>
class TSHashTableReuse : public TSHashTable<T, TKey> {
    public:
    // Virtual member functions
    virtual ~TSHashTableReuse();
    virtual void Destroy();

    private:
    using object_chunk_t = TSHashObjectChunk<T, TKey>;

    // Member variables
    STORM_EXPLICIT_LIST(T, m_linktoslot) m_reuseList;
    uint32_t m_chunkSize = 16;
    STORM_EXPLICIT_LIST(object_chunk_t, m_link) m_chunkList;

    // Virtual member functions
    virtual void InternalDelete(T* ptr);
    virtual T* InternalNew(STORM_EXPLICIT_LIST(T, m_linktoslot)* listptr, size_t extrabytes, uint32_t flags);

    // Member functions
    void Destructor();
};

template <class T, class TKey>
TSHashTableReuse<T, TKey>::~TSHashTableReuse() {
    this->Destructor();
}

template <class T, class TKey>
void TSHashTableReuse<T, TKey>::Destroy() {
    this->Clear();
    this->Destructor();
}

template <class T, class TKey>
void TSHashTableReuse<T, TKey>::Destructor() {
    this->m_chunkList.Clear();
    this->m_reuseList.Clear();
    this->m_chunkSize = 16;
}

template <class T, class TKey>
void TSHashTableReuse<T, TKey>::InternalDelete(T* ptr) {
    this->m_fulllist.UnlinkNode(ptr);
    this->m_reuseList.LinkNode(ptr, 1, nullptr);
}

template <class T, class TKey>
T* TSHashTableReuse<T, TKey>::InternalNew(STORM_EXPLICIT_LIST(T, m_linktoslot)* listptr, size_t extrabytes, uint32_t flags) {
    STORM_ASSERT(!extrabytes);

    auto node = this->m_reuseList.Head();

    if (!node) {
        object_chunk_t* chunk;

        while (true) {
            chunk = this->m_chunkList.Head();

            if (chunk && chunk->m_array.Reserved()) {
                break;
            }

            chunk = this->m_chunkList.NewNode(1, 0, 0x0);
            chunk->m_array.Reserve(this->m_chunkSize, 0);
        }

        node = chunk->m_array.New();
    }

    listptr->LinkNode(node, 1, nullptr);

    return node;
}

#endif
