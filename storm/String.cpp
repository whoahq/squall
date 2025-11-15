#include "storm/String.hpp"
#include "storm/Error.hpp"
#include "storm/Memory.hpp"
#include "storm/string/bjhash.hpp"
#include <cctype>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstring>

#if defined(WHOA_SYSTEM_WIN)
#include <string.h>
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
#include <strings.h>
#endif

uint8_t bytesFromUTF8[256] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6
};

uint32_t offsetsFromUTF8[8] = {
    0,
    0,
    0x3080,
    0x0E2080,
    0x3C82080,
    0x0FA082080,
    0x82082080,
    0
};

int32_t s_initialized;

double s_realDigit[20][10];

const uint32_t s_hashtable[16] = {
    0x486E26EE, 0xDCAA16B3, 0xE1918EEF, 0x202DAFDB,
    0x341C7DC7, 0x1C365303, 0x40EF2D37, 0x65FD5E49,
    0xD6057177, 0x904ECE93, 0x1C38024F, 0x98FD323B,
    0xE3061AE7, 0xA39B0FA1, 0x9797F25F, 0xE4444563,
};

const uint64_t s_hashtable64[16] = {
    0x486E26EEDCAA16B3ULL, 0xE1918EEF202DAFDBULL, 0x341C7DC71C365303ULL, 0x40EF2D3765FD5E49ULL,
    0xD6057177904ECE93ULL, 0x1C38024F98FD323BULL, 0xE3061AE7A39B0FA1ULL, 0x9797F25FE4444563ULL,
    0xCD2EC20C8DC1B898ULL, 0x31759633799A306DULL, 0x8C2063852E6E9627ULL, 0x79237D9973922C66ULL,
    0x8728628D28628824ULL, 0x8F1F7E9625887795ULL, 0x296E3281389C0D60ULL, 0x6F4893CA61636542ULL,
};

void GetNextTextUpper(uint32_t* orig, const char** string, uint32_t* upper) {
    uint8_t byte = **string;
    int32_t v3 = bytesFromUTF8[byte];

    *orig = 0;
    *upper = 0;

    switch (v3) {
    case 6:
    case 5:
    case 4:
    case 3:
    case 2:
        *orig += **string;
        (*string)++;

        if (!**string) {
            return;
        }

        *orig <<= 6;

    case 1:
        *orig += **string;
        (*string)++;

    default:
        *orig -= offsetsFromUTF8[v3];

        if (*orig > 0xFFFF) {
            *orig = 0xFFFF;
        }

        uint32_t v4 = *orig;

        bool v5, v6;

        if (v3 == 1) {
            if (v4 < 0x61) {
                *upper = v4;
                return;
            }

            v5 = v4 < 0x7A;
            v6 = v4 == 122;

            if (!v5 && !v6) {
                *upper = v4;
                return;
            }

            v4 -= 32;
            *upper = v4;
            return;
        }

        if (v3 != 2) {
            *upper = v4;
            return;
        }

        if (v4 >= 0xE0 && v4 <= 0xFE) {
            v4 -= 32;
            *upper = v4;
            return;
        }

        if (v4 == 339) {
            *upper = 338;
            return;
        }

        if (v4 == 1105) {
            *upper = 1025;
            return;
        }

        if (v4 >= 0x430) {
            v5 = v4 < 0x44F;
            v6 = v4 == 1103;

            if (!v5 && !v6) {
                *upper = v4;
                return;
            }

            v4 -= 32;
            *upper = v4;
            return;
        }

        *upper = v4;
        return;
    }
}

void InitializeFloatDigits() {
    for (int32_t i = -1; i > -21; i--) {
        for (int32_t j = 0; j < 10; j++) {
            double v3 = 10.0;
            int32_t v4 = i < 0 ? -i : i;
            double v5 = 1.0;

            while (true) {
                if (v4 & 1) {
                    v5 *= v3;
                }

                v4 >>= 1;

                if (!v4) {
                    break;
                }

                v3 *= v3;
            }

            double v6 = i < 0 ? 1.0 / v5 : v5;

            double v7 = v6 * static_cast<double>(j);

            s_realDigit[-i - 1][j] = v7;
        }
    }
}

size_t ISStrVPrintf(char* dest, size_t maxchars, const char* format, va_list va) {
    if (!maxchars) {
        return 0;
    }

    size_t result;

    if (maxchars == STORM_MAX_STR) {
        // TODO conditional vsoprintf;
        result = vsprintf(dest, format, va);
    } else {
        // TODO conditional vsnoprintf;
        result = vsnprintf(dest, maxchars, format, va);

        if (result >= maxchars) {
            result = maxchars - 1;
            dest[result] = '\0';
        }
    }

    return result;
}

