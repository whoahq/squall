#ifndef STORM_LIST_TS_EXPLICIT_LIST_HPP
#define STORM_LIST_TS_EXPLICIT_LIST_HPP

#include "storm/list/TSGetExplicitLink.hpp"
#include "storm/list/TSList.hpp"
#include <cstdlib>

#define STORM_EXPLICIT_LIST(T, link) TSExplicitList<T, offsetof(T, link)>

template <class T, size_t offset>
class TSExplicitList : public TSList<T, TSGetExplicitLink<T>> {
    public:
    // Member functions
    TSExplicitList();
};

template <class T, size_t offset>
TSExplicitList<T, offset>::TSExplicitList()
    : TSList<T, TSGetExplicitLink<T>>() {
    this->SetLinkOffset(offset);
}

#endif
