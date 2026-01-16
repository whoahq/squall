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

#define SFILE_ARCHIVE_READ_FROM_CD_ONLY         0x000001
#define SFILE_ARCHIVE_ENABLE_OVERLAPPED         0x000002
#define SFILE_ARCHIVE_DONT_CHECKDISK            0x000004
#define SFILE_ARCHIVE_DONT_SEARCH               0x000008
#define SFILE_ARCHIVE_ARC4_DECRYPT              0x000010
#define SFILE_ARCHIVE_DECRYPTION_TYPES          0x000010
#define SFILE_ARCHIVE_WRITE_PERMISSION          0x000010
#define SFILE_ARCHIVE_OPEN_LAST_ARCHIVE         0x000020
#define SFILE_ARCHIVE_LOAD_MD5_VALUES           0x000040
#define SFILE_ARCHIVE_LOAD_CRC_VALUES           0x000080
#define SFILE_ARCHIVE_LOAD_TIMESTAMPS           0x000100
#define SFILE_ARCHIVE_CHECK_MD5_VALUES          0x000200
#define SFILE_ARCHIVE_USE_NEW_BLOCK_HASH_FORMAT 0x000400
#define SFILE_ARCHIVE_INIT_TABLE_ON_OPEN        0x000800
#define SFILE_ARCHIVE_LOAD_DELTA_VALUES         0x001000
#define SFILE_ARCHIVE_LOAD_DELTA_AS_RAW         0x002000
#define SFILE_ARCHIVE_DONT_TRUNCATE             0x004000
#define SFILE_ARCHIVE_DISK_DELETE_CAN_FAIL      0x010000
#define SFILE_ARCHIVE_SC1161_PERMISSIVE         0x020000

#define SFILE_OPENFLAG_CHECKDISK 1
#define SFILE_OPENFLAG_CHECKDISK_NOPATH 2
#define SFILE_OPENFLAG_NATIVEHANDLE 4  // made up
#define SFILE_OPENFLAG_PERM_SHARED_WRITE 0x8000
#define SFILE_OPENFLAG_PRESERVE_PATH_SEPARATORS 0x10000

#define SFILE_BEGIN 0
#define SFILE_CURRENT 1
#define SFILE_END 2

/* // Leaving as documentation

#define SFILE_AUTH_UNABLETOAUTHENTICATE 0
#define SFILE_AUTH_NOSIGNATURE 1
#define SFILE_AUTH_BADSIGNATURE 2
#define SFILE_AUTH_UNKNOWNSIGNATURE 3
#define SFILE_AUTH_FIRSTAUTHENTIC 5
#define SFILE_AUTH_AUTHENTICBLIZZARD 5

#define SFILE_DIRECT_ENABLE_RELATIVE 1
#define SFILE_DIRECT_ENABLE_NOPATH 2

#define SFILE_PLATFORM_ANY 0
#define SFILE_PLATFORM_WIN32 1
#define SFILE_PLATFORM_MAC 2

enum SARCHIVE_TYPE {
    SARCHIVE_MPQ,
    SARCHIVE_ZIP,
};

enum SFILE_TYPE {
    SFILE_PLAIN,
    SFILE_COMPRESSED,
    SFILE_PAQ,
    SFILE_OLD_SFILE,
    SFILE_ZIP_FILE,
};
*/

int32_t STORMAPI SFileCloseArchive(HSARCHIVE handle);

int32_t STORMAPI SFileCloseFile(HSFILE handle);

uint32_t STORMAPI SFileGetFileSize(HSFILE handle, uint32_t* filesizehigh = nullptr);

int32_t STORMAPI SFileOpenArchive(const char* archivename, int32_t priority, uint32_t flags, HSARCHIVE* handle);

int32_t STORMAPI SFileOpenFileEx(HSARCHIVE archivehandle, const char* filename, uint32_t flags, HSFILE* handle);

int32_t STORMAPI SFileReadFile(HSFILE handle, void* buffer, uint32_t bytestoread, uint32_t* bytesread = nullptr, LPOVERLAPPED overlapped = nullptr);

uint32_t STORMAPI SFileSetFilePointer(HSFILE handle, int32_t distancetomove, int32_t* distancetomovehigh, uint32_t movemethod);

#endif
