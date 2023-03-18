#ifndef STORM_MD5_HPP
#define STORM_MD5_HPP

#include <cstdint>
#include <cstring>

class MD5 {
    public:
    MD5(const MD5& copy);
    MD5(uint32_t a, uint32_t b, uint32_t c, uint32_t d);
    MD5();
    const MD5& operator=(const MD5& copy);
    bool operator==(const MD5& cmp);
    bool operator!=(const MD5& cmp);

    private:
    uint32_t val[4];
};

inline MD5::MD5(const MD5& copy) {
    std::memcpy(this->val, copy.val, sizeof(this->val));
}

inline MD5::MD5(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    this->val[0] = a;
    this->val[1] = b;
    this->val[2] = c;
    this->val[3] = d;
}

inline MD5::MD5() {
    // COMMENT: Original code does nothing
    // But it's good practice to set initial values
    std::memset(this->val, 0, sizeof(this->val));
}

inline const MD5& MD5::operator=(const MD5& copy) {
    std::memcpy(this->val, copy.val, sizeof(this->val));
}

inline bool MD5::operator==(const MD5& cmp) {
    return std::memcmp(this->val, cmp.val, sizeof(this->val)) == 0;
}

inline bool MD5::operator!=(const MD5& cmp) {
    return std::memcmp(this->val, cmp.val, sizeof(this->val)) != 0;
}

#endif
