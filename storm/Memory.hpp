#ifndef STORM_MEMORY_HPP
#define STORM_MEMORY_HPP

#include <cstdint>
#include <cstdlib>
#include <new>
#include "Core.hpp"

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

void* STORMAPI SMemAlloc(size_t bytes, const char* filename, int32_t linenumber, uint32_t flags = 0);

void STORMAPI SMemCopy(void* dst, void* src, size_t bytes);

void STORMAPI SMemFill(void* ptr, size_t bytes, uint8_t value);

void STORMAPI SMemFree(void* ptr);

void STORMAPI SMemFree(void* ptr, const char* filename, int32_t linenumber, uint32_t flags = 0);

void STORMAPI SMemMove(void* dst, void* src, size_t bytes);

void* STORMAPI SMemReAlloc(void* ptr, size_t bytes, const char* filename, int32_t linenumber, uint32_t flags = 0);

void STORMAPI SMemZero(void* ptr, size_t bytes);

#endif
