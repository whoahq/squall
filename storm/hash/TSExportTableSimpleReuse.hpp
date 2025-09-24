#ifndef STORM_HASH_TS_EXPORT_TABLE_SIMPLE_REUSE_HPP
#define STORM_HASH_TS_EXPORT_TABLE_SIMPLE_REUSE_HPP

#include "storm/hash/Hashkey.hpp"
#include "storm/hash/TSHashTableReuse.hpp"

template <class T, class THandle>
class TSExportTableSimpleReuse : public TSHashTableReuse<T, HASHKEY_NONE> {
    public:
    // Virtual member functions
    virtual ~TSExportTableSimpleReuse() = default;

    // Member functions
    void Delete(T* ptr);
    T* New(THandle* handlePtr);
    T* Ptr(THandle handle);

    private:
    // Member variables
    HASHKEY_NONE m_key;
    uint32_t m_sequence = ~reinterpret_cast<uintptr_t>(this) & 0xFFFFFFF;
    int32_t m_wrapped = 0;

    // Member functions
    THandle GenerateUniqueHandle();
};

template <class T, class THandle>
void TSExportTableSimpleReuse<T, THandle>::Delete(T* ptr) {
    TSHashTable<T, HASHKEY_NONE>::Delete(ptr);
}

template <class T, class THandle>
THandle TSExportTableSimpleReuse<T, THandle>::GenerateUniqueHandle() {
    while (true) {
        while (true) {
            this->m_sequence += 1;

            if (this->m_sequence) {
                break;
            }

            this->m_wrapped = 1;
        }

        if (!this->m_wrapped || !this->Ptr(reinterpret_cast<THandle>(static_cast<uintptr_t>(this->m_sequence)))) {
            break;
        }
    }

    return reinterpret_cast<THandle>(static_cast<uintptr_t>(this->m_sequence));
}

template <class T, class THandle>
T* TSExportTableSimpleReuse<T, THandle>::New(THandle* handlePtr) {
    auto handle = this->GenerateUniqueHandle();
    *handlePtr = handle;

    auto hashval = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(handle));
    return this->TSHashTable<T, HASHKEY_NONE>::New(hashval, this->m_key, 0, 0x0);
}

template <class T, class THandle>
T* TSExportTableSimpleReuse<T, THandle>::Ptr(THandle handle) {
    auto hashval = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(handle));
    return this->TSHashTable<T, HASHKEY_NONE>::Ptr(hashval, this->m_key);
}

#endif
