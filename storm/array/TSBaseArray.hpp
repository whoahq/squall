#ifndef STORM_ARRAY_TS_BASE_ARRAY_HPP
#define STORM_ARRAY_TS_BASE_ARRAY_HPP

#include "storm/Error.hpp"
#include <cstdint>
#include <typeinfo>

template <class T>
class TSBaseArray {
    public:
    uint32_t m_alloc = 0;
    uint32_t m_count = 0;
    T* m_data = nullptr;

    virtual const char* MemFileName() const;
    virtual int32_t MemLineNo() const;

    T& operator[](uint32_t index);
    void CheckArrayBounds(uint32_t index) const;
    uint32_t Count() const;
    void Clear();
    T* Ptr();
    const T* Ptr() const;
    T* Top();
};

template <class T>
T& TSBaseArray<T>::operator[](uint32_t index) {
    this->CheckArrayBounds(index);
    return this->m_data[index];
}

template <class T>
void TSBaseArray<T>::CheckArrayBounds(uint32_t index) const {
    if (index < this->Count()) {
        return;
    }

    SErrDisplayErrorFmt(
        0x85100080,
        this->MemFileName(),
        this->MemLineNo(),
        1,
        1,
        "index (0x%08X), array size (0x%08X)",
        index,
        this->Count());
}

template <class T>
uint32_t TSBaseArray<T>::Count() const {
    return this->m_count;
}

template <class T>
void TSBaseArray<T>::Clear() {
    delete[] this->m_data;
    TSBaseArray<T>();
}

template <class T>
const char* TSBaseArray<T>::MemFileName() const {
    return typeid(T).name();
}

template <class T>
int32_t TSBaseArray<T>::MemLineNo() const {
    return -2;
}

template <class T>
T* TSBaseArray<T>::Ptr() {
    return this->m_data;
}

template <class T>
const T* TSBaseArray<T>::Ptr() const {
    return this->m_data;
}

template <class T>
T* TSBaseArray<T>::Top() {
    if (this->m_count == 0) {
        return nullptr;
    }

    return &this->m_data[this->m_count - 1];
}

#endif
