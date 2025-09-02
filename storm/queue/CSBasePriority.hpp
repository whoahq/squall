#ifndef STORM_QUEUE_CS_BASE_PRIORITY_HPP
#define STORM_QUEUE_CS_BASE_PRIORITY_HPP

#include <cstdint>

class CSBasePriorityQueue;

class CSBasePriority {
    public:
    // Member variables
    CSBasePriorityQueue* m_queue = nullptr;
    uint32_t m_index = 0;

    // Member functions
    virtual bool Compare(CSBasePriority* a) = 0;
    void Unlink();
    void Relink();
};

#endif
