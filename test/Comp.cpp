#include "Test.hpp"
#include "storm/Comp.hpp"
#include "storm/Error.hpp"

#include <fstream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

typedef std::vector<char> datastream;

datastream ReadBinary(const std::string& filename) {
    std::ifstream f("file/" + filename, std::ios::binary);
    if (!f) {
        FAIL("Unable to read file " << filename);
    }
    return datastream(std::istreambuf_iterator<char>(f), {});
}

struct CompSettings {
    std::string prefix;
    uint32_t comp, hint, opt;

    bool operator <(const CompSettings& other) const {
        return std::tie(comp, hint, opt) < std::tie(other.comp, other.hint, other.opt);
    }
};

struct CompTestCase {
    std::string filename;
    std::vector<CompSettings> settings;
};

const CompSettings ADPCM_MONO_0{ "ADPCM_Mono_0_", SCOMP_TYPE_LOSSY_ADPCM_MONO, SCOMP_HINT_NONE, 0 };
const CompSettings ADPCM_MONO_1{ "ADPCM_Mono_1_", SCOMP_TYPE_LOSSY_ADPCM_MONO, SCOMP_HINT_NONE, 1 };
const CompSettings ADPCM_MONO_2{ "ADPCM_Mono_2_", SCOMP_TYPE_LOSSY_ADPCM_MONO, SCOMP_HINT_NONE, 2 };
const CompSettings ADPCM_STEREO_0{ "ADPCM_Stereo_0_", SCOMP_TYPE_LOSSY_ADPCM_STEREO, SCOMP_HINT_NONE, 0 };
const CompSettings ADPCM_STEREO_1{ "ADPCM_Stereo_1_", SCOMP_TYPE_LOSSY_ADPCM_STEREO, SCOMP_HINT_NONE, 1 };
const CompSettings ADPCM_STEREO_2{ "ADPCM_Stereo_2_", SCOMP_TYPE_LOSSY_ADPCM_STEREO, SCOMP_HINT_NONE, 2 };

const CompSettings HUFFMAN_NONE{ "Huffman_None_", SCOMP_TYPE_HUFFMAN, SCOMP_HINT_NONE };
const CompSettings HUFFMAN_BINARY{ "Huffman_Binary_", SCOMP_TYPE_HUFFMAN, SCOMP_HINT_BINARY };
const CompSettings HUFFMAN_TEXT{ "Huffman_Text_", SCOMP_TYPE_HUFFMAN, SCOMP_HINT_TEXT };
const CompSettings HUFFMAN_EXE{ "Huffman_Exe_", SCOMP_TYPE_HUFFMAN, SCOMP_HINT_EXECUTABLE };
const CompSettings HUFFMAN_WAV8MONO{ "Huffman_wav8mono_", SCOMP_TYPE_HUFFMAN, SCOMP_HINT_ADPCM_8_MONO };
const CompSettings HUFFMAN_WAV8STEREO{ "Huffman_wav8stereo_", SCOMP_TYPE_HUFFMAN, SCOMP_HINT_ADPCM_8_STEREO };
const CompSettings HUFFMAN_WAV16MONO{ "Huffman_wav16mono_", SCOMP_TYPE_HUFFMAN, SCOMP_HINT_ADPCM_16_MONO };
const CompSettings HUFFMAN_WAV16STEREO{ "Huffman_wav16stereo_", SCOMP_TYPE_HUFFMAN, SCOMP_HINT_ADPCM_16_STEREO };
const CompSettings HUFFMAN_WAV16STEREO2{ "Huffman_wav16stereo2_", SCOMP_TYPE_HUFFMAN, SCOMP_HINT_ADPCM_16_STEREO2 };

const CompSettings PKWARE{ "PKWare_", SCOMP_TYPE_PKWARE, SCOMP_HINT_NONE };
const CompSettings PKWARE_TEXT{ "PKWare_Text_", SCOMP_TYPE_PKWARE, SCOMP_HINT_TEXT };

