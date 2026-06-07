#ifndef STORM_COMP_HPP
#define STORM_COMP_HPP

#include "storm/Core.hpp"
#include <cstdint>

// Support:
//   SC1, WC2, D1, D2: Huffman, PKWare, and ADPCM mono/stereo
//   Early WoW and WC3: SC1 + ZLIB
//   Later WoW and WC3: All but LZMA
//   SC2: All
//
// ADPCM:
//      - Lossy
//      - 16-bit PCM audio
//      - Only operates on raw PCM data (i.e. NOT wav file headers, only the data)
//      - `optimization` can be from 0 to 3 (1 and 2 are the same)
//
// PKWare:
//      - If `hint` is SCOMP_HINT_TEXT, a special mode is used. All other values are the same as SCOMP_HINT_NONE.
//
// Huffman:
//      - Uses a different distribution table based on the `hint`.
//
// If *destsize == sourcesize then compression would have created a larger file and it is left uncompressed.

#define SCOMP_HINT_NONE 0
#define SCOMP_HINT_BINARY 1
#define SCOMP_HINT_TEXT 2
#define SCOMP_HINT_EXECUTABLE 3
// These are all just guesses
#define SCOMP_HINT_ADPCM_8_MONO 4
#define SCOMP_HINT_ADPCM_8_STEREO 5
#define SCOMP_HINT_ADPCM_16_MONO 6
#define SCOMP_HINT_ADPCM_16_STEREO 7
#define SCOMP_HINT_ADPCM_16_STEREO2 8
#define SCOMP_HINTS 9

#define SCOMP_TYPE_HUFFMAN 0x01
#define SCOMP_TYPE_ZLIB 0x02
#define SCOMP_TYPE_PKWARE 0x08
#define SCOMP_TYPE_BZIP2 0x10
#define SCOMP_TYPE_RLE_ZERO 0x20
#define SCOMP_TYPE_LOSSY_ADPCM_MONO 0x40
#define SCOMP_TYPE_LOSSY_ADPCM_STEREO 0x80
#define SCOMP_TYPE_LZMA 0x12

#define SCOMP_TYPE_DIAB_LOSSY_ADPCM_MONO 0x10
#define SCOMP_TYPE_DIAB_LOSSY_ADPCM_STEREO 0x20

int32_t STORMAPI SCompCompress(void* dest, uint32_t* destsize, const void* source, uint32_t sourcesize, uint32_t compressiontypes, uint32_t hint, uint32_t optimization);

int32_t STORMAPI SCompDecompress(void* dest, uint32_t* destsize, const void* source, uint32_t sourcesize);

int32_t STORMAPI SCompDestroy();

#endif
