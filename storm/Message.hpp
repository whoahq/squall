#ifndef STORM_MESSAGE_HPP
#define STORM_MESSAGE_HPP
#include "Core.hpp"
#include "region/Types.hpp"

#if defined(WHOA_SYSTEM_WIN)
#include <Windows.h>
#endif

typedef struct _PARAMS {
    void* window;
    uint32_t message;
    uint32_t wparam;
    int32_t lparam;
    uint32_t notifycode;
    void* extra;
    int32_t useresult;
    int32_t result;
} PARAMS;

typedef void (STORMAPI* SMSGHANDLER)(PARAMS*);

int32_t STORMAPI SMsgBreakHandlerChain(PARAMS* params);

int32_t STORMAPI SMsgDestroy();

int32_t STORMAPI SMsgDispatchMessage(HWND window, uint32_t message, uint32_t wparam, int32_t lparam, int32_t* useresult, int32_t* result);

HWND STORMAPI SMsgGetDefaultWindow();

int32_t STORMAPI SMsgGetDefaultWindowRect(RECT* rect);

int32_t STORMAPI SMsgPopRegisterState(HWND window);

int32_t STORMAPI SMsgPushRegisterState(HWND window);

int32_t STORMAPI SMsgRegisterCommand(HWND window, uint32_t id, SMSGHANDLER handler);

int32_t STORMAPI SMsgRegisterKeyDown(HWND window, uint32_t id, SMSGHANDLER handler);

int32_t STORMAPI SMsgRegisterKeyUp(HWND window, uint32_t id, SMSGHANDLER handler);

int32_t STORMAPI SMsgRegisterMessage(HWND window, uint32_t id, SMSGHANDLER handler);

int32_t STORMAPI SMsgRegisterSysCommand(HWND window, uint32_t id, SMSGHANDLER handler);

int32_t STORMAPI SMsgSetDefaultWindow(HWND window);

void STORMAPI SMsgSetDefaultWindowRect(RECT* rect);

int32_t STORMAPI SMsgUnregisterCommand(HWND window, uint32_t id, SMSGHANDLER handler);

int32_t STORMAPI SMsgUnregisterKeyDown(HWND window, uint32_t id, SMSGHANDLER handler);

int32_t STORMAPI SMsgUnregisterKeyUp(HWND window, uint32_t id, SMSGHANDLER handler);

int32_t STORMAPI SMsgUnregisterMessage(HWND window, uint32_t id, SMSGHANDLER handler);

int32_t STORMAPI SMsgUnregisterSysCommand(HWND window, uint32_t id, SMSGHANDLER handler);

#endif
