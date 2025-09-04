#include "storm/hash/Hashkey.hpp"
#include "storm/Memory.hpp"
#include "storm/String.hpp"

bool HASHKEY_NONE::operator==(const HASHKEY_NONE& key) {
    return true;
}

HASHKEY_PTR::HASHKEY_PTR() {
    this->m_key = nullptr;
}

HASHKEY_PTR::HASHKEY_PTR(void* key) {
    this->m_key = key;
}

HASHKEY_PTR& HASHKEY_PTR::operator=(const HASHKEY_PTR& key) {
    this->m_key = key.m_key;
    return *this;
}

bool HASHKEY_PTR::operator==(const HASHKEY_PTR& key) const {
    return this->m_key == key.m_key;
}

void* HASHKEY_PTR::GetPtr() const {
    return this->m_key;
}

HASHKEY_STR::HASHKEY_STR() {
    this->m_str = nullptr;
}

HASHKEY_STR::HASHKEY_STR(const char* str) {
    this->m_str = SStrDupA(str, __FILE__, __LINE__);
}

HASHKEY_STR::~HASHKEY_STR() {
    if (this->m_str) {
        SMemFree(this->m_str, __FILE__, __LINE__);
    }
}

HASHKEY_STR& HASHKEY_STR::operator=(const char* str) {
    if (this->m_str != str) {
        if (this->m_str) {
            SMemFree(this->m_str, __FILE__, __LINE__);
        }

        this->m_str = SStrDupA(str, __FILE__, __LINE__);
    }

    return *this;
}

HASHKEY_STR& HASHKEY_STR::operator=(const HASHKEY_STR& key) {
    this->operator=(key.m_str);

    return *this;
}

bool HASHKEY_STR::operator==(const char* str) const {
    return SStrCmp(this->m_str, str) == 0;
}

bool HASHKEY_STR::operator==(const HASHKEY_STR& key) const {
    return this->operator==(key.m_str);
}

const char* HASHKEY_STR::GetString() const {
    return this->m_str;
}

HASHKEY_STRI& HASHKEY_STRI::operator=(const char* str) {
    static_cast<HASHKEY_STR&>(*this) = str;

    return *this;
}

HASHKEY_STRI& HASHKEY_STRI::operator=(const HASHKEY_STRI& key) {
    static_cast<HASHKEY_STR&>(*this) = key.m_str;

    return *this;
}

bool HASHKEY_STRI::operator==(const char* str) const {
    return SStrCmpI(this->m_str, str) == 0;
}

bool HASHKEY_STRI::operator==(const HASHKEY_STRI& key) const {
    return this->operator==(key.m_str);
}