void STORMAPI SStrInitialize() {
    if (!s_initialized) {
        InitializeFloatDigits();
        s_initialized = 1;
    }
}

char* STORMAPI SStrChr(char* string, char search) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE_END;

    if (!*string) {
        return nullptr;
    }

    while (*string != search) {
        string++;

        if (!*string) {
            return nullptr;
        }
    }

    return string;
}

const char* STORMAPI SStrChr(const char* string, char search) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE_END;

    if (!*string) {
        return nullptr;
    }

    while (*string != search) {
        string++;

        if (!*string) {
            return nullptr;
        }
    }

    return string;
}

char* STORMAPI SStrChrR(char* string, char search) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE_END;

    char* result;

    for (result = nullptr; *string; string++) {
        if (*string == search) {
            result = string;
        }
    }

    return result;
}

const char* STORMAPI SStrChrR(const char* string, char search) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE_END;

    const char* result;

    for (result = nullptr; *string; string++) {
        if (*string == search) {
            result = string;
        }
    }

    return result;
}

int32_t STORMAPI SStrCmp(const char* string1, const char* string2, size_t maxchars) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string1);
    STORM_VALIDATE(string2);
    STORM_VALIDATE_END;

    return strncmp(string1, string2, maxchars);
}

int32_t STORMAPI SStrCmpI(const char* string1, const char* string2, size_t maxchars) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string1);
    STORM_VALIDATE(string2);
    STORM_VALIDATE_END;

#if defined(WHOA_SYSTEM_WIN)
    return _strnicmp(string1, string2, maxchars);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    return strncasecmp(string1, string2, maxchars);
#endif
}

size_t STORMAPI SStrCopy(char* dest, const char* source, size_t destsize) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(dest);
    STORM_VALIDATE(source);
    STORM_VALIDATE_END;

    char* destbuf = dest;

    if (destsize == STORM_MAX_STR) {
        while (*source) {
            *destbuf = *source;

            ++destbuf;
            ++source;
        }
    } else {
        if (*source) {
            while (destbuf < &dest[destsize - 1]) {
                *destbuf = *source;

                ++destbuf;
                ++source;

                if (!*source) {
                    *destbuf = '\0';
                    return static_cast<size_t>(destbuf - dest);
                }
            }
        }
    }

    *destbuf = '\0';
    return static_cast<size_t>(destbuf - dest);
}

char* STORMAPI SStrDupA(const char* string, const char* filename, uint32_t linenumber) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE_END;

    size_t len = SStrLen(string) + 1;
    char* dup = static_cast<char*>(SMemAlloc(len, filename, linenumber, 0x0));
    memcpy(dup, string, len);

    return dup;
}

uint32_t STORMAPI SStrHash(const char* string, uint32_t flags, uint32_t seed) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE_END;

    uint32_t result = seed ? seed : 0x7FED7FED;
    uint32_t adjust = 0xEEEEEEEE;
    uint32_t ch;

    if (flags & SSTR_HASH_CASESENSITIVE) {
        for (; *string; string++) {
            ch = *string;

            result = (s_hashtable[ch / 16] - s_hashtable[ch % 16]) ^ (adjust + result);
            adjust = 33 * adjust + result + ch + 3;
        }
    }
    else {
        for (; *string; string++) {
            ch = *string;

            if (ch >= 'a' && ch <= 'z') {
                ch -= 32;
            }

            if (ch == '/') {
                ch = '\\';
            }

            result = (s_hashtable[ch / 16] - s_hashtable[ch % 16]) ^ (adjust + result);
            adjust = 33 * adjust + result + ch + 3;
        }
    }
    return result ? result : 1;
}

uint32_t STORMAPI SStrHashHT(const char* string) {
    char normalized[0x400];
    char* buf = normalized;

    uint32_t length = 0;

    if (*string) {
        uint32_t value, orig, upper;

        while (*string && length <= 0x3FB) {
            // Convert each character to uppercase
            GetNextTextUpper(&orig, &string, &upper);

            // Replace forward slash with back slash
            value = upper == '/' ? '\\' : upper;

            while (value) {
                *buf = value;
                value >>= 8;
                length++;
                buf++;
            }
        }
    }

    // Ensure string is terminated
    *buf = 0;

    return bjhash((uint8_t*)&normalized, length, 0);
}

