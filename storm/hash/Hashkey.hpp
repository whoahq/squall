#ifndef STORM_HASH_HASHKEY_HPP
#define STORM_HASH_HASHKEY_HPP

class HASHKEY_PTR {
    public:
    // Member variables
    void* m_key;

    // Member functions
    HASHKEY_PTR();
    HASHKEY_PTR(void* key);
    bool operator==(const HASHKEY_PTR& key);
};

class HASHKEY_STR {
    public:
    // Member variables
    char* m_str;

    // Member functions
    HASHKEY_STR();
    HASHKEY_STR(const char* str);
    ~HASHKEY_STR();
    HASHKEY_STR& operator=(const char* str);
    bool operator==(const char* str);
};

class HASHKEY_STRI : public HASHKEY_STR {
    public:
    // Member functions
    HASHKEY_STRI& operator=(const char* str);
    bool operator==(const char* str);
};

class HASHKEY_NONE {
    public:
    // Member functions
    bool operator==(const HASHKEY_NONE& key);
};

#endif
