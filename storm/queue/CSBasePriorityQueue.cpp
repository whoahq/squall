#include "storm/queue/CSBasePriorityQueue.hpp"
#include "storm/queue/CSBasePriority.hpp"

void* CSBasePriorityQueue::Dequeue() {
    if (this->Count()) {
        void* bottom = this->operator[](0);
        this->Remove(0);
        return bottom;
    } else {
        return nullptr;
    }
}

void CSBasePriorityQueue::Enqueue(void* val) {
    int32_t v3 = this->Count();
    this->SetCount(v3 + 1);

    while (true) {
        int32_t v4 = (v3 - 1) >> 1;

        if (!v3) {
            break;
        }

        if (this->Link(v4)->Compare(reinterpret_cast<CSBasePriority*>(reinterpret_cast<uintptr_t>(val) + this->m_linkOffset))) {
            break;
        }

        this->operator[](v3) = this->operator[](v4);
        this->SetLink(v3);

        v3 = (v3 - 1) >> 1;
    }

    this->operator[](v3) = val;
    this->SetLink(v3);
}

CSBasePriority* CSBasePriorityQueue::Link(uint32_t index) const {
    return reinterpret_cast<CSBasePriority*>(
        reinterpret_cast<uintptr_t>(this->m_data[index]) + this->m_linkOffset);
}

void CSBasePriorityQueue::SetLink(uint32_t index) {
    CSBasePriority* link = this->Link(index);

    link->m_index = index;
    link->m_queue = this;
}

void* CSBasePriorityQueue::Top() {
    int32_t count = this->Count();

    if (count) {
        return this->m_data[count - 1];
    } else {
        return nullptr;
    }
}

void CSBasePriorityQueue::Remove(uint32_t index) {
    this->Link(index)->Unlink();
}
