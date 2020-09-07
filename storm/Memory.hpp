#ifndef STORM_MEMORY_HPP
#define STORM_MEMORY_HPP

#include <cstdint>
#include <cstdlib>

void* SMemAlloc(size_t bytes, const char* filename, int32_t linenumber, uint32_t flags);

void SMemFree(void* ptr);

void SMemFree(void* ptr, const char* filename, int32_t linenumber, uint32_t flags);

void* SMemReAlloc(void* ptr, size_t bytes, const char* filename, int32_t linenumber, uint32_t flags);

#endif