const CompSettings ADPCM_MONO_HUFFMAN{ "ADPCM_Mono_Huffman_", SCOMP_TYPE_LOSSY_ADPCM_MONO | SCOMP_TYPE_HUFFMAN, SCOMP_HINT_ADPCM_16_MONO };
const CompSettings ADPCM_STEREO_HUFFMAN{ "ADPCM_Stereo_Huffman_", SCOMP_TYPE_LOSSY_ADPCM_STEREO | SCOMP_TYPE_HUFFMAN, SCOMP_HINT_ADPCM_16_STEREO };

// Note: Can't test ZLIB compression due to discrepancies between versions being depended on producing different results.
// In order to handle it we would need to change the zlib version depending on which game+version squall is targetting.
// This is doable but needs a separate change.
// Decompression is still testable.

const CompSettings ZLIB_DEFAULT{ "ZLIB_Default_", SCOMP_TYPE_ZLIB, SCOMP_HINT_NONE, 0 };
const CompSettings ZLIB_9{ "ZLIB_9_", SCOMP_TYPE_ZLIB, SCOMP_HINT_NONE, 1 };
const CompSettings ZLIB_1{ "ZLIB_1_", SCOMP_TYPE_ZLIB, SCOMP_HINT_NONE, 2 };

// Note: StormLib fails on compressing/decompressing EVERYTHING_SC1 - don't care to investigate since it would never be used
const CompSettings EVERYTHING_SC1{ "Everything_SC1_", SCOMP_TYPE_PKWARE | SCOMP_TYPE_HUFFMAN | SCOMP_TYPE_LOSSY_ADPCM_MONO | SCOMP_TYPE_LOSSY_ADPCM_STEREO };

#define ALL_ADPCM ADPCM_MONO_0, ADPCM_MONO_1, ADPCM_MONO_2, ADPCM_STEREO_0, ADPCM_STEREO_1, ADPCM_STEREO_2
#define ALL_PKWARE PKWARE, PKWARE_TEXT
#define ALL_HUFFMAN_BINARY HUFFMAN_NONE, HUFFMAN_BINARY, HUFFMAN_EXE, HUFFMAN_WAV8MONO, HUFFMAN_WAV8STEREO, HUFFMAN_WAV16MONO, HUFFMAN_WAV16STEREO, HUFFMAN_WAV16STEREO2
#define ALL_HUFFMAN ALL_HUFFMAN_BINARY, HUFFMAN_TEXT
#define ALL_ADPCM_COMBOS ADPCM_MONO_HUFFMAN, ADPCM_STEREO_HUFFMAN

#if defined(WHOA_SCOMP_NO_ZLIB)
#define ALL_ZLIB
#else
#define ALL_ZLIB ZLIB_DEFAULT, ZLIB_1, ZLIB_9
#endif

#define ALL_SETTINGS ALL_ADPCM, ALL_PKWARE, ALL_HUFFMAN, ALL_ADPCM_COMBOS /*, ALL_ZLIB*/

std::set<CompSettings> InvertSettings(const std::vector<CompSettings>& settings) {
    std::set<CompSettings> result {ALL_SETTINGS};
    for (auto& setting : settings) {
        result.erase(setting);
    }
    return result;
}

