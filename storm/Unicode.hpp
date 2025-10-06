#ifndef STORM_UNICODE_HPP
#define STORM_UNICODE_HPP

#include "Core.hpp"

uint32_t STORMAPI SUniSGetUTF8(const uint8_t* strptr, int32_t* chars);

void STORMAPI SUniSPutUTF8(uint32_t c, char* strptr);

#endif
