#include "loader.hpp"
#include "addressdef.hpp"

#include <cstdint>
#include <Windows.h>
#include <vector>

namespace WoWLoader {
    uintptr_t g_base_addr = 0;

    std::vector<WowAddrShared*>& RegisteredAddrs() {
        static std::vector<WowAddrShared*> g_registered_addrs;
        return g_registered_addrs;
    }

    bool Init() {
        g_base_addr = reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr));
        for (auto& addr : RegisteredAddrs()) {
            addr->SetBase(g_base_addr);
        }
        return g_base_addr != 0;
    }

    uintptr_t Base() {
        return g_base_addr;
    }

    void RegisterAddr(WowAddrShared* addr) {
        RegisteredAddrs().emplace_back(addr);
    }
}
