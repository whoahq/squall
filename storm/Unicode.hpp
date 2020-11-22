#ifndef STORM_UNICODE_HPP
#define STORM_UNICODE_HPP

#include <cstdint>

uint32_t SUniSGetUTF8(const uint8_t* strptr, int32_t* chars);

void SUniSPutUTF8(uint32_t c, char* strptr);

#endif
