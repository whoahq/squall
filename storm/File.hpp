#ifndef STORM_FILE_HPP
#define STORM_FILE_HPP

#include <cstdint>
#include "storm/Core.hpp"

#if defined(WHOA_SYSTEM_WIN)
#include <Windows.h>
#else
typedef void* HANDLE;
typedef uint32_t DWORD;
typedef uint32_t* LPDWORD;
typedef int32_t LONG;
typedef void* LPOVERLAPPED;
#endif

bool STORMAPI SFileOpenArchive(const char* mpqName, DWORD priority, DWORD flags, HANDLE* archive);
bool STORMAPI SFileCloseArchive(HANDLE archive);
bool STORMAPI SFileOpenFileEx(HANDLE archive, const char* fileName, DWORD searchScope, HANDLE* file);
bool STORMAPI SFileReadFile(HANDLE file, void* buffer, DWORD toRead, LPDWORD read, LPOVERLAPPED overlapped);
DWORD STORMAPI SFileGetFileSize(HANDLE file, LPDWORD fileSizeHigh);
DWORD STORMAPI SFileSetFilePointer(HANDLE file, LONG filePos, LONG* filePosHigh, DWORD moveMethod);
bool STORMAPI SFileCloseFile(HANDLE file);

#endif
