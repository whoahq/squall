#include "storm/Comp.hpp"
#include "storm/Error.hpp"

int32_t StormLib_SCompCompress(void* dest, uint32_t* destsize, const void* source, uint32_t sourcesize, uint32_t compressiontypes, uint32_t hint, uint32_t optimization);
int32_t StormLib_SCompDecompress(void* dest, uint32_t* destsize, const void* source, uint32_t sourcesize);

int32_t STORMAPI SCompCompress(void* dest, uint32_t* destsize, const void* source, uint32_t sourcesize, uint32_t compressiontypes, uint32_t hint, uint32_t optimization) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(dest);
    STORM_VALIDATE(*destsize >= sourcesize);
    STORM_VALIDATE(source);
    STORM_VALIDATE_END;

    return StormLib_SCompCompress(dest, destsize, source, sourcesize, compressiontypes, hint, optimization);
}

int32_t STORMAPI SCompDecompress(void* dest, uint32_t* destsize, const void* source, uint32_t sourcesize) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(dest);
    STORM_VALIDATE(*destsize >= sourcesize);
    STORM_VALIDATE(source);
    STORM_VALIDATE_END;

    return StormLib_SCompDecompress(dest, destsize, source, sourcesize);
}

int32_t STORMAPI SCompDestroy() {
  //if (s_DecompressBuffer) { ... } // handled by StormLib
  return 1;
}
