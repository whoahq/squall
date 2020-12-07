#ifndef STORM_QUEUE_TS_PRIORITY_QUEUE_HPP
#define STORM_QUEUE_TS_PRIORITY_QUEUE_HPP

#include "storm/queue/CSBasePriorityQueue.hpp"
#include <cstdint>

template <class T>
class TSPriorityQueue : public CSBasePriorityQueue {
    public:
    // Member functions
    TSPriorityQueue(uint32_t linkOffset);
    T* Dequeue();
};

template <class T>
TSPriorityQueue<T>::TSPriorityQueue(uint32_t linkOffset)
    : CSBasePriorityQueue() {
    this->m_linkOffset = linkOffset;
}

template <class T>
T* TSPriorityQueue<T>::Dequeue() {
    return static_cast<T*>(CSBasePriorityQueue::Dequeue());
}

#endif
