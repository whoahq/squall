#ifndef STORM_LIST_TS_LINK_HPP
#define STORM_LIST_TS_LINK_HPP

#include <cstddef>
#include <cstdint>

template <class T>
class TSLink {
    public:
    // Member variables
    TSLink<T>* m_prevlink = nullptr;
    T* m_next = nullptr;

    // Member functions
    ~TSLink();
    bool IsLinked(void);
    T* Next(void);
    TSLink<T>* NextLink(ptrdiff_t linkoffset);
    T* Prev(void);
    T* RawNext(void);
    void Unlink(void);
};

template <class T>
TSLink<T>::~TSLink() {
    this->Unlink();
}

template <class T>
bool TSLink<T>::IsLinked() {
    return this->m_next != nullptr;
}

template <class T>
T* TSLink<T>::Next() {
    // Check for sentinel node (indicates list end)
    return reinterpret_cast<intptr_t>(this->m_next) <= 0 ? nullptr : this->m_next;
}

template <class T>
TSLink<T>* TSLink<T>::NextLink(ptrdiff_t linkoffset) {
    T* next = this->m_next;

    if (reinterpret_cast<intptr_t>(next) <= 0) {
        // End of list
        return reinterpret_cast<TSLink<T>*>(~reinterpret_cast<uintptr_t>(next));
    } else {
        ptrdiff_t offset;

        if (linkoffset < 0) {
            offset = reinterpret_cast<uintptr_t>(this) - reinterpret_cast<uintptr_t>(this->m_prevlink->m_next);
        } else {
            offset = linkoffset;
        }

        return reinterpret_cast<TSLink<T>*>(reinterpret_cast<uintptr_t>(this->m_next) + offset);
    }
}

template <class T>
T* TSLink<T>::Prev() {
    return this->m_prevlink->m_prevlink->Next();
}

template <class T>
T* TSLink<T>::RawNext() {
    return this->m_next;
}

template <class T>
void TSLink<T>::Unlink() {
    if (this->m_prevlink) {
        this->NextLink(-1)->m_prevlink = this->m_prevlink;
        this->m_prevlink->m_next = this->m_next;

        this->m_prevlink = nullptr;
        this->m_next = nullptr;
    }
}

#endif
