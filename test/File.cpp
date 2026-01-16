#include "FileTest.hpp"

// most of these only pass against storm.dll for now
#if defined(WHOA_TEST_STORMDLL)
TEST_CASE("SFileCloseArchive", "[file]") {
    HSARCHIVE archive = nullptr;

    SECTION("closes an archive") {
        SFileOpenArchive("wowtest1.mpq", 0, 0, &archive);
        REQUIRE(archive != nullptr);

        HSFILE file;
        CHECK(SFileOpenFileEx(nullptr, "test.txt", 0, &file));
        CHECK(SFileCloseFile(file));

        CHECK(SFileCloseArchive(archive) == 1);

        CHECK_FALSE(SFileOpenFileEx(nullptr, "test.txt", 0, &file));
    }

    // idk how to test this yet
    /*
    SECTION("doesn't delete if there is more than one reference") {
        // TODO
    }*/
}

TEST_CASE("SFileCloseFile", "[file]") {
    HSFILE file = nullptr;

    SECTION("closes a MPQ file") {
        file = ReadTestFileFromMpq();
        CHECK(SFileCloseFile(file) == 1);
    }

    SECTION("closes a filesystem file") {
        file = ReadTestFileFromDisk();
        CHECK(SFileCloseFile(file) == 1);
    }

    // idk how to test this yet
    /*
    SECTION("doesn't delete if there is more than one reference") {
        // TODO
    }*/
}

TEST_CASE("SFileGetFileSize", "[file]") {
    HSARCHIVE archive;
    HSFILE file;

    SECTION("retrieves MPQ file size") {
        SFileOpenArchive("wowtest1.mpq", 0, 0, &archive);

        REQUIRE(SFileOpenFileEx(archive, "test2.txt", 0, &file));
        CHECK(SFileGetFileSize(file) == 13);

        REQUIRE(SFileOpenFileEx(archive, "empty.txt", 0, &file));
        CHECK(SFileGetFileSize(file) == 0);

        uint32_t filesizehigh = 1234;
        REQUIRE(SFileOpenFileEx(archive, "test.txt", 0, &file));
        CHECK(SFileGetFileSize(file, &filesizehigh) == 6);
        CHECK(filesizehigh == 0);
    }

    SECTION("retrieves filesystem file size") {
        REQUIRE(SFileOpenFileEx(nullptr, "empty_diskonly.txt", SFILE_OPENFLAG_CHECKDISK, &file));
        CHECK(SFileGetFileSize(file) == 0);

        uint32_t filesizehigh = 1234;
        REQUIRE(SFileOpenFileEx(nullptr, "test_diskonly.txt", SFILE_OPENFLAG_CHECKDISK, &file));
        CHECK(SFileGetFileSize(file, &filesizehigh) == 6);
        CHECK(filesizehigh == 0);
    }
}

TEST_CASE("SFileOpenArchive", "[file]") {
    HSARCHIVE mpq = nullptr;
    SErrSetLastError(ERROR_SUCCESS);

    SECTION("opens a MPQ archive") {
        CHECK(SFileOpenArchive("wowtest1.mpq", 0, 0, &mpq) == 1);
        CHECK(mpq != nullptr);
    }

    /* // something about CD ROM drives? not sure how to test this
    SECTION("fails if drive is inaccessible") {
        // TODO
    }
    */

    SECTION("fails if archive file is nonexistent") {
        mpq = reinterpret_cast<HSARCHIVE>(1234);
        CHECK_FALSE(SFileOpenArchive("nice_try.mpq", 0, 0, &mpq));

        CHECK(mpq == nullptr);
        CHECK(SErrGetLastError() == ERROR_SUCCESS);
    }

    SECTION("fails if archive file is too small") {
        mpq = reinterpret_cast<HSARCHIVE>(1234);
        CHECK_FALSE(SFileOpenArchive("bad_toosmall.mpq", 0, 0, &mpq));
        CHECK(mpq == nullptr);

        CHECK(SErrGetLastError() == STORM_ERROR_NOT_ARCHIVE);
    }

    SECTION("fails if using a directory") {
        mpq = reinterpret_cast<HSARCHIVE>(1234);
        CHECK_FALSE(SFileOpenArchive("directorytest", 0, 0, &mpq));
        CHECK(mpq == nullptr);

        CHECK(SErrGetLastError() == ERROR_SUCCESS);
    }

    SECTION("fails if archive header magic doesn't match") {
        mpq = reinterpret_cast<HSARCHIVE>(1234);
        CHECK_FALSE(SFileOpenArchive("bad_nomagic.mpq", 0, 0, &mpq));
        CHECK(mpq == nullptr);

        CHECK(SErrGetLastError() == STORM_ERROR_NOT_ARCHIVE);
    }

    SECTION("fails if archive header size doesn't match") {
        mpq = reinterpret_cast<HSARCHIVE>(1234);
        CHECK_FALSE(SFileOpenArchive("bad_headertoosmall.mpq", 0, 0, &mpq));
        CHECK(mpq == nullptr);

        CHECK(SErrGetLastError() == STORM_ERROR_NOT_ARCHIVE);
    }
}

