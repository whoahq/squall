#include "storm/big/BigBuffer.hpp"

uint32_t& BigBuffer::operator[](uint32_t index) {
    this->GrowToFit(index);
    return this->m_data[this->m_offset + index];
}

uint32_t BigBuffer::operator[](uint32_t index) const {
    if (this->IsUsed(index)) {
        return const_cast<TSGrowableArray<uint32_t>&>(this->m_data)[this->m_offset + index];
    }

    return 0;
}

void BigBuffer::Clear() {
    this->m_data.SetCount(this->m_offset);
}

uint32_t BigBuffer::Count() const {
    return this->m_data.Count() - this->m_offset;
}

void BigBuffer::GrowToFit(uint32_t index) {
    this->m_data.GrowToFit(this->m_offset + index, 1);
}

int32_t BigBuffer::IsUsed(uint32_t index) const {
    return index + this->m_offset < this->m_data.Count();
}

void BigBuffer::SetCount(uint32_t count) {
    this->m_data.SetCount(this->m_offset + count);
}

void BigBuffer::Trim() const {
    while (this->Count()) {
        auto& data = const_cast<TSGrowableArray<uint32_t>&>(this->m_data);

        if (*data.Top()) {
            break;
        }

        data.SetCount(data.Count() - 1);
    }
}
