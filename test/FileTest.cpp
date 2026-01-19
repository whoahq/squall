#include "FileTest.hpp"

HSARCHIVE OpenNullArchive() {
    return nullptr;
}

HSARCHIVE OpenTestArchive() {
    HSARCHIVE archive = nullptr;
    SFileOpenArchive("wowtest1.mpq", 0, 0, &archive);
    REQUIRE(archive != nullptr);
    return archive;
}

HSFILE ReadTestFileFromDisk() {
    HSFILE file = nullptr;
    SFileOpenFileEx(nullptr, "test_diskonly.txt", SFILE_OPENFLAG_CHECKDISK, &file);
    return file;
}

HSFILE ReadTestFileFromMpq() {
    HSARCHIVE archive = nullptr;
    SFileOpenArchive("wowtest1.mpq", 0, 0, &archive);

    HSFILE file = nullptr;
    SFileOpenFileEx(archive, "test.txt", 0, &file);
    return file;
}

OpenFileTestCase OpenFromDiskCase{ "file from disk", OpenNullArchive, SFILE_OPENFLAG_CHECKDISK };
OpenFileTestCase OpenFromMPQCase{ "file from MPQ", OpenTestArchive, 0 };

ReadFileTestCase ReadFromDiskCase{ "file from disk", ReadTestFileFromDisk, ERROR_SUCCESS };
ReadFileTestCase ReadFromMPQCase{ "file from MPQ", ReadTestFileFromMpq, ERROR_HANDLE_EOF };