TEST_CASE("SCompCompress", "[comp]") {
    SECTION("fails silently with invalid compression type") {
        SErrSetLastError(ERROR_SUCCESS);
        datastream buff(16);
        uint32_t size = 16;
        CHECK_FALSE(SCompCompress(buff.data(), &size, "poop", 4, 0x990000, 0, 0));
        CHECK(SErrGetLastError() == ERROR_SUCCESS);
    }

    SECTION("doesn't compress when a compression type is unspecified") {
        std::string content = GENERATE("a", "memes, cats, and chocolate cake yummy");
        std::string content_copy = content; // Don't give the function a chance to overwrite the original content

        datastream dest(content.size());
        uint32_t destsize = static_cast<uint32_t>(dest.size());
        REQUIRE(SCompCompress(dest.data(), &destsize, content_copy.data(), static_cast<uint32_t>(content_copy.size()), 0, 0, 0) == 1);

        std::string result{dest.data(), destsize};
        CHECK(content_copy == content);
        CHECK(result == content);
    }

    SECTION("with overlapping source and dest") {
        SECTION("succeeds when dest == source") {
            datastream data = ReadBinary("all_ascii.txt");
            uint32_t destsize = static_cast<uint32_t>(data.size());
            REQUIRE(SCompCompress(data.data(), &destsize, data.data(), destsize, SCOMP_TYPE_PKWARE, SCOMP_HINT_TEXT, 0) == 1);
            data.resize(destsize);

            datastream expected = ReadBinary("compressed/PKWare_Text_all_ascii.txt.bin");
            REQUIRE(destsize == expected.size());
            CHECK(data == expected);
        }

        SECTION("succeeds when dest > source") {
            uint32_t offset = GENERATE(1, 2, 10);
            datastream data = ReadBinary("all_ascii.txt");
            uint32_t srcsize = static_cast<uint32_t>(data.size());
            data.insert(data.end(), offset, 0);

            uint32_t destsize = static_cast<uint32_t>(data.size());
            REQUIRE(SCompCompress(data.data() + offset, &destsize, data.data(), srcsize, SCOMP_TYPE_PKWARE, SCOMP_HINT_TEXT, 0) == 1);
            datastream data_copy(data.data() + offset, data.data() + offset + destsize);

            datastream expected = ReadBinary("compressed/PKWare_Text_all_ascii.txt.bin");
            REQUIRE(destsize == expected.size());
            CHECK(data_copy == expected);
        }

        SECTION("succeeds when dest < source") {
            uint32_t offset = GENERATE(1, 2, 10);
            datastream data = ReadBinary("all_ascii.txt");
            uint32_t srcsize = static_cast<uint32_t>(data.size());
            data.insert(data.begin(), offset, 0);

            uint32_t destsize = static_cast<uint32_t>(data.size()) - offset;
            REQUIRE(SCompCompress(data.data(), &destsize, data.data() + offset, srcsize, SCOMP_TYPE_PKWARE, SCOMP_HINT_TEXT, 0) == 1);
            data.resize(destsize);

            datastream expected = ReadBinary("compressed/PKWare_Text_all_ascii.txt.bin");
            REQUIRE(destsize == expected.size());
            CHECK(data == expected);
        }
    }

    SECTION("normal operation") {
        auto testcase = GENERATE(
            CompTestCase{"all_ascii.txt", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS}},
            CompTestCase{"all_null.bin", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, ALL_HUFFMAN_BINARY}},
            CompTestCase{"all_same_ascii.txt", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE, HUFFMAN_BINARY, HUFFMAN_TEXT}},
            CompTestCase{"newlines.txt", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_TEXT, HUFFMAN_WAV8MONO, HUFFMAN_WAV8STEREO, HUFFMAN_WAV16STEREO, HUFFMAN_WAV16STEREO2, HUFFMAN_BINARY, HUFFMAN_NONE}},
            CompTestCase{"scenario.chk", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, ALL_HUFFMAN_BINARY}},
            CompTestCase{"test_11025_8_mono.wav", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE}},
            CompTestCase{"test_11025_8_stereo.wav", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE}},
            CompTestCase{"test_11025_16_mono.wav", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE, HUFFMAN_BINARY, HUFFMAN_EXE}},
            CompTestCase{"test_11025_16_stereo.wav", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE, HUFFMAN_BINARY, HUFFMAN_EXE}},
            CompTestCase{"test_44100_8_mono.wav", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE}},
            CompTestCase{"test_44100_8_stereo.wav", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE}},
            CompTestCase{"test_44100_16_mono.wav", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE, HUFFMAN_BINARY, HUFFMAN_EXE}},
            CompTestCase{"test_44100_16_stereo.wav", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE, HUFFMAN_BINARY, HUFFMAN_EXE}},
            CompTestCase{"smallest_null.bin", {HUFFMAN_NONE}},
            CompTestCase{"smallest_ascii.txt", {HUFFMAN_NONE}},
            CompTestCase{"3bytes.bin", {}}, // These don't compress under any circumstances
            CompTestCase{"1byte.bin", {}}
        );

        DYNAMIC_SECTION(testcase.filename) {
            SECTION("compression") {
                for (auto& setting : testcase.settings) {
                    DYNAMIC_SECTION(setting.prefix) {
                        datastream data = ReadBinary(testcase.filename);
                        datastream dest(data.size());
                        uint32_t destsize = static_cast<uint32_t>(dest.size());
                        REQUIRE(SCompCompress(dest.data(), &destsize, data.data(), static_cast<uint32_t>(data.size()), setting.comp, setting.hint, setting.opt) == 1);
                        dest.resize(destsize);

                        datastream expected = ReadBinary("compressed/" + setting.prefix + testcase.filename + ".bin");
                        REQUIRE(destsize == expected.size());
                        CHECK(dest == expected);
                    }
                }
            }

            // case where the "compressed" size is greater than the original size
            SECTION("compressed size too large") {
                for (auto& setting : InvertSettings(testcase.settings)) {
                    DYNAMIC_SECTION(setting.prefix) {
                        datastream data = ReadBinary(testcase.filename);
                        datastream dest(data.size());
                        uint32_t destsize = static_cast<uint32_t>(dest.size());
                        REQUIRE(SCompCompress(dest.data(), &destsize, data.data(), static_cast<uint32_t>(data.size()), setting.comp, setting.hint, setting.opt) == 1);
                        dest.resize(destsize);

                        datastream expected = ReadBinary(testcase.filename);
                        REQUIRE(destsize == expected.size());
                        CHECK(dest == expected);
                    }
                }
            }
        }
    }

    // NOTE: buffer underrun when source size is 0 in actual storm.dll
    /*
    SECTION("succeeds when source size is 0") {
        datastream data(1);
        uint32_t destsize = 1;
        CHECK(SCompCompress(data.data(), &destsize, data.data(), 0, SCOMP_TYPE_PKWARE, 0, 0) == 1);
        CHECK(destsize == 0);
    }*/

    // TODO: BZIP2, ZLIB, RLE, ZLIB+RLE, BZIP2+RLE, non-WOW combos
}

