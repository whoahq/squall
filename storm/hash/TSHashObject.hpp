#ifndef STORM_HASH_TS_HASH_OBJECT_HPP
#define STORM_HASH_TS_HASH_OBJECT_HPP

#include "storm/List.hpp"
#include <cstdint>

template <class T, class TKey>
class TSHashObject {
    public:
    // Member variables
    uint32_t m_hashval;
    TSLink<T> m_linktoslot;
    TSLink<T> m_linktofull;
    TKey m_key;

    TSHashObject & operator=(const TSHashObject &source) {
        return *this;
    }
};

#endif
