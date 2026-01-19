#include "Test.hpp"
#include "storm/Error.hpp"
#include "storm/File.hpp"

#include <string>

struct OpenFileTestCase {
    std::string info;
    HSARCHIVE (*OpenArchiveFn)();
    uint32_t flags;
};

struct ReadFileTestCase {
    std::string info;
    HSFILE (*OpenFileFn)();
    uint32_t eofcode;
};

HSARCHIVE OpenNullArchive();
HSARCHIVE OpenTestArchive();

HSFILE ReadTestFileFromDisk();
HSFILE ReadTestFileFromMpq();

extern OpenFileTestCase OpenFromDiskCase;
extern OpenFileTestCase OpenFromMPQCase;

extern ReadFileTestCase ReadFromDiskCase;
extern ReadFileTestCase ReadFromMPQCase;
