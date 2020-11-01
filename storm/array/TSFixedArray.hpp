#ifndef STORM_ARRAY_TS_FIXED_ARRAY_HPP
#define STORM_ARRAY_TS_FIXED_ARRAY_HPP

#include "storm/array/TSBaseArray.hpp"
#include "storm/Memory.hpp"
#include <cstdint>

template<class T>
class TSFixedArray : public TSBaseArray<T> {
    public:
    void ReallocData(uint32_t count);
    void SetCount(uint32_t count);
};

template<class T>
void TSFixedArray<T>::ReallocData(uint32_t count) {
    T* oldData = this->m_data;

    if (count < this->m_count) {
        for (int32_t i = count; i < this->m_count; i++) {
            T* element = &this->m_data[i];
            delete element;
        }
    }

    this->m_alloc = count;

    void* v6 = SMemReAlloc(oldData, sizeof(T) * count, nullptr, 0, 0x10);
    this->m_data = (T*)v6;

    if (!v6) {
        this->m_data = (T*)SMemAlloc(sizeof(T) * count, nullptr, 0, 0x0);

        if (oldData) {
            uint32_t smallestCount = count >= this->m_count ? this->m_count : count;

            for (int32_t i = 0; i < smallestCount; i++) {
                T* v8 = &this->m_data[i];

                if (v8) {
                    *v8 = oldData[i];
                }
            }

            SMemFree(oldData, nullptr, 0, 0x0);
        }
    }
}

template<class T>
void TSFixedArray<T>::SetCount(uint32_t count) {
    if (count != this->m_count) {
        if (count) {
            this->ReallocData(count);

            for (int32_t i = this->m_count; i < count; i++) {
                new (&this->m_data[i]) T();
            }

            this->m_count = count;
        } else {
            this->Clear();
        }
    }
}

#endif
