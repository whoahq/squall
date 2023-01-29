#ifndef STORM_BIG_BIG_DATA_HPP
#define STORM_BIG_BIG_DATA_HPP

#include "storm/big/BigBuffer.hpp"
#include "storm/big/BigStack.hpp"
#include "storm/Array.hpp"

class BigData {
    public:
    // Member variables
    BigBuffer m_primary;
    BigStack m_stack;
    TSGrowableArray<uint8_t> m_output;

    // Member functions
    TSGrowableArray<uint8_t>& Output() const;
    BigBuffer& Primary();
};

#endif
