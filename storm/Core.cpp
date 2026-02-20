#include "Core.hpp"
#include "Event.hpp"
#include "Region.hpp"
#include "String.hpp"
#include "Transparency.hpp"

#if defined(WHOA_SYSTEM_WIN)
#include <Windows.h>
#endif

int32_t STORMAPI StormDestroy() {
    // Combined list of all destroy calls found in every game (as documentation)

    // SErrSetBlizzardErrorFunction(nullptr);   // SC 1.17
    // SDlgDestroy();       // SC 1.17
    // SGdiDestroy();       // SC 1.17
    // SVidDestroy();       // SC 1.17
    // SDrawDestroy();      // SC 1.17
    SRgnDestroy();          // WoW 3.3.5 (win), SC 1.17
    // SMsgDestroy();       // WoW 3.3.5 (win), SC 1.17
    // SNetDestroy();       // SC 1.17
    SEvtDestroy();          // WoW 3.3.5 (win)
    // SBltDestroy();       // SC 1.17
    // SCodeDestroy();      // SC 1.17
    // SCmdDestroy();       // WoW 3.3.5 (win+mac), SC 1.17
    // SFileDestroy();      // WoW 3.3.5 (mac)?, SC 1.17
    // SFile::Destroy();    // WoW 0.5.3 (win)?
    // SCompDestroy();      // WoW 3.3.5 (win)?
    SStrDestroy();          // WoW 3.3.5 (mac), SC 1.17
    // SRegDestroy();       // WoW 3.3.5 (mac)
    // SErrDestroy();       // WoW 3.3.5 (mac)
    // SLogDestroy();       // WoW 3.3.5 (mac)
    STransDestroy();        // SC 1.17
    return 1;
}

// NOTE: If squall is built as a DLL in the future, this should be updated to return hinstDLL from DllMain instead.
// No idea what the mac/linux equivalents would be, since the old Mac OS versions have an entire WinAPI emulator.
HINSTANCE STORMAPI StormGetInstance() {
#if defined(WHOA_SYSTEM_WIN)
    return GetModuleHandle(nullptr);
#else
    return nullptr;
#endif
}
