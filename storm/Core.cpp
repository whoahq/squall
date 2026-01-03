#include "Core.hpp"
#include "Event.hpp"
#include "Transparency.hpp"

int32_t STORMAPI StormDestroy() {
    // Combined list of all destroy calls found in every game (as documentation)

    // SErrSetBlizzardErrorFunction(nullptr);
    // SDlgDestroy();
    // SGdiDestroy();
    // SVidDestroy();
    // SDrawDestroy();
    // SRgnDestroy();
    // SMsgDestroy();
    // SNetDestroy();
    SEvtDestroy();
    // SBltDestroy();
    // SCodeDestroy();
    // SCmdDestroy();
    // SFileDestroy();
    // SFile::Destroy();
    // SCompDestroy();
    // SStrDestroy();
    // SRegDestroy();
    // SErrDestroy();
    // SLogDestroy();
    STransDestroy();
    return 1;
}
