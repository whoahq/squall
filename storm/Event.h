#ifndef STORM_EVENT_HPP
#define STORM_EVENT_HPP

#include <cstdint>

#ifndef STORMAPI
#if defined(_MSC_VER)
    #define STORMAPI __stdcall
#else
    #define STORMAPI
#endif
#endif

typedef void (STORMAPI* SEVTHANDLER)(void*);


int32_t SEvtBreakHandlerChain(void* data);

int32_t SEvtDestroy();

int32_t SEvtDispatch(uint32_t type, uint32_t subtype, uint32_t id, void* data);

int32_t SEvtPopState(uint32_t type, uint32_t subtype);

int32_t SEvtPushState(uint32_t type, uint32_t subtype);

int32_t SEvtRegisterHandler(uint32_t type, uint32_t subtype, uint32_t id, uint32_t flags, SEVTHANDLER handler);

int32_t SEvtUnregisterHandler(uint32_t type, uint32_t subtype, uint32_t id, SEVTHANDLER handler);

int32_t SEvtUnregisterType(uint32_t type, uint32_t subtype);


#endif
