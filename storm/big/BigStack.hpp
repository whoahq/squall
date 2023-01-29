#ifndef STORM_BIG_BIG_STACK_HPP
#define STORM_BIG_BIG_STACK_HPP

#include "storm/big/BigBuffer.hpp"
#include <cstdint>

class BigStack {
    public:
    // Static variables
    const static uint32_t SIZE = 16;

    // Member variables
    BigBuffer m_buffer[SIZE];
    uint32_t m_used = 0;

    // Member functions
};

#endif
