#ifndef STORM_HASH_HASHKEY_HPP
#define STORM_HASH_HASHKEY_HPP

class HASHKEY_PTR {
    public:
    // Member variables
    void* m_key;

    // Member functions
    bool operator==(const HASHKEY_PTR&);
};

class HASHKEY_STR {
    public:
    // Member variables
    char* m_str;

    // Member functions
    ~HASHKEY_STR();
    HASHKEY_STR& operator=(const char*);
    bool operator==(const char*);
};

class HASHKEY_STRI : public HASHKEY_STR {
    public:
    // Member functions
    HASHKEY_STRI& operator=(const char*);
    bool operator==(const char*);
};

class HASHKEY_NONE {
    public:
    // Member functions
    bool operator==(const HASHKEY_NONE&);
};

#endif
