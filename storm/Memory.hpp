#ifndef STORM_MEMORY_HPP
#define STORM_MEMORY_HPP

#include <cstdint>
#include <cstdlib>

#define SMEM_FLAG_ZEROMEMORY 0x8

void* SMemAlloc(size_t bytes, const char* filename, int32_t linenumber, uint32_t flags = 0);

void SMemCopy(void* dst, void* src, size_t bytes);

void SMemFill(void* ptr, size_t bytes, uint8_t value);

void SMemFree(void* ptr);

void SMemFree(void* ptr, const char* filename, int32_t linenumber, uint32_t flags = 0);

void SMemMove(void* dst, void* src, size_t bytes);

void* SMemReAlloc(void* ptr, size_t bytes, const char* filename, int32_t linenumber, uint32_t flags = 0);

void SMemZero(void* ptr, size_t bytes);

#endif