TEST_CASE("SFileOpenFileEx", "[file]") {
    SECTION("shared testcases") {
        OpenFileTestCase testcase = GENERATE(OpenFromDiskCase, OpenFromMPQCase);

        INFO(testcase.info);
        HSARCHIVE archive = testcase.OpenArchiveFn();
        HSFILE file = nullptr;

        SECTION("opens a file") {
            CHECK(SFileOpenFileEx(archive, "test.txt", testcase.flags, &file) == 1);
            CHECK(file != nullptr);
            CHECK(file != reinterpret_cast<HSFILE>(1234));
        }

        SECTION("fails if file not found") {
            SErrSetLastError(ERROR_SUCCESS);
            file = reinterpret_cast<HSFILE>(1234);
            CHECK_FALSE(SFileOpenFileEx(archive, "nice try buddy but your file is in another castle", testcase.flags, &file));
            CHECK(file == nullptr);
            CHECK(SErrGetLastError() == ERROR_FILE_NOT_FOUND);
        }
    }

    SECTION("fails when trying to open a directory") {
        SErrSetLastError(ERROR_SUCCESS);
        HSFILE file = reinterpret_cast<HSFILE>(1234);
        CHECK_FALSE(SFileOpenFileEx(nullptr, "directorytest", SFILE_OPENFLAG_CHECKDISK, &file));
        CHECK(file == nullptr);
        CHECK(SErrGetLastError() == ERROR_FILE_NOT_FOUND);
    }

    SECTION("mpq testcases") {
        HSARCHIVE mpq1, mpq2, mpq3;
        HSFILE file;

        SECTION("opens the highest priority file from all MPQs") {
            SFileOpenArchive("wowtest1.mpq", 100, 0, &mpq1);
            SFileOpenArchive("wowtest2.mpq", 500, 0, &mpq2);
            SFileOpenArchive("wowtest3.mpq", 400, 0, &mpq3);

            CHECK(SFileOpenFileEx(nullptr, "test.txt", 0, &file) == 1);

            char result[16] = {};
            SFileReadFile(file, result, sizeof(result));

            CHECK(std::string(result) == "skibbidy");
        }

        SECTION("opens the most recently opened file from same priority MPQs") {
            SFileOpenArchive("wowtest1.mpq", 100, 0, &mpq1);
            SFileOpenArchive("wowtest2.mpq", 500, 0, &mpq2);
            SFileOpenArchive("wowtest3.mpq", 500, 0, &mpq3);

            CHECK(SFileOpenFileEx(nullptr, "test.txt", 0, &file) == 1);

            char result[16] = {};
            SFileReadFile(file, result, sizeof(result));

            CHECK(std::string(result) == "rizz");
        }

        /*
        SECTION("opens the file for the currently selected locale") {
            // TODO
        }
        */
        SECTION("fails if file not found in target MPQ") {
            SFileOpenArchive("wowtest1.mpq", 100, 0, &mpq1);
            SFileOpenArchive("wowtest2.mpq", 500, 0, &mpq2);
            SFileOpenArchive("wowtest3.mpq", 400, 0, &mpq3);

            CHECK(SFileOpenFileEx(nullptr, "test2.txt", 0, &file) == 1);
            SFileCloseFile(file);

            SErrSetLastError(ERROR_SUCCESS);
            CHECK_FALSE(SFileOpenFileEx(mpq3, "test2.txt", 0, &file));
            CHECK(SErrGetLastError() == ERROR_FILE_NOT_FOUND);
        }

        SECTION("fails if file not found in any MPQ") {
            SFileOpenArchive("wowtest1.mpq", 100, 0, &mpq1);
            SFileOpenArchive("wowtest2.mpq", 500, 0, &mpq2);
            SFileOpenArchive("wowtest3.mpq", 400, 0, &mpq3);

            SErrSetLastError(ERROR_SUCCESS);
            CHECK_FALSE(SFileOpenFileEx(nullptr, "yep not here", 0, &file));
            CHECK(SErrGetLastError() == ERROR_FILE_NOT_FOUND);
        }

        SECTION("can open attributes file") {
            SFileOpenArchive("wowtest1.mpq", 100, 0, &mpq1);
            CHECK(SFileOpenFileEx(nullptr, "(attributes)", 0, &file) == 1);
        }

        SECTION("can open listfile") {
            SFileOpenArchive("wowtest1.mpq", 100, 0, &mpq1);
            CHECK(SFileOpenFileEx(nullptr, "(listfile)", 0, &file) == 1);
        }
    }
}

