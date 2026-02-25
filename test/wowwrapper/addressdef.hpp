#ifndef WOWWRAPPER_ADDRESSDEF
#define WOWWRAPPER_ADDRESSDEF

#include <cstdint>
#include <utility>

#include "loader.hpp"

struct WowAddrShared {
    uintptr_t real = 0; // must be first member for hacks
    const uintptr_t offset = 0;

    WowAddrShared(uintptr_t reladdress) : offset(reladdress) {
        WoWLoader::RegisterAddr(this);
    }

    void SetBase(uintptr_t baseaddr) {
        real = baseaddr + offset;
    }
};

template <class T>
struct WowFn : public WowAddrShared {
    WowFn(uintptr_t reladdress) : WowAddrShared(reladdress) {}

    T* get() const {
        return reinterpret_cast<T*>(this->real);
    }

    T& operator*() const {
        return *this->get();
    }

    T* operator->() const {
        return this->get();
    }

    template <class... Args>
    auto operator()(Args&&... args) const -> decltype(this->get()(std::forward<Args>(args)...)) const {
        return this->get()(std::forward<Args>(args)...);
    }
};

#endif
