#include "storm/File.hpp"

int32_t StormLib_SFileOpenArchive(const char* archivename, uint32_t priority, uint32_t flags, void** handle);
int32_t StormLib_SFileCloseArchive(void* handle);
int32_t StormLib_SFileOpenFileEx(void* archivehandle, const char* filename, uint32_t flags, void** handle);
int32_t StormLib_SFileReadFile(void* handle, void* buffer, uint32_t bytestoread, uint32_t* bytesread, void* overlapped);
uint32_t StormLib_SFileGetFileSize(void* handle, uint32_t* filesizehigh);
uint32_t StormLib_SFileSetFilePointer(void* handle, int32_t distancetomove, int32_t* distancetomovehigh, uint32_t movemethod);
int32_t StormLib_SFileCloseFile(void* handle);

int32_t STORMAPI SFileOpenArchive(const char* archivename, int32_t priority, uint32_t flags, HSARCHIVE* handle) {
    if (!handle) {
        return 0;
    }

    void* archive = nullptr;
    int32_t result = StormLib_SFileOpenArchive(archivename, static_cast<uint32_t>(priority), flags, &archive);
    *handle = reinterpret_cast<HSARCHIVE>(archive);
    return result;
}

int32_t STORMAPI SFileCloseArchive(HSARCHIVE handle) {
    return StormLib_SFileCloseArchive(reinterpret_cast<void*>(handle));
}

int32_t STORMAPI SFileOpenFileEx(HSARCHIVE archivehandle, const char* filename, uint32_t flags, HSFILE* handle) {
    if (!handle) {
        return 0;
    }

    void* file = nullptr;
    int32_t result = StormLib_SFileOpenFileEx(reinterpret_cast<void*>(archivehandle), filename, flags, &file);
    *handle = reinterpret_cast<HSFILE>(file);
    return result;
}

int32_t STORMAPI SFileReadFile(HSFILE handle, void* buffer, uint32_t bytestoread, uint32_t* bytesread, LPOVERLAPPED overlapped) {
    return StormLib_SFileReadFile(reinterpret_cast<void*>(handle), buffer, bytestoread, bytesread, reinterpret_cast<void*>(overlapped));
}

uint32_t STORMAPI SFileGetFileSize(HSFILE handle, uint32_t* filesizehigh) {
    return StormLib_SFileGetFileSize(reinterpret_cast<void*>(handle), filesizehigh);
}

uint32_t STORMAPI SFileSetFilePointer(HSFILE handle, int32_t distancetomove, int32_t* distancetomovehigh, uint32_t movemethod) {
    return StormLib_SFileSetFilePointer(reinterpret_cast<void*>(handle), distancetomove, distancetomovehigh, movemethod);
}

int32_t STORMAPI SFileCloseFile(HSFILE handle) {
    return StormLib_SFileCloseFile(reinterpret_cast<void*>(handle));
}