int64_t STORMAPI SStrHash64(const char* string, uint32_t flags, int64_t seed) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE_END;

    int64_t result = seed ? seed : 0x7FED7FED7FED7FEDLL;
    int64_t adjust = 0xEEEEEEEEEEEEEEEELL;
    uint32_t ch;

    if (flags & SSTR_HASH_CASESENSITIVE) {
        for (; *string; string++) {
            ch = static_cast<uint8_t>(*string);

#if defined(WHOA_SSTRHASH64_SUBTRACTS)
            result = (s_hashtable64[ch / 16] - s_hashtable64[ch % 16]) ^ (adjust + result);
#else
            result = (s_hashtable64[ch / 16] + s_hashtable64[ch % 16]) ^ (adjust + result);
#endif
            adjust = 33 * adjust + result + ch + 3;
        }
    }
    else {
        for (; *string; string++) {
            ch = static_cast<uint8_t>(*string);

            if (ch >= 'a' && ch <= 'z') {
                ch -= 32;
            }

            if (ch == '/') {
                ch = '\\';
            }

#if defined(WHOA_SSTRHASH64_SUBTRACTS)
            result = (s_hashtable64[ch / 16] - s_hashtable64[ch % 16]) ^ (adjust + result);
#else
            result = (s_hashtable64[ch / 16] + s_hashtable64[ch % 16]) ^ (adjust + result);
#endif
            adjust = 33 * adjust + result + ch + 3;
        }
    }
    return result ? result : 1LL;
}

size_t STORMAPI SStrLen(const char* string) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE_END;

    auto stringEnd = string;
    while (*stringEnd) {
        stringEnd++;
    }

    return stringEnd - string;
}

void STORMAPI SStrLower(char* string) {
    while (*string) {
        *string = static_cast<char>(tolower(*string));
        string++;
    }
}

uint32_t STORMAPI SStrPack(char* dest, const char* source, uint32_t destsize) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(dest);
    STORM_VALIDATE(source);
    STORM_VALIDATE_END;

    if (!destsize) {
        return 0;
    }

    char* i;
    const char* v5;
    char v6;
    const char* j;

    for (i = dest; *i; i++)
        ;

    if (destsize == STORM_MAX_STR) {
        v6 = *source;

        for (j = source; *j; i++) {
            j++;
            *i = v6;
            v6 = *j;
        }
    } else {
        v5 = source;

        if (*source) {
            while (i < &dest[destsize - 1]) {
                *i++ = *v5++;

                if (!*v5) {
                    *i = '\0';
                    return static_cast<uint32_t>(i - dest);
                }
            }
        }
    }

    *i = '\0';
    return static_cast<uint32_t>(i - dest);
}

size_t STORMCDECL SStrPrintf(char* dest, size_t maxchars, const char* format, ...) {
    va_list va;
    va_start(va, format);

    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(dest);
    STORM_VALIDATE(format);
    STORM_VALIDATE_END;

    return ISStrVPrintf(dest, maxchars, format, va);
}

size_t STORMCDECL SStrVPrintf(char* dest, size_t maxchars, const char* format, va_list arglist) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(dest);
    STORM_VALIDATE(format);
    STORM_VALIDATE_END;

    return ISStrVPrintf(dest, maxchars, format, arglist);
}

char* STORMAPI SStrStr(char* string, const char* search) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE(search);
    STORM_VALIDATE_END;

    size_t searchLength = SStrLen(search);

    for (; *string; string++) {
        if (!SStrCmp(string, search, searchLength)) {
            return string;
        }
    }
    return nullptr;
}

const char* STORMAPI SStrStr(const char* string, const char* search) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE(search);
    STORM_VALIDATE_END;

    size_t searchLength = SStrLen(search);

    for (; *string; string++) {
        if (!SStrCmp(string, search, searchLength)) {
            return string;
        }
    }
    return nullptr;
}

char* STORMAPI SStrStrI(char* string, const char* search) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE(search);
    STORM_VALIDATE_END;

    size_t searchLength = SStrLen(search);

    for (; *string; string++) {
        if (!SStrCmpI(string, search, searchLength)) {
            return string;
        }
    }
    return nullptr;
}

const char* STORMAPI SStrStrI(const char* string, const char* search) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE(search);
    STORM_VALIDATE_END;

    size_t searchLength = SStrLen(search);

    for (; *string; string++) {
        if (!SStrCmpI(string, search, searchLength)) {
            return string;
        }
    }
    return nullptr;
}

