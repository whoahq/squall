#include "storm/queue/CSBasePriority.hpp"
#include "storm/queue/CSBasePriorityQueue.hpp"

void CSBasePriority::Relink() {
    CSBasePriorityQueue* queue = this->m_queue;

    if (!queue) {
        return;
    }

    void* ptr = queue->operator[](this->m_index);

    this->Unlink();

    int32_t v18 = queue->Count();
    queue->SetCount(v18 + 1);

    while (v18) {
        auto v19 = reinterpret_cast<CSBasePriority*>(
            reinterpret_cast<uintptr_t>(ptr) + queue->m_linkOffset);

        CSBasePriority* v20 = queue->Link((v18 - 1) >> 1);

        if (v20->Compare(v19)) {
            break;
        }

        queue->operator[](v18) = queue->operator[]((v18 - 1) >> 1);
        queue->SetLink(v18);

        v18 = (v18 - 1) >> 1;
    }

    queue->operator[](v18) = ptr;
    queue->SetLink(v18);
}

void CSBasePriority::Unlink() {
    CSBasePriorityQueue* queue = this->m_queue;

    if (!queue) {
        return;
    }

    int32_t index = this->m_index;

    CSBasePriority* link = this->m_queue->Link(this->m_index);
    link->m_queue = 0;
    link->m_index = 0;

    void* top = queue->Top();

    int32_t v8 = queue->Count();
    int32_t v9 = queue->Count() - 1;

    queue->SetCount(v9);

    if (index == v9) {
        return;
    }

    int32_t hBound = v9 - 1;
    int32_t lBound = (v9 - 2) >> 1;

    if (index < hBound && index <= lBound >> 1) {
        while (true) {
            int32_t v11 = 2 * index + 1;

            if (v11 < hBound) {
                if (queue->Link(2 * index + 2)->Compare(queue->Link(v11))) {
                    v11 = 2 * index + 2;
                }
            }

            if (reinterpret_cast<CSBasePriority*>(reinterpret_cast<uintptr_t>(top) + queue->m_linkOffset)->Compare(queue->Link(v11))) {
                break;
            }

            queue->operator[](index) = queue->operator[](v11);
            queue->SetLink(index);

            index = v11;

            if (v11 > lBound) {
                break;
            }
        }
    }

    queue->operator[](index) = top;
    queue->SetLink(index);
}