TEST_CASE("SFileReadFile", "[file]") {
    SECTION("shared testcases") {
        ReadFileTestCase testcase = GENERATE(ReadFromDiskCase, ReadFromMPQCase);

        INFO(testcase.info);
        HSFILE file = testcase.OpenFileFn();
        REQUIRE(file != nullptr);

        SECTION("reads a file") {
            char buffer[32] = {};
            CHECK(SFileReadFile(file, buffer, 6, nullptr, nullptr) == 1);
            CHECK(std::string(buffer) == "catdog");
        }

        SECTION("reads partial file if bytestoread is too small") {
            char buffer[4] = "";
            CHECK(SFileReadFile(file, &buffer, 3, nullptr, nullptr) == 1);
            CHECK(std::string(buffer) == "cat");
        }

        SECTION("continues reading from the last stored position") {
            char buffer[4] = "";
            CHECK(SFileReadFile(file, &buffer, 3, nullptr, nullptr) == 1);
            CHECK(SFileReadFile(file, &buffer, 3, nullptr, nullptr) == 1);
            CHECK(std::string(buffer) == "dog");
        }

        SECTION("continues reading from an explicitly set position") {
            char buffer[4] = "";
            SFileSetFilePointer(file, 3, nullptr, SFILE_BEGIN);
            CHECK(SFileReadFile(file, &buffer, 3, nullptr, nullptr) == 1);
            CHECK(std::string(buffer) == "dog");
        }

        SECTION("succeeds if bytestoread is 0") {
            char buffer;
            CHECK(SFileReadFile(file, &buffer, 0, nullptr, nullptr) == 1);

            uint32_t read = 42;
            CHECK(SFileReadFile(file, &buffer, 0, &read, nullptr) == 1);
            CHECK(read == 0);
        }

        SECTION("succeeds if bytestoread is 0 past eof") {
            char buffer[8];
            CHECK_FALSE(SFileReadFile(file, &buffer, 8, nullptr, nullptr));

            uint32_t read = 42;
            CHECK(SFileReadFile(file, &buffer, 0, &read, nullptr) == 1);
            CHECK(read == 0);
        }

        SECTION("fails when reading past end of file") {
            char buffer[8] = "";
            CHECK(SFileReadFile(file, &buffer, 6, nullptr, nullptr) == 1);
            CHECK(std::string(buffer) == "catdog");

            SErrSetLastError(0);
            CHECK_FALSE(SFileReadFile(file, &buffer, 1, nullptr, nullptr));
            CHECK(SErrGetLastError() == testcase.eofcode);

            CHECK(std::string(buffer) == "catdog");
        }

        SECTION("fails if bytestoread is larger than file size when reading from disk") {
            char buffer[32] = "";

            SErrSetLastError(0);
            CHECK_FALSE(SFileReadFile(file, buffer, sizeof(buffer), nullptr, nullptr));
            CHECK(SErrGetLastError() == testcase.eofcode);

            CHECK(std::string(buffer) == "catdog");
        }
    }
}

