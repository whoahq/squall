#include "storm/File.hpp"
#include <fstream>
#include <limits>
#include <string>

namespace {
    struct NativeFileHandle {
        std::ifstream stream;
        std::streamsize size = 0;
    };

    std::string NormalizePath(const char* filename) {
        std::string path = filename ? filename : "";
        for (char& ch : path) {
            if (ch == '\\') {
                ch = '/';
            }
        }
        return path;
    }
}

int32_t STORMAPI SFileOpenArchive(const char* archivename, int32_t priority, uint32_t flags, HSARCHIVE* handle) {
    (void)archivename;
    (void)priority;
    (void)flags;
    if (handle) {
        *handle = nullptr;
    }
    return 0;
}

int32_t STORMAPI SFileCloseArchive(HSARCHIVE handle) {
    (void)handle;
    return 0;
}

int32_t STORMAPI SFileOpenFileEx(HSARCHIVE archivehandle, const char* filename, uint32_t flags, HSFILE* handle) {
    (void)archivehandle;
    (void)flags;

    if (!filename || !handle) {
        return 0;
    }

    auto file = new NativeFileHandle();
    file->stream.open(NormalizePath(filename), std::ios::in | std::ios::binary | std::ios::ate);

    if (!file->stream.is_open()) {
        delete file;
        *handle = nullptr;
        return 0;
    }

    file->stream.seekg(0, std::ios::beg);
    file->stream.ignore(std::numeric_limits<std::streamsize>::max());
    file->size = file->stream.gcount();
    file->stream.clear();
    file->stream.seekg(0, std::ios::beg);

    *handle = reinterpret_cast<HSFILE>(file);
    return 1;
}

int32_t STORMAPI SFileReadFile(HSFILE handle, void* buffer, uint32_t bytestoread, uint32_t* bytesread, LPOVERLAPPED overlapped) {
    (void)overlapped;

    if (!handle || !buffer) {
        if (bytesread) {
            *bytesread = 0;
        }
        return 0;
    }

    auto file = reinterpret_cast<NativeFileHandle*>(handle);
    file->stream.read(static_cast<char*>(buffer), bytestoread);

    if (bytesread) {
        *bytesread = static_cast<uint32_t>(file->stream.gcount());
    }

    return 1;
}

uint32_t STORMAPI SFileGetFileSize(HSFILE handle, uint32_t* filesizehigh) {
    if (!handle) {
        if (filesizehigh) {
            *filesizehigh = 0;
        }
        return 0;
    }

    auto file = reinterpret_cast<NativeFileHandle*>(handle);
    uint64_t size = static_cast<uint64_t>(file->size);

    if (filesizehigh) {
        *filesizehigh = static_cast<uint32_t>(size >> 32);
    }

    return static_cast<uint32_t>(size & 0xFFFFFFFFu);
}

uint32_t STORMAPI SFileSetFilePointer(HSFILE handle, int32_t distancetomove, int32_t* distancetomovehigh, uint32_t movemethod) {
    if (!handle) {
        if (distancetomovehigh) {
            *distancetomovehigh = 0;
        }
        return 0;
    }

    int64_t offset = static_cast<int64_t>(distancetomove);
    if (distancetomovehigh) {
        offset |= (static_cast<int64_t>(*distancetomovehigh) << 32);
    }

    std::ios_base::seekdir dir = std::ios::beg;
    switch (movemethod) {
        case 1:
            dir = std::ios::cur;
            break;
        case 2:
            dir = std::ios::end;
            break;
        default:
            dir = std::ios::beg;
            break;
    }

    auto file = reinterpret_cast<NativeFileHandle*>(handle);
    file->stream.clear();
    file->stream.seekg(static_cast<std::streamoff>(offset), dir);

    std::streamoff pos = file->stream.tellg();
    uint64_t upos = pos < 0 ? 0 : static_cast<uint64_t>(pos);

    if (distancetomovehigh) {
        *distancetomovehigh = static_cast<int32_t>(upos >> 32);
    }

    return static_cast<uint32_t>(upos & 0xFFFFFFFFu);
}

int32_t STORMAPI SFileCloseFile(HSFILE handle) {
    if (!handle) {
        return 0;
    }

    auto file = reinterpret_cast<NativeFileHandle*>(handle);
    file->stream.close();
    delete file;
    return 1;
}
