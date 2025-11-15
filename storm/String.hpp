#ifndef STORM_STRING_HPP
#define STORM_STRING_HPP

#include <cstdarg>
#include <cstdlib>
#include "Core.hpp"

#define STORM_MAX_PATH 260
#define STORM_MAX_STR 0x7FFFFFFF

#define SSTR_HASH_CASESENSITIVE 1

char* STORMAPI SStrChr(char* string, char search);

const char* STORMAPI SStrChr(const char* string, char search);

const char* STORMAPI SStrChrBidir(const char* string, char search, int32_t reverse);

char* STORMAPI SStrChrR(char* string, char search);

const char* STORMAPI SStrChrR(const char* string, char search);

int32_t STORMAPI SStrCmp(const char* string1, const char* string2, size_t maxchars = STORM_MAX_STR);

int32_t STORMAPI SStrCmpI(const char* string1, const char* string2, size_t maxchars = STORM_MAX_STR);

size_t STORMAPI SStrCopy(char* dest, const char* source, size_t destsize = STORM_MAX_STR);

char* STORMAPI SStrDupA(const char* string, const char* filename, uint32_t linenumber);

uint32_t STORMAPI SStrHash(const char* string, uint32_t flags = 0, uint32_t seed = 0);

uint32_t STORMAPI SStrHashHT(const char* string);

int64_t STORMAPI SStrHash64(const char* string, uint32_t flags = 0, int64_t seed = 0);

size_t STORMAPI SStrLen(const char* string);

void STORMAPI SStrLower(char* string);

uint32_t STORMAPI SStrPack(char* dest, const char* source, uint32_t destsize);

size_t STORMCDECL SStrPrintf(char* dest, size_t maxchars, const char* format, ...);

size_t STORMCDECL SStrVPrintf(char* dest, size_t maxchars, const char* format, va_list arglist);

char* STORMAPI SStrStr(char* string, const char* search);

const char* STORMAPI SStrStr(const char* string, const char* search);

char* STORMAPI SStrStrI(char* string, const char* search);

const char* STORMAPI SStrStrI(const char* string, const char* search);

void STORMAPI SStrTokenize(const char** string, char* buffer, size_t bufferchars, const char* whitespace, int32_t* quoted);

double STORMAPI SStrToDouble(const char* string);

float STORMAPI SStrToFloat(const char* string);

int32_t STORMAPI SStrToInt(const char* string);

uint32_t STORMAPI SStrToUnsigned(const char* string);

void STORMAPI SStrUpper(char* string);

#endif
