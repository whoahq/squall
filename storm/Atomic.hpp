#ifndef STORM_ATOMIC_HPP
#define STORM_ATOMIC_HPP

#include <cstdint>

int32_t SInterlockedDecrement(int32_t* ptr);

int32_t SInterlockedIncrement(int32_t* ptr);

#endif
