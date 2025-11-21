#ifndef STORM_EVENT_HPP
#define STORM_EVENT_HPP

#include <cstdint>
#include "Core.hpp"

typedef void (STORMAPI* SEVTHANDLER)(void*);

int32_t STORMAPI SEvtBreakHandlerChain(void* data);

int32_t STORMAPI SEvtDestroy();

int32_t STORMAPI SEvtDispatch(uint32_t type, uint32_t subtype, uint32_t id, void* data);

int32_t STORMAPI SEvtPopState(uint32_t type, uint32_t subtype);

int32_t STORMAPI SEvtPushState(uint32_t type, uint32_t subtype);

int32_t STORMAPI SEvtRegisterHandler(uint32_t type, uint32_t subtype, uint32_t id, uint32_t flags, SEVTHANDLER handler);

int32_t STORMAPI SEvtUnregisterHandler(uint32_t type, uint32_t subtype, uint32_t id, SEVTHANDLER handler);

int32_t STORMAPI SEvtUnregisterType(uint32_t type, uint32_t subtype);

#endif
