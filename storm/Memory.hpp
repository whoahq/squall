#ifndef STORM_MEMORY_HPP
#define STORM_MEMORY_HPP

#include <cstdint>
#include <cstdlib>


#define SMEM_FLAG_ZEROMEMORY 8


void* SMemAlloc(size_t bytes, const char* filename, int32_t linenumber, uint32_t flags = 0);

void SMemFill(void* ptr, size_t bytes, uint8_t value);

void SMemFree(void* ptr);

void SMemFree(void* ptr, const char* filename, int32_t linenumber, uint32_t flags = 0);

void* SMemReAlloc(void* ptr, size_t bytes, const char* filename, int32_t linenumber, uint32_t flags = 0);

void SMemZero(void* ptr, size_t bytes);

#endif