TEST_CASE("SCompDecompress", "[comp]") {
    SECTION("copies as uncompressed when source and dest are the same size") {
        std::string content = GENERATE("a", "memes, cats, and chocolate cake yummy");
        std::string content_copy = content;

        datastream dest(content.size());
        uint32_t destsize = static_cast<uint32_t>(dest.size());
        REQUIRE(SCompDecompress(dest.data(), &destsize, content_copy.data(), static_cast<uint32_t>(content_copy.size())) == 1);

        std::string result{dest.data(), destsize};
        CHECK(content_copy == content); // Ensure original content wasn't tampered with
        CHECK(result == content);
    }

    SECTION("with overlapping source and dest") {
        SECTION("succeeds when dest == source") {
            datastream data = ReadBinary("compressed/PKWare_Text_all_ascii.txt.bin");
            uint32_t srcsize = static_cast<uint32_t>(data.size());
            data.resize(data.size() + 4096);

            uint32_t destsize = static_cast<uint32_t>(data.size());
            REQUIRE(SCompDecompress(data.data(), &destsize, data.data(), srcsize) == 1);
            data.resize(destsize);

            datastream expected = ReadBinary("all_ascii.txt");
            REQUIRE(destsize == expected.size());
            CHECK(data == expected);
        }

        SECTION("succeeds when dest > source") {
            uint32_t offset = GENERATE(1, 2, 10);
            datastream data = ReadBinary("compressed/PKWare_Text_all_ascii.txt.bin");
            uint32_t srcsize = static_cast<uint32_t>(data.size());
            data.resize(data.size() + 4096);

            uint32_t destsize = static_cast<uint32_t>(data.size());
            REQUIRE(SCompDecompress(data.data() + offset, &destsize, data.data(), srcsize) == 1);
            datastream data_copy(data.data() + offset, data.data() + offset + destsize);

            datastream expected = ReadBinary("all_ascii.txt");
            REQUIRE(destsize == expected.size());
            CHECK(data_copy == expected);
        }

        SECTION("succeeds when dest < source") {
            uint32_t offset = GENERATE(1, 2, 10);
            datastream data = ReadBinary("compressed/PKWare_Text_all_ascii.txt.bin");
            uint32_t srcsize = static_cast<uint32_t>(data.size());
            data.insert(data.begin(), offset, 0);
            data.resize(data.size() + 4096);

            uint32_t destsize = static_cast<uint32_t>(data.size());
            REQUIRE(SCompDecompress(data.data(), &destsize, data.data() + offset, srcsize) == 1);
            data.resize(destsize);

            datastream expected = ReadBinary("all_ascii.txt");
            REQUIRE(destsize == expected.size());
            CHECK(data == expected);
        }
    }

    SECTION("fails when source size is 0") {
        datastream data(1);
        uint32_t destsize = 1;
        CHECK_FALSE(SCompDecompress(data.data(), &destsize, data.data(), 0));
    }

    SECTION("performs only partial decompression when there are not enough bytes") {
        uint32_t extrabytes = GENERATE(1, 2, 10);
        datastream data = ReadBinary("compressed/PKWare_Text_all_ascii.txt.bin");

        datastream dest(data.size() + extrabytes);
        uint32_t destsize = static_cast<uint32_t>(dest.size());
        CHECK(SCompDecompress(dest.data(), &destsize, data.data(), static_cast<uint32_t>(data.size())) == 1);

        datastream expected = ReadBinary("all_ascii.txt");
        CHECK(destsize == dest.size());
        CHECK(destsize != expected.size());

        CHECK(dest != expected);
        expected.resize(dest.size());
        CHECK(dest == expected);
    }

    SECTION("succeeds when dest has exactly enough bytes") {
        datastream data = ReadBinary("compressed/PKWare_Text_all_ascii.txt.bin");
        datastream expected = ReadBinary("all_ascii.txt");

        datastream dest(expected.size());
        uint32_t destsize = static_cast<uint32_t>(dest.size());
        REQUIRE(SCompDecompress(dest.data(), &destsize, data.data(), static_cast<uint32_t>(data.size())) == 1);
        CHECK(dest == expected);
    }

    SECTION("normal operation") {
#if defined(WHOA_SCOMP_OLD)
        auto testcase = GENERATE(
            CompTestCase{"all_ascii.txt", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, ALL_ZLIB}},
            CompTestCase{"all_null.bin", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, ALL_HUFFMAN_BINARY, ALL_ZLIB}},
            CompTestCase{"all_same_ascii.txt", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE, HUFFMAN_BINARY, HUFFMAN_TEXT, ALL_ZLIB}},
            CompTestCase{"newlines.txt", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_TEXT, HUFFMAN_WAV8MONO, HUFFMAN_WAV8STEREO, HUFFMAN_WAV16STEREO, HUFFMAN_WAV16STEREO2, HUFFMAN_BINARY, HUFFMAN_NONE, ALL_ZLIB}},
            CompTestCase{"scenario.chk", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, ALL_HUFFMAN_BINARY, ALL_ZLIB}},
            CompTestCase{"test_11025_8_mono.wav", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE, ALL_ZLIB}},
            CompTestCase{"test_11025_8_stereo.wav", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE, ALL_ZLIB}},
            CompTestCase{"test_11025_16_mono.wav", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE, HUFFMAN_BINARY, HUFFMAN_EXE, ALL_ZLIB}},
            CompTestCase{"test_11025_16_stereo.wav", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE, HUFFMAN_BINARY, HUFFMAN_EXE, ALL_ZLIB}},
            CompTestCase{"test_44100_8_mono.wav", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE, ALL_ZLIB}},
            CompTestCase{"test_44100_8_stereo.wav", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE, ALL_ZLIB}},
            CompTestCase{"test_44100_16_mono.wav", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE, HUFFMAN_BINARY, HUFFMAN_EXE, ALL_ZLIB}},
            CompTestCase{"test_44100_16_stereo.wav", {ALL_ADPCM, ALL_PKWARE, ALL_ADPCM_COMBOS, HUFFMAN_NONE, HUFFMAN_BINARY, HUFFMAN_EXE, ALL_ZLIB}},
            CompTestCase{"smallest_null.bin", {HUFFMAN_NONE}},
            CompTestCase{"smallest_ascii.txt", {HUFFMAN_NONE}}
        );
