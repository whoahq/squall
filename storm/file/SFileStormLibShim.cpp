#include "StormLib.h"
#include <cstdint>

int32_t StormLib_SFileOpenArchive(const char* archivename, uint32_t priority, uint32_t flags, void** handle) {
    if (!handle) {
        return 0;
    }

    HANDLE archive = nullptr;
    bool result = SFileOpenArchive(archivename, static_cast<DWORD>(priority), static_cast<DWORD>(flags), &archive);
    *handle = archive;
    return result ? 1 : 0;
}

int32_t StormLib_SFileCloseArchive(void* handle) {
    return SFileCloseArchive(static_cast<HANDLE>(handle)) ? 1 : 0;
}

int32_t StormLib_SFileOpenFileEx(void* archivehandle, const char* filename, uint32_t flags, void** handle) {
    if (!handle) {
        return 0;
    }

    HANDLE file = nullptr;
    bool result = SFileOpenFileEx(static_cast<HANDLE>(archivehandle), filename, static_cast<DWORD>(flags), &file);
    *handle = file;
    return result ? 1 : 0;
}

int32_t StormLib_SFileReadFile(void* handle, void* buffer, uint32_t bytestoread, uint32_t* bytesread, void* overlapped) {
    return SFileReadFile(
        static_cast<HANDLE>(handle),
        buffer,
        static_cast<DWORD>(bytestoread),
        reinterpret_cast<LPDWORD>(bytesread),
        reinterpret_cast<LPOVERLAPPED>(overlapped)
    ) ? 1 : 0;
}

uint32_t StormLib_SFileGetFileSize(void* handle, uint32_t* filesizehigh) {
    return SFileGetFileSize(
        static_cast<HANDLE>(handle),
        reinterpret_cast<LPDWORD>(filesizehigh)
    );
}

uint32_t StormLib_SFileSetFilePointer(void* handle, int32_t distancetomove, int32_t* distancetomovehigh, uint32_t movemethod) {
    return SFileSetFilePointer(
        static_cast<HANDLE>(handle),
        static_cast<LONG>(distancetomove),
        reinterpret_cast<PLONG>(distancetomovehigh),
        static_cast<DWORD>(movemethod)
    );
}

int32_t StormLib_SFileCloseFile(void* handle) {
    return SFileCloseFile(static_cast<HANDLE>(handle)) ? 1 : 0;
}