void STORMAPI SStrTokenize(const char** string, char* buffer, size_t bufferchars, const char* whitespace, int32_t* quoted) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE(*string);
    STORM_VALIDATE(buffer || bufferchars == 0);
    STORM_VALIDATE(whitespace);
    STORM_VALIDATE_END_VOID;

    int32_t checkquotes = SStrChr(whitespace, '"') != nullptr;

    int32_t inquotes = 0;
    int32_t usedquotes = 0;
    const char* currsource = *string;

    while (*currsource && SStrChr(whitespace, *currsource)) {
        if (checkquotes && *currsource == '"') {
            usedquotes = 1;
            inquotes = 1;
            currsource++;

            break;
        }

        currsource++;
    }

    uint32_t destchars = 0;

    while(*currsource) {
        if (checkquotes && *currsource == '"') {
            if (destchars && !inquotes) {
                break;
            }

            usedquotes = 1;
            inquotes = !inquotes;
            currsource++;

            if (!inquotes) {
                break;
            }
        }
        else {
            if (!inquotes && SStrChr(whitespace, *currsource)) {
                currsource++;
                break;
            }

            if (destchars + 1 < bufferchars) {
                buffer[destchars] = *currsource;
                destchars++;
            }
            currsource++;
        }
    }

    if (destchars < bufferchars) {
        buffer[destchars] = 0;
    }

    *string = currsource;

    if (quoted) {
        *quoted = usedquotes;
    }
}

static inline double ISStrToDouble(const char* string) {
    SStrInitialize();

    double result;
    bool negative = false;

    if (*string == '-') {
        negative = true;
        string++;
    }

    double v16 = 10.0;
    double v4 = 0.0;
    uint32_t v5 = *string - '0';
    const char* v6 = string;

    if (v5 >= 10) {
        v5 = 0;
        result = static_cast<double>(v5);
    } else {
        string++;

        uint32_t v8 = *string - '0';

        if (v8 >= 10) {
            result = static_cast<double>(v5);
        } else {
            do {
                v5 = v8 + 10 * v5;
                string++;

                if (v5 >= 0x19999999) {
                    v4 = v4 * pow(10.0, string - v6) + static_cast<double>(v5);
                    v5 = 0;
                    v6 = string;
                }

                v8 = *string - '0';
            } while (v8 < 10);

            if (v4 == 0.0) {
                result = static_cast<double>(v5);
            } else {
                result = pow(10.0, string - v6) * v4 + static_cast<double>(v5);
            }
        }
    }

    if (*string == '.') {
        string++;

        uint32_t v23 = *string - '0';
        int32_t v24 = 0;

        if (v23 < 10) {
            int32_t v25 = 0;
            int32_t v26 = -1;
            double v31;

            do {
                string++;

                if (v24 < 20) {
                    v31 = s_realDigit[0][v25 + v23];
                } else {
                    v31 = pow(v16, v26) * v23;
                }

                result += v31;

                v23 = *string - '0';
                v24++;
                v26--;
                v25 += 10;
            } while (v23 < 10);
        }
    }

    if (*string == 'e' || *string == 'E') {
        const char* v32 = string + 1;

        if (*v32 == '+') {
            v32++;
        }

        result *= pow(10.0, SStrToInt(v32));
    }

    if (negative) {
        result = -result;
    }

    return result;
}

double STORMAPI SStrToDouble(const char* string) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE_END;

    return ISStrToDouble(string);
}

float STORMAPI SStrToFloat(const char* string) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE_END;

    return static_cast<float>(ISStrToDouble(string));
}

int32_t STORMAPI SStrToInt(const char* string) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE_END;

    int32_t result = 0;
    bool negative = false;

    if (*string == '-') {
        negative = true;
        string++;
    }

    uint32_t digit;
    while ((digit = *string - '0') < 10) {
        result = digit + (10 * result);
        string++;
    }

    if (negative) {
        result = -result;
    }

    return result;
}

uint32_t STORMAPI SStrToUnsigned(const char* string) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(string);
    STORM_VALIDATE_END;

    uint32_t result = 0;

    uint32_t digit;
    while ((digit = *string - '0') < 10) {
        result = digit + (10 * result);
        string++;
    }

    return result;
}

void STORMAPI SStrUpper(char* string) {
    while (*string) {
        *string = static_cast<char>(toupper(*string));
        string++;
    }
}
