// Dummy DLL to prevent having to copy it from WoW directory
// We are not utilizing anything related to it, so just stub it

#include <cstdint>

extern "C" {
    __declspec(dllexport)
    int32_t DivxDecode(uint32_t, void*, uint32_t) { return 0; }

    __declspec(dllexport)
    int32_t InitializeDivxDecoder(uint32_t, uint32_t, uint32_t) { return 0; }

    __declspec(dllexport)
    int32_t SetOutputFormat(uint32_t, uint32_t, uint32_t, uint32_t) { return 0; }

    __declspec(dllexport)
    int32_t UnInitializeDivxDecoder(uint32_t) { return 0; }
}