#ifndef STORM_UNICODE_HPP
#define STORM_UNICODE_HPP

#include "Core.hpp"
#include <cstddef>

ptrdiff_t STORMAPI SUniConvertUTF16ToDos(char* dest, const char16_t* source, uint32_t destsize);

ptrdiff_t STORMAPI SUniConvertUTF16ToMac(char* dest, const char16_t* source, uint32_t destsize);

ptrdiff_t STORMAPI SUniConvertUTF16ToWin(char* dest, const char16_t* source, uint32_t destsize);

ptrdiff_t STORMAPI SUniConvertDosToUTF16(char16_t* dest, const char* source, uint32_t destsize);

ptrdiff_t STORMAPI SUniConvertMacToUTF16(char16_t* dest, const char* source, uint32_t destsize);

ptrdiff_t STORMAPI SUniConvertWinToUTF16(char16_t* dest, const char* source, uint32_t destsize);

// Assumes index is within the bounds of utf8String
int32_t STORMAPI SUniFindAfterUTF8Chr(const char* utf8String, int32_t index);

// Assumes index is within the bounds of utf8String
int32_t STORMAPI SUniFindUTF8ChrStart(const char* utf8String, int32_t index);

uint32_t STORMAPI SUniSGetUTF8(const uint8_t* strptr, int32_t* chars);

void STORMAPI SUniSPutUTF8(uint32_t c, char* strptr);

#endif
