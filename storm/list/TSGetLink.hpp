#ifndef STORM_LIST_TS_GET_LINK_HPP
#define STORM_LIST_TS_GET_LINK_HPP

#include "list/TSLink.hpp"
#include "list/TSLinkedNode.hpp"
#include <cstddef>
#include <cstdint>

template <class T>
class TSGetLink {
    public:
    // Static functions
    static TSLink<T>* Link(const TSLinkedNode<T>* nodeptr, ptrdiff_t linkoffset);
};

template <class T>
TSLink<T>* TSGetLink<T>::Link(const TSLinkedNode<T>* nodeptr, ptrdiff_t linkoffset) {
    return reinterpret_cast<TSLink<T>*>(reinterpret_cast<uintptr_t>(nodeptr));
}

#endif
