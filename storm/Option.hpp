#ifndef STORM_OPTION_OPTION_HPP
#define STORM_OPTION_OPTION_HPP

#include <cstdint>
#include "Core.hpp"

int32_t STORMAPI StormSetOption(int32_t optname, const void* optval, uint32_t optlen);

int32_t STORMAPI StormGetOption(int32_t optname, void* optval, uint32_t* optlen);

#endif