#else // HUFFMAN and ADPCM on their own are not supported in new versions
        // TODO BZIP2, ZLIB, LZMA, SPARSE
        // Combos: BZIP2+SPARSE, ZLIB+SPARSE
        auto testcase = GENERATE(
            // TODO ALL_ADPCM_COMBOS on real WoW.exe for these 3 needs verification since they fail the test
            CompTestCase{"all_ascii.txt", {ALL_PKWARE, ALL_ZLIB}},
            CompTestCase{"all_null.bin", {ALL_PKWARE, ALL_ZLIB}},
            CompTestCase{"all_same_ascii.txt", {ALL_PKWARE, ALL_ZLIB}},
            CompTestCase{"newlines.txt", {ALL_PKWARE, ALL_ADPCM_COMBOS, ALL_ZLIB}},
            CompTestCase{"scenario.chk", {ALL_PKWARE, ALL_ADPCM_COMBOS, ALL_ZLIB}},
            CompTestCase{"test_11025_8_mono.wav", {ALL_PKWARE, ALL_ADPCM_COMBOS, ALL_ZLIB}},
            CompTestCase{"test_11025_8_stereo.wav", {ALL_PKWARE, ALL_ADPCM_COMBOS, ALL_ZLIB}},
            CompTestCase{"test_11025_16_mono.wav", {ALL_PKWARE, ALL_ADPCM_COMBOS, ALL_ZLIB}},
            CompTestCase{"test_11025_16_stereo.wav", {ALL_PKWARE, ALL_ADPCM_COMBOS, ALL_ZLIB}},
            CompTestCase{"test_44100_8_mono.wav", {ALL_PKWARE, ALL_ADPCM_COMBOS, ALL_ZLIB}},
            CompTestCase{"test_44100_8_stereo.wav", {ALL_PKWARE, ALL_ADPCM_COMBOS, ALL_ZLIB}},
            CompTestCase{"test_44100_16_mono.wav", {ALL_PKWARE, ALL_ADPCM_COMBOS, ALL_ZLIB}},
            CompTestCase{"test_44100_16_stereo.wav", {ALL_PKWARE, ALL_ADPCM_COMBOS, ALL_ZLIB}}
        );
#endif
        DYNAMIC_SECTION(testcase.filename) {
            SECTION("decompression") {
                for (auto& setting : testcase.settings) {
                    DYNAMIC_SECTION(setting.prefix) {
                        datastream data = ReadBinary("compressed/" + setting.prefix + testcase.filename + ".bin");
                        datastream dest(1024 * 1024);
                        uint32_t destsize = static_cast<uint32_t>(dest.size());
                        REQUIRE(SCompDecompress(dest.data(), &destsize, data.data(), static_cast<uint32_t>(data.size())) == 1);
                        dest.resize(destsize);

                        datastream expected = ReadBinary("decompressed/" + setting.prefix + testcase.filename);
                        REQUIRE(destsize == expected.size());
                        CHECK(dest == expected);
                    }
                }
            }
        }
    }

    // TODO BZIP2, RLE, ZLIB+RLE, BZIP2+RLE, non-WOW combos
}
#if !defined(WHOA_TEST_STORMDLL)
TEST_CASE("SCompDestroy", "[comp]") {
    SECTION("always returns 1") {
        CHECK(SCompDestroy() == 1);
    }
}
#endif
