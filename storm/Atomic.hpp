#ifndef STORM_ATOMIC_HPP
#define STORM_ATOMIC_HPP

#include <cstdint>

#if defined(WHOA_SYSTEM_WIN)
typedef long ATOMIC32;
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
typedef int32_t ATOMIC32;
#endif

ATOMIC32 SInterlockedDecrement(ATOMIC32* ptr);

ATOMIC32 SInterlockedIncrement(ATOMIC32* ptr);

#endif
