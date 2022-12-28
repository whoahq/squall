#ifndef STORM_LIST_TS_LINKED_NODE_HPP
#define STORM_LIST_TS_LINKED_NODE_HPP

#include "storm/list/TSLink.hpp"

template <class T>
class TSLinkedNode {
    public:
    // Member variables
    TSLink<T> m_link;

    // Member functions
    ~TSLinkedNode();
    T* Next();
    T* Prev();
    void Unlink();
};

template <class T>
TSLinkedNode<T>::~TSLinkedNode() {
    this->Unlink();
}

template <class T>
T* TSLinkedNode<T>::Next() {
    return this->m_link.Next();
}

template <class T>
T* TSLinkedNode<T>::Prev() {
    return this->m_link.Prev();
}

template <class T>
void TSLinkedNode<T>::Unlink() {
    this->m_link.Unlink();
}

#endif
