#ifndef WOWWRAPPER_LOADER_HPP
#define WOWWRAPPER_LOADER_HPP

#include <cstdint>
#include <string>

struct WowAddrShared;

namespace WoWLoader {
    bool Init();
    uintptr_t Base();

    void RegisterAddr(WowAddrShared* addr);
}

#endif
