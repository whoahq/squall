#include "storm/file/SFile.hpp"
#include "storm/File.hpp"
#include "storm/Memory.hpp"
#include <algorithm>
#include <climits>
#include <new>

int32_t SFile::Close(SFile* file) {
    return SFileCloseFile(reinterpret_cast<HSFILE>(file));
}

int32_t SFile::FileExists(const char* filename) {
    HSFILE handle = nullptr;
    int32_t result = SFileOpenFileEx(nullptr, filename, 0, &handle);
    if (result && handle) {
        SFileCloseFile(handle);
    }
    return result;
}

size_t SFile::GetFileSize(SFile* file, size_t* filesizeHigh) {
    uint32_t high = 0;
    uint32_t low = SFileGetFileSize(reinterpret_cast<HSFILE>(file), filesizeHigh ? &high : nullptr);
    if (filesizeHigh) {
        *filesizeHigh = static_cast<size_t>(high);
    }

    if (sizeof(size_t) > sizeof(uint32_t) && high != 0) {
        return (static_cast<size_t>(high) << 32) | low;
    }

    return static_cast<size_t>(low);
}

int32_t SFile::IsStreamingMode() {
    return 0;
}

int32_t SFile::IsStreamingTrial() {
    return 0;
}

int32_t SFile::Load(SArchive* archive, const char* filename, void** buffer, size_t* bytes, size_t extraBytes, uint32_t flags, SOVERLAPPED* overlapped) {
    (void)overlapped;

    HSFILE handle = nullptr;
    if (!SFileOpenFileEx(reinterpret_cast<HSARCHIVE>(archive), filename, flags, &handle) || !handle) {
        return 0;
    }

    uint32_t high = 0;
    uint32_t low = SFileGetFileSize(handle, &high);
    size_t size = static_cast<size_t>(low);
    if (sizeof(size_t) > sizeof(uint32_t) && high != 0) {
        size |= (static_cast<size_t>(high) << 32);
    }

    if (size > UINT32_MAX) {
        SFileCloseFile(handle);
        return 0;
    }

    if (bytes) {
        *bytes = size;
    }

    char* data = new (std::nothrow) char[size + extraBytes];
    if (!data) {
        SFileCloseFile(handle);
        return 0;
    }

    uint32_t bytesRead = 0;
    int32_t result = SFileReadFile(handle, data, static_cast<uint32_t>(size), &bytesRead, nullptr);
    SFileCloseFile(handle);

    if (!result || bytesRead != size) {
        STORM_FREE(data);
        return 0;
    }

    if (extraBytes) {
        std::fill(data + size, data + size + extraBytes, 0);
    }

    *buffer = data;
    return 1;
}

int32_t SFile::Open(const char* filename, SFile** file) {
    return SFileOpenFileEx(nullptr, filename, 0, reinterpret_cast<HSFILE*>(file));
}

int32_t SFile::OpenEx(SArchive* archive, const char* filename, uint32_t flags, SFile** file) {
    return SFileOpenFileEx(reinterpret_cast<HSARCHIVE>(archive), filename, flags, reinterpret_cast<HSFILE*>(file));
}

int32_t SFile::Read(SFile* file, void* buffer, size_t bytestoread, size_t* bytesread, SOVERLAPPED* overlapped, TASYNCPARAMBLOCK* asyncparam) {
    (void)asyncparam;

    uint32_t toRead = bytestoread > UINT32_MAX ? UINT32_MAX : static_cast<uint32_t>(bytestoread);
    uint32_t outRead = 0;
    int32_t result = SFileReadFile(reinterpret_cast<HSFILE>(file), buffer, toRead, &outRead, reinterpret_cast<LPOVERLAPPED>(overlapped));
    if (bytesread) {
        *bytesread = static_cast<size_t>(outRead);
    }
    return result;
}

int32_t SFile::Unload(void* ptr) {
    STORM_FREE(ptr);
    return 1;
}
