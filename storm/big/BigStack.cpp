#include "storm/big/BigStack.hpp"

BigBuffer& BigStack::Alloc(uint32_t* count) {
    STORM_ASSERT(this->m_used < SIZE);

    if (count) {
        (*count)++;
    }

    auto& buffer = this->m_buffer[this->m_used];
    this->m_used++;

    return buffer;
}

void BigStack::Free(uint32_t count) {
    STORM_ASSERT(count <= this->m_used);

    this->m_used -= count;
}

BigBuffer& BigStack::MakeDistinct(BigBuffer& orig, int32_t required) {
    return required ? this->Alloc(nullptr) : orig;
}

void BigStack::UnmakeDistinct(BigBuffer& orig, BigBuffer& distinct) {
    if (&orig != &distinct) {
        orig = distinct;
        this->Free(1);
    }
}
