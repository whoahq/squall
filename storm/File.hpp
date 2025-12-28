#ifndef STORM_FILE_HPP
#define STORM_FILE_HPP

#include "storm/Core.hpp"
#include "storm/Handle.hpp"
#include <cstdint>

#if defined(WHOA_SYSTEM_WIN)
#include <Windows.h>
#endif

DECLARE_STORM_HANDLE(HSARCHIVE);
DECLARE_STORM_HANDLE(HSFILE);

int32_t STORMAPI SFileOpenArchive(const char* archivename, int32_t priority, uint32_t flags, HSARCHIVE* handle);

int32_t STORMAPI SFileCloseArchive(HSARCHIVE handle);

int32_t STORMAPI SFileOpenFileEx(HSARCHIVE archivehandle, const char* filename, uint32_t flags, HSFILE* handle);

int32_t STORMAPI SFileReadFile(HSFILE handle, void* buffer, uint32_t bytestoread, uint32_t* bytesread, LPOVERLAPPED overlapped);

uint32_t STORMAPI SFileGetFileSize(HSFILE handle, uint32_t* filesizehigh);

uint32_t STORMAPI SFileSetFilePointer(HSFILE handle, int32_t distancetomove, int32_t* distancetomovehigh, uint32_t movemethod);

int32_t STORMAPI SFileCloseFile(HSFILE handle);

#endif
