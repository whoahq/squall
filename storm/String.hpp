#ifndef STORM_STRING_HPP
#define STORM_STRING_HPP

#include <cstdint>
#include <cstdlib>

#define STORM_MAX_PATH 260
#define STORM_MAX_STR 0x7FFFFFFF

char* SStrChr(char* string, char search);

const char* SStrChr(const char* string, char search);

char* SStrChrR(char* string, char search);

const char* SStrChrR(const char* string, char search);

int32_t SStrCmp(const char* string1, const char* string2, size_t maxchars);

int32_t SStrCmpI(const char* string1, const char* string2, size_t maxchars);

size_t SStrCopy(char* dest, const char* source, size_t destsize);

char* SStrDupA(const char* string, const char* filename, uint32_t linenumber);

uint32_t SStrHashHT(const char* string);

size_t SStrLen(const char* string);

void SStrLower(char* string);

uint32_t SStrPack(char* dest, const char* source, uint32_t destsize);

size_t SStrPrintf(char* dest, size_t maxchars, const char* format, ...);

const char* SStrStr(const char* string, const char* search);

void SStrTokenize(const char** string, char* buffer, size_t bufferchars, const char* whitespace, int32_t* quoted);

float SStrToFloat(const char* string);

int32_t SStrToInt(const char* string);

void SStrUpper(char* string);

#endif
