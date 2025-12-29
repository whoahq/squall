#include "storm/File.hpp"

// TODO: implement native filesystem-backed SFile functions. same functionality as whoa's SFile?

int32_t STORMAPI SFileOpenArchive(const char* archivename, int32_t priority, uint32_t flags, HSARCHIVE* handle) {
    (void)archivename;
    (void)priority;
    (void)flags;
    (void)handle;
    return 0;
}

int32_t STORMAPI SFileCloseArchive(HSARCHIVE handle) {
    (void)handle;
    return 0;
}

int32_t STORMAPI SFileOpenFileEx(HSARCHIVE archivehandle, const char* filename, uint32_t flags, HSFILE* handle) {
    (void)archivehandle;
    (void)filename;
    (void)flags;
    (void)handle;
    return 0;
}

int32_t STORMAPI SFileReadFile(HSFILE handle, void* buffer, uint32_t bytestoread, uint32_t* bytesread, LPOVERLAPPED overlapped) {
    (void)handle;
    (void)buffer;
    (void)bytestoread;
    (void)overlapped;
    if (bytesread) {
        *bytesread = 0;
    }
    return 0;
}

uint32_t STORMAPI SFileGetFileSize(HSFILE handle, uint32_t* filesizehigh) {
    (void)handle;
    if (filesizehigh) {
        *filesizehigh = 0;
    }
    return 0;
}

uint32_t STORMAPI SFileSetFilePointer(HSFILE handle, int32_t distancetomove, int32_t* distancetomovehigh, uint32_t movemethod) {
    (void)handle;
    (void)distancetomove;
    (void)movemethod;
    if (distancetomovehigh) {
        *distancetomovehigh = 0;
    }
    return 0;
}

int32_t STORMAPI SFileCloseFile(HSFILE handle) {
    (void)handle;
    return 0;
}
