#ifndef STORM_HASH_TS_HASH_TABLE_HPP
#define STORM_HASH_TS_HASH_TABLE_HPP

#include "storm/Array.hpp"
#include "storm/List.hpp"
#include "storm/String.hpp"

template <class T, class TKey>
class TSHashTable {
    public:
    // Virtual member functions
    virtual ~TSHashTable();
    virtual void Destroy();

    // Member functions
    void Clear();
    void Delete(T* ptr);
    T* Head();
    void Insert(T* ptr, uint32_t hashval, const TKey& key);
    T* New(const char* str, size_t extrabytes, uint32_t flags);
    T* New(uint32_t hashval, const char* str, size_t extrabytes, uint32_t flags);
    T* New(uint32_t hashval, const TKey& key, size_t extrabytes, uint32_t flags);
    T* Next(const T* ptr);
    T* Ptr(const char* str);
    T* Ptr(uint32_t hashval, const TKey& key);
    void Unlink(T* ptr);

    protected:
    // Member variables
    STORM_EXPLICIT_LIST(T, m_linktofull) m_fulllist;

    // Member functions
    int32_t GetLinkOffset();

    private:
    // Member variables
    uint32_t m_fullnessIndicator = 0;
    TSGrowableArray<STORM_EXPLICIT_LIST(T, m_linktoslot)> m_slotlistarray;
    uint32_t m_slotmask = -1;

    // Virtual member functions
    virtual void InternalDelete(T* ptr);
    virtual T* InternalNew(STORM_EXPLICIT_LIST(T, m_linktoslot)* listptr, size_t extrabytes, uint32_t flags);

    // Member functions
    uint32_t ComputeSlot(uint32_t hashval);
    void GrowListArray(uint32_t newarraysize);
    void Initialize();
    bool Initialized();
    void InternalClear(int32_t warn);
    void InternalLinkNode(T* ptr, uint32_t hashval);
    T* InternalNewNode(uint32_t, size_t extrabytes, uint32_t flags);
    int32_t MonitorFullness(uint32_t slot);
};

template <class T, class TKey>
TSHashTable<T, TKey>::~TSHashTable() {
    this->InternalClear(1);
}

template <class T, class TKey>
void TSHashTable<T, TKey>::Clear() {
    this->InternalClear(0);
}

template <class T, class TKey>
void TSHashTable<T, TKey>::Delete(T* ptr) {
    this->Unlink(ptr);
    this->InternalDelete(ptr);
}

template <class T, class TKey>
void TSHashTable<T, TKey>::Destroy() {
    this->InternalClear(1);
    this->m_fullnessIndicator = 0;
    this->m_slotmask = -1;
    this->m_slotlistarray.Clear();
}

template <class T, class TKey>
uint32_t TSHashTable<T, TKey>::ComputeSlot(uint32_t hashval) {
    return hashval & this->m_slotmask;
}

template <class T, class TKey>
int32_t TSHashTable<T, TKey>::GetLinkOffset() {
    return offsetof(T, m_linktoslot);
}

template <class T, class TKey>
void TSHashTable<T, TKey>::GrowListArray(uint32_t newarraysize) {
    uint32_t slotmask = this->m_slotmask + 1;
    int32_t linkOffset = this->GetLinkOffset();

    TSExplicitList<T, 0xDDDDDDDD> templist;
    templist.ChangeLinkOffset(linkOffset);
    for (uint32_t i = 0; i < slotmask; i++) {
        while (T* ptr = this->m_slotlistarray[i].Head()) {
            templist.LinkToTail(ptr);
        }
    }

    this->m_slotlistarray.SetCount(newarraysize);
    for (uint32_t i = 0; i < newarraysize; i++) {
        this->m_slotlistarray[i].ChangeLinkOffset(linkOffset);
    }

    this->m_slotmask = newarraysize - 1;
    while (T* ptr = templist.Head()) {
        auto& slot = this->m_slotlistarray[this->ComputeSlot(ptr->m_hashval)];
        slot.LinkToTail(ptr);
    }
}

template <class T, class TKey>
T* TSHashTable<T, TKey>::Head() {
    return this->m_fulllist.Head();
}

template <class T, class TKey>
void TSHashTable<T, TKey>::Initialize() {
    this->m_slotmask = 3;
    this->m_slotlistarray.SetCount(this->m_slotmask + 1);

    auto linkOffset = this->GetLinkOffset();

    for (uint32_t slot = 0; slot <= this->m_slotmask; slot++) {
        this->m_slotlistarray[slot].ChangeLinkOffset(linkOffset);
    }
}

template <class T, class TKey>
bool TSHashTable<T, TKey>::Initialized() {
    return this->m_slotmask != -1;
}

template <class T, class TKey>
void TSHashTable<T, TKey>::Insert(T* ptr, uint32_t hashval, const TKey& key) {
    this->InternalLinkNode(ptr, hashval);
    ptr->m_hashval = hashval;
    ptr->m_key = key;
}

template <class T, class TKey>
void TSHashTable<T, TKey>::InternalClear(int32_t warn) {
    this->m_fullnessIndicator = 0;

    this->m_fulllist.UnlinkAll();

    for (uint32_t i = 0; i < this->m_slotlistarray.Count(); i++) {
        auto& slotList = this->m_slotlistarray[i];

        for (auto node = slotList.Head(); node; node = slotList.Head()) {
            if (warn) {
                slotList.UnlinkNode(node);
            } else {
                this->InternalDelete(node);
            }
        }
    }
}

