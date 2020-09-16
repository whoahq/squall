#ifndef STORM_LIST_TS_GET_EXPLICIT_LINK_HPP
#define STORM_LIST_TS_GET_EXPLICIT_LINK_HPP

#include "list/TSLink.hpp"
#include <cstddef>
#include <cstdint>

template <class T>
class TSGetExplicitLink {
    public:
    // Static functions
    static TSLink<T>* Link(const void* nodeptr, ptrdiff_t linkoffset);
};

template <class T>
TSLink<T>* TSGetExplicitLink<T>::Link(const void* nodeptr, ptrdiff_t linkoffset) {
    return reinterpret_cast<TSLink<T>*>((reinterpret_cast<uintptr_t>(nodeptr) + linkoffset));
}

#endif
