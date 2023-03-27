#ifndef STORM_HASH_TS_EXPORT_TABLE_SYNC_REUSE_HPP
#define STORM_HASH_TS_EXPORT_TABLE_SYNC_REUSE_HPP

#include "storm/hash/Hashkey.hpp"
#include "storm/hash/TSExportTableSimpleReuse.hpp"

template <class T, class THandle, class TLockedHandle, class TSync>
class TSExportTableSyncReuse : public TSExportTableSimpleReuse<T, THandle> {
    public:
    // Virtual member functions
    virtual ~TSExportTableSyncReuse() = default;

    // Member functions
    void Delete(THandle handle);
    void DeleteUnlock(T* ptr, TLockedHandle lockedHandle);
    T* Lock(THandle handle, TLockedHandle* lockedHandlePtr, int32_t forWriting);
    T* NewLock(THandle* handlePtr, TLockedHandle* lockedHandlePtr);
    void Unlock(TLockedHandle lockedHandle);

    private:
    // Member variables
    TSync m_sync;

    // Member functions
    int32_t IsForWriting(TLockedHandle lockedHandle);
    void SyncEnterLock(TLockedHandle* lockedHandlePtr, int32_t forWriting);
    void SyncLeaveLock(TLockedHandle lockedHandle);
};

template <class T, class THandle, class TLockedHandle, class TSync>
void TSExportTableSyncReuse<T, THandle, TLockedHandle, TSync>::Delete(THandle handle) {
    TLockedHandle lockedHandle;
    auto ptr = this->Lock(handle, &lockedHandle, 1);
    this->DeleteUnlock(ptr, lockedHandle);
}

template <class T, class THandle, class TLockedHandle, class TSync>
void TSExportTableSyncReuse<T, THandle, TLockedHandle, TSync>::DeleteUnlock(T* ptr, TLockedHandle lockedHandle) {
    this->TSExportTableSimpleReuse<T, THandle>::Delete(ptr);
    this->Unlock(lockedHandle);
}

template <class T, class THandle, class TLockedHandle, class TSync>
int32_t TSExportTableSyncReuse<T, THandle, TLockedHandle, TSync>::IsForWriting(TLockedHandle lockedHandle) {
    return reinterpret_cast<intptr_t>(lockedHandle) == 1;
}

template <class T, class THandle, class TLockedHandle, class TSync>
T* TSExportTableSyncReuse<T, THandle, TLockedHandle, TSync>::Lock(THandle handle, TLockedHandle* lockedHandlePtr, int32_t forWriting) {
    this->SyncEnterLock(lockedHandlePtr, forWriting);

    auto ptr = this->Ptr(handle);

    if (!ptr) {
        this->SyncLeaveLock(*lockedHandlePtr);
        *lockedHandlePtr = nullptr;
        return nullptr;
    }

    return ptr;
}

template <class T, class THandle, class TLockedHandle, class TSync>
T* TSExportTableSyncReuse<T, THandle, TLockedHandle, TSync>::NewLock(THandle* handlePtr, TLockedHandle* lockedHandlePtr) {
    this->SyncEnterLock(lockedHandlePtr, 1);
    return this->New(handlePtr);
}

template <class T, class THandle, class TLockedHandle, class TSync>
void TSExportTableSyncReuse<T, THandle, TLockedHandle, TSync>::SyncEnterLock(TLockedHandle* lockedHandlePtr, int32_t forWriting) {
    this->m_sync.Enter(forWriting);
    *lockedHandlePtr = reinterpret_cast<TLockedHandle>(forWriting ? 1 : -1);
}

template <class T, class THandle, class TLockedHandle, class TSync>
void TSExportTableSyncReuse<T, THandle, TLockedHandle, TSync>::SyncLeaveLock(TLockedHandle lockedHandle) {
    if (lockedHandle) {
        this->m_sync.Leave(this->IsForWriting(lockedHandle));
    }
}

template <class T, class THandle, class TLockedHandle, class TSync>
void TSExportTableSyncReuse<T, THandle, TLockedHandle, TSync>::Unlock(TLockedHandle lockedHandle) {
    this->SyncLeaveLock(lockedHandle);
}

#endif