template <class T, class TKey>
void TSHashTable<T, TKey>::InternalDelete(T* ptr) {
    ptr->~T();
    STORM_FREE(ptr);
}

template <class T, class TKey>
void TSHashTable<T, TKey>::InternalLinkNode(T* ptr, uint32_t hashval) {
    if (!this->Initialized()) {
        this->Initialize();
    }

    uint32_t slot = this->ComputeSlot(hashval);

    if (this->MonitorFullness(slot)) {
        slot = this->ComputeSlot(hashval);
    }

    auto& slotList = this->m_slotlistarray[slot];
    slotList.LinkToTail(ptr);
    this->m_fulllist.LinkToTail(ptr);
}

template <class T, class TKey>
T* TSHashTable<T, TKey>::InternalNew(STORM_EXPLICIT_LIST(T, m_linktoslot)* listptr, size_t extrabytes, uint32_t flags) {
    return listptr->NewNode(1, extrabytes, flags);
}

template <class T, class TKey>
T* TSHashTable<T, TKey>::InternalNewNode(uint32_t hashval, size_t extrabytes, uint32_t flags) {
    if (!this->Initialized()) {
        this->Initialize();
    }

    uint32_t slot = this->ComputeSlot(hashval);

    if (this->MonitorFullness(slot)) {
        slot = this->ComputeSlot(hashval);
    }

    STORM_EXPLICIT_LIST(T, m_linktoslot)* v7 = &this->m_slotlistarray[slot];
    T* v8 = this->InternalNew(v7, extrabytes, flags);
    this->m_fulllist.LinkNode(v8, 2, 0);

    return v8;
}

template <class T, class TKey>
int32_t TSHashTable<T, TKey>::MonitorFullness(uint32_t slot) {
    if (this->m_slotmask >= 0x1FFF) {
        return 0;
    }

    if (this->m_fullnessIndicator > 3) {
        this->m_fullnessIndicator -= 3;
    }
    else {
        this->m_fullnessIndicator = 0;
    }

    for (T* ptr = this->m_slotlistarray[slot].Head(); reinterpret_cast<intptr_t>(ptr) > 0; ptr = this->m_slotlistarray[slot].RawNext(ptr)) {
        this->m_fullnessIndicator++;
        if (this->m_fullnessIndicator > 13) {
            uint32_t grow_amt = 2 * this->m_slotmask + 2;
            this->m_fullnessIndicator = 0;
            this->GrowListArray(grow_amt);
            return 1;
        }
    }
    return 0;
}

template <class T, class TKey>
T* TSHashTable<T, TKey>::New(const char* str, size_t extrabytes, uint32_t flags) {
    uint32_t hashval = SStrHashHT(str);
    return this->New(hashval, str, extrabytes, flags);
}

template <class T, class TKey>
T* TSHashTable<T, TKey>::New(uint32_t hashval, const char* str, size_t extrabytes, uint32_t flags) {
    T* ptr = this->InternalNewNode(hashval, extrabytes, flags);

    ptr->m_hashval = hashval;
    ptr->m_key = str;

    return ptr;
}

template <class T, class TKey>
T* TSHashTable<T, TKey>::New(uint32_t hashval, const TKey& key, size_t extrabytes, uint32_t flags) {
    T* ptr = this->InternalNewNode(hashval, extrabytes, flags);

    ptr->m_hashval = hashval;
    ptr->m_key = key;

    return ptr;
}

template <class T, class TKey>
T* TSHashTable<T, TKey>::Next(const T* ptr) {
    return this->m_fulllist.Next(ptr);
}

template <class T, class TKey>
T* TSHashTable<T, TKey>::Ptr(const char* str) {
    if (!this->Initialized()) {
        return nullptr;
    }

    uint32_t hashval = SStrHashHT(str);

    for (T* ptr = this->m_slotlistarray[this->ComputeSlot(hashval)].Head(); reinterpret_cast<intptr_t>(ptr) > 0; ptr = this->m_slotlistarray[this->ComputeSlot(hashval)].RawNext(ptr)) {
        if (ptr->m_hashval == hashval && ptr->m_key == str) {
            return ptr;
        }
    }
    return nullptr;
}

template <class T, class TKey>
T* TSHashTable<T, TKey>::Ptr(uint32_t hashval, const TKey& key) {
    if (!this->Initialized()) {
        return nullptr;
    }

    for (T* ptr = this->m_slotlistarray[this->ComputeSlot(hashval)].Head(); reinterpret_cast<intptr_t>(ptr) > 0; ptr = this->m_slotlistarray[this->ComputeSlot(hashval)].RawNext(ptr)) {
        if (ptr->m_hashval == hashval && ptr->m_key == key) {
            return ptr;
        }
    }
    return nullptr;
}

template <class T, class TKey>
void TSHashTable<T, TKey>::Unlink(T* ptr) {
    if (ptr->m_linktoslot.IsLinked()) {
        ptr->m_linktoslot.Unlink();
        ptr->m_linktofull.Unlink();
    }
}

#endif
