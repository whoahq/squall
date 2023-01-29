#include "storm/big/BigData.hpp"

TSGrowableArray<uint8_t>& BigData::Output() const {
    return const_cast<TSGrowableArray<uint8_t>&>(this->m_output);
}

BigBuffer& BigData::Primary() {
    return this->m_primary;
}
