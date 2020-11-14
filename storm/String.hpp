#ifndef STORM_STRING_HPP
#define STORM_STRING_HPP

#include <cstdint>
#include <cstdlib>

size_t SStrCopy(char* dest, const char* source, size_t destsize);

uint32_t SStrHashHT(const char* string);

#endif
