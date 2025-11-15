#ifndef STORM_UNICODE_HPP
#define STORM_UNICODE_HPP

#include "Core.hpp"
#include <cstddef>

ptrdiff_t SUniConvertUTF16ToDos(char* dest, const wchar_t* source, uint32_t destsize);

ptrdiff_t SUniConvertUTF16ToMac(char* dest, const wchar_t* source, uint32_t destsize);

ptrdiff_t SUniConvertUTF16ToWin(char* dest, const wchar_t* source, uint32_t destsize);

ptrdiff_t SUniConvertDosToUTF16(wchar_t* dest, const char* source, uint32_t destsize);

ptrdiff_t SUniConvertMacToUTF16(wchar_t* dest, const char* source, uint32_t destsize);

ptrdiff_t SUniConvertWinToUTF16(wchar_t* dest, const char* source, uint32_t destsize);

uint32_t STORMAPI SUniSGetUTF8(const uint8_t* strptr, int32_t* chars);

void STORMAPI SUniSPutUTF8(uint32_t c, char* strptr);

#endif
