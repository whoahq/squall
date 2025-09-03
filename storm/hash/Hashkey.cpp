#include "storm/hash/Hashkey.hpp"
#include "storm/Memory.hpp"
#include "storm/String.hpp"

bool HASHKEY_NONE::operator==(const HASHKEY_NONE& key) {
    return true;
}

HASHKEY_PTR::HASHKEY_PTR(void* key) {
    this->m_key = key;
}

bool HASHKEY_PTR::operator==(const HASHKEY_PTR& key) {
    return this->m_key == key.m_key;
}

HASHKEY_STR::~HASHKEY_STR() {
    if (this->m_str) {
        SMemFree(this->m_str, __FILE__, __LINE__, 0x0);
    }
}

HASHKEY_STR& HASHKEY_STR::operator=(const char* str) {
    if (this->m_str != str) {
        if (this->m_str) {
            SMemFree(this->m_str, __FILE__, __LINE__, 0x0);
        }

        this->m_str = SStrDupA(str, __FILE__, __LINE__);
    }

    return *this;
}

bool HASHKEY_STR::operator==(const char* str) {
    return SStrCmp(this->m_str, str, STORM_MAX_STR) == 0;
}

HASHKEY_STRI& HASHKEY_STRI::operator=(const char* str) {
    static_cast<HASHKEY_STR&>(*this) = str;
    return *this;
}

bool HASHKEY_STRI::operator==(const char* str) {
    return SStrCmpI(this->m_str, str, STORM_MAX_STR) == 0;
}