TEST_CASE("SFileSetFilePointer", "[file]") {
    SECTION("shared testcases") {
        ReadFileTestCase testcase = GENERATE(ReadFromDiskCase, ReadFromMPQCase);

        INFO(testcase.info);
        HSFILE file = testcase.OpenFileFn();
        REQUIRE(file != nullptr);

        SECTION("sets position from beginning") {
            CHECK(SFileSetFilePointer(file, 2, nullptr, SFILE_BEGIN) == 2);
            CHECK(SFileSetFilePointer(file, 0, nullptr, SFILE_BEGIN) == 0);
            CHECK(SFileSetFilePointer(file, 5, nullptr, SFILE_BEGIN) == 5);
        }

        SECTION("returns the cursor position") {
            CHECK(SFileSetFilePointer(file, 2, nullptr, SFILE_BEGIN) == 2);
            CHECK(SFileSetFilePointer(file, 1, nullptr, SFILE_BEGIN) == 1);
            CHECK(SFileSetFilePointer(file, 5, nullptr, SFILE_BEGIN) == 5);
            CHECK(SFileSetFilePointer(file, -4, nullptr, SFILE_CURRENT) == 1);
            CHECK(SFileSetFilePointer(file, 0, nullptr, SFILE_BEGIN) == 0);
        }

        SECTION("sets position from current") {
            CHECK(SFileSetFilePointer(file, 0, nullptr, SFILE_CURRENT) == 0);
            CHECK(SFileSetFilePointer(file, 1, nullptr, SFILE_CURRENT) == 1);
            CHECK(SFileSetFilePointer(file, 1, nullptr, SFILE_CURRENT) == 2);
            CHECK(SFileSetFilePointer(file, 1, nullptr, SFILE_CURRENT) == 3);
            CHECK(SFileSetFilePointer(file, 0, nullptr, SFILE_CURRENT) == 3);
            CHECK(SFileSetFilePointer(file, -2, nullptr, SFILE_CURRENT) == 1);
            CHECK(SFileSetFilePointer(file, -1, nullptr, SFILE_CURRENT) == 0);
        }
    }

    SECTION("cases from filesystem") {
        // Probably Windows specific
        HSFILE file = ReadTestFileFromDisk();
        REQUIRE(file != nullptr);

        SECTION("sets position from beginning out of bounds") {
            CHECK(SFileSetFilePointer(file, -1, nullptr, SFILE_BEGIN) == -1);
            CHECK(SFileSetFilePointer(file, -100, nullptr, SFILE_BEGIN) == -1);
            CHECK(SFileSetFilePointer(file, 6, nullptr, SFILE_BEGIN) == 6);
            CHECK(SFileSetFilePointer(file, 100, nullptr, SFILE_BEGIN) == 100);
        }

        SECTION("sets position from current out of bounds") {
            CHECK(SFileSetFilePointer(file, -1, nullptr, SFILE_CURRENT) == -1);
            CHECK(SFileSetFilePointer(file, -100, nullptr, SFILE_CURRENT) == -1);
            CHECK(SFileSetFilePointer(file, 106, nullptr, SFILE_CURRENT) == 106);
            CHECK(SFileSetFilePointer(file, 100, nullptr, SFILE_CURRENT) == 206);
        }

        SECTION("sets position from end") {
            CHECK(SFileSetFilePointer(file, -5, nullptr, SFILE_END) == 1);
            CHECK(SFileSetFilePointer(file, -2, nullptr, SFILE_END) == 4);
            CHECK(SFileSetFilePointer(file, -1, nullptr, SFILE_END) == 5);
            CHECK(SFileSetFilePointer(file, 0, nullptr, SFILE_END) == 6);
        }

        SECTION("sets position from end out of bounds") {
            CHECK(SFileSetFilePointer(file, -100, nullptr, SFILE_END) == -1);
            CHECK(SFileSetFilePointer(file, -10, nullptr, SFILE_END) == -1);
            CHECK(SFileSetFilePointer(file, 1, nullptr, SFILE_END) == 7);
            CHECK(SFileSetFilePointer(file, 5, nullptr, SFILE_END) == 11);
        }
    }

    SECTION("cases from MPQ") {
        HSFILE file = ReadTestFileFromMpq();
        REQUIRE(file != nullptr);

        SECTION("sets position from beginning out of bounds") {
            CHECK(SFileSetFilePointer(file, -1, nullptr, SFILE_BEGIN) == 5);
            CHECK(SFileSetFilePointer(file, -100, nullptr, SFILE_BEGIN) == 5);
            CHECK(SFileSetFilePointer(file, 6, nullptr, SFILE_BEGIN) == 5);
            CHECK(SFileSetFilePointer(file, 100, nullptr, SFILE_BEGIN) == 5);
        }

        SECTION("sets position from current out of bounds") {
            CHECK(SFileSetFilePointer(file, -1, nullptr, SFILE_CURRENT) == 0);
            CHECK(SFileSetFilePointer(file, -100, nullptr, SFILE_CURRENT) == 0);
            CHECK(SFileSetFilePointer(file, 106, nullptr, SFILE_CURRENT) == 5);
            CHECK(SFileSetFilePointer(file, 100, nullptr, SFILE_CURRENT) == 5);
        }

        SECTION("sets position from end") {
            CHECK(SFileSetFilePointer(file, -5, nullptr, SFILE_END) == 1);
            CHECK(SFileSetFilePointer(file, -2, nullptr, SFILE_END) == 4);
            CHECK(SFileSetFilePointer(file, -1, nullptr, SFILE_END) == 5);
            CHECK(SFileSetFilePointer(file, 0, nullptr, SFILE_END) == 5);
        }

        SECTION("sets position from end out of bounds") {
            CHECK(SFileSetFilePointer(file, -100, nullptr, SFILE_END) == 0);
            CHECK(SFileSetFilePointer(file, -10, nullptr, SFILE_END) == 0);
            CHECK(SFileSetFilePointer(file, 1, nullptr, SFILE_END) == 5);
            CHECK(SFileSetFilePointer(file, 5, nullptr, SFILE_END) == 5);
        }
    }
}
#endif
