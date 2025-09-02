#include "storm/Memory.hpp"

#include <cstring>

constexpr size_t ALIGNMENT = 8;

void* SMemAlloc(size_t bytes, const char* filename, int32_t linenumber, uint32_t flags) {
    size_t alignedBytes = (bytes + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);

    void* result;

    if (flags & SMEM_FLAG_ZEROMEMORY) {
        result = calloc(1, alignedBytes);
    } else {
        result = malloc(alignedBytes);
    }

    if (result) {
        return result;
    } else {
        // TODO handle errors
        return nullptr;
    }
}

void SMemFill(void* ptr, size_t bytes, uint8_t value) {
    memset(ptr, value, bytes);
}

void SMemFree(void* ptr) {
    if (ptr) {
        free(ptr);
    }
}

void SMemFree(void* ptr, const char* filename, int32_t linenumber, uint32_t flags) {
    if (ptr) {
        free(ptr);
    }
}

void* SMemReAlloc(void* ptr, size_t bytes, const char* filename, int32_t linenumber, uint32_t flags) {
    if (flags == 0xB00BEEE5) {
        return nullptr;
    }

    if (!ptr) {
        return SMemAlloc(bytes, filename, linenumber, flags);
    }

    if (flags & 0x10) {
        return nullptr;
    }

    size_t alignedBytes = (bytes + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);

    void* result = realloc(ptr, alignedBytes);

    if (result) {
        if (flags & 0x8) {
            // TODO zero out expanded portion
        }

        return result;
    } else {
        if (alignedBytes) {
            // TODO handle errors
        }

        return nullptr;
    }
}
