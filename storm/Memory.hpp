#ifndef STORM_MEMORY_HPP
#define STORM_MEMORY_HPP

#include <cstdint>
#include <cstdlib>
#include <new>

#define SMEM_FLAG_ZEROMEMORY 0x8

#define STORM_ALLOC(bytes)  \
    SMemAlloc(bytes, __FILE__, __LINE__, 0x0)

#define STORM_ALLOC_ZERO(bytes) \
    SMemAlloc(bytes, __FILE__, __LINE__, SMEM_FLAG_ZEROMEMORY)

#define STORM_FREE(ptr)  \
    SMemFree(ptr, __FILE__, __LINE__, 0x0)

#define STORM_NEW(t)    \
    new(SMemAlloc(sizeof(t), __FILE__, __LINE__, 0x0)) t

#define STORM_NEW_ZERO(t)   \
    new(SMemAlloc(sizeof(t), __FILE__, __LINE__, SMEM_FLAG_ZEROMEMORY)) t

void* SMemAlloc(size_t bytes, const char* filename, int32_t linenumber, uint32_t flags = 0);

void SMemCopy(void* dst, void* src, size_t bytes);

void SMemFill(void* ptr, size_t bytes, uint8_t value);

void SMemFree(void* ptr);

void SMemFree(void* ptr, const char* filename, int32_t linenumber, uint32_t flags = 0);

void SMemMove(void* dst, void* src, size_t bytes);

void* SMemReAlloc(void* ptr, size_t bytes, const char* filename, int32_t linenumber, uint32_t flags = 0);

void SMemZero(void* ptr, size_t bytes);

#endif
