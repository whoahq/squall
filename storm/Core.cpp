#include "Core.hpp"
#include "Event.hpp"
#include "Transparency.hpp"

int32_t STORMAPI StormDestroy() {
    // Combined list of all calls from every game

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
    // SCompDestroy();
    // SStrDestroy();
    // SRegDestroy();
    // SErrDestroy();
    // SLogDestroy();
    STransDestroy();
    return 1;
}
