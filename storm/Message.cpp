#include "Message.hpp"

#include "Error.hpp"
#include "Event.hpp"
#include "List.hpp"
#include "region/Types.hpp"

#if defined(WHOA_SYSTEM_WIN)
#include <Windows.h>
#else
#ifndef WM_KEYDOWN
#define WM_NCDESTROY 0x82
#define WM_KEYDOWN 0x100
#define WM_KEYUP 0x101
#define WM_COMMAND 0x111
#define WM_SYSCOMMAND 0x112
#endif
#endif

#ifndef MAKEFOURCC
#define MAKEFOURCC(a,b,c,d) ((a << 24) | (b << 16) | (c << 8) | (d))
#endif

#define REGISTERTYPE_BASE MAKEFOURCC('S', 'M', 'S', 'G')
#define REGISTERTYPE_MESSAGE    (REGISTERTYPE_BASE)
#define REGISTERTYPE_COMMAND    (REGISTERTYPE_BASE+1)
#define REGISTERTYPE_SYSCOMMAND (REGISTERTYPE_BASE+2)
#define REGISTERTYPE_KEYDOWN    (REGISTERTYPE_BASE+3)
#define REGISTERTYPE_KEYUP      (REGISTERTYPE_BASE+4)

struct WNDREC : public TSLinkedNode<WNDREC> {
    HWND window;
};

STORM_LIST(WNDREC) s_wndlist;
RECT s_defaultwindowrect;
HWND s_defaultwindow;

WNDREC* FindWindow(HWND window);

void AddWindow(HWND window) {
    s_wndlist.NewNode(STORM_LIST_TAIL, 0, 0)->window = window;
}

void DeleteWindow(HWND window) {
    if (window == s_defaultwindow) {
        s_defaultwindowrect = {};
        s_defaultwindow = nullptr;
    }
    WNDREC* ptr = FindWindow(window);
    if (ptr) {
        SEvtUnregisterType(REGISTERTYPE_MESSAGE, reinterpret_cast<uint32_t>(window));
        SEvtUnregisterType(REGISTERTYPE_COMMAND, reinterpret_cast<uint32_t>(window));
        SEvtUnregisterType(REGISTERTYPE_SYSCOMMAND, reinterpret_cast<uint32_t>(window));
        SEvtUnregisterType(REGISTERTYPE_KEYUP, reinterpret_cast<uint32_t>(window));
        SEvtUnregisterType(REGISTERTYPE_KEYDOWN, reinterpret_cast<uint32_t>(window));
        s_wndlist.DeleteNode(ptr);
    }
}

WNDREC* FindWindow(HWND window) {
    for (WNDREC* curr = s_wndlist.Head(); curr; curr = curr->Next()) {
        if (curr->window == window) return curr;
    }
    return nullptr;
}

int32_t InternalRegister(uint32_t type, HWND window, uint32_t id, SMSGHANDLER handler) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(handler);
    STORM_VALIDATE_END;

    if (!FindWindow(window)) {
        AddWindow(window);
    }
    return SEvtRegisterHandler(type, reinterpret_cast<uint32_t>(window), id, 0, reinterpret_cast<SEVTHANDLER>(handler));
}

int32_t InternalUnregister(uint32_t type, HWND window, uint32_t id, SMSGHANDLER handler) {
    if (!FindWindow(window)) {
        AddWindow(window);
    }
    return SEvtUnregisterHandler(type, reinterpret_cast<uint32_t>(window), id, reinterpret_cast<SEVTHANDLER>(handler));
}

int32_t STORMAPI SMsgBreakHandlerChain(PARAMS* params) {
    return SEvtBreakHandlerChain(params);
}

int32_t STORMAPI SMsgDestroy() {
    while (WNDREC* wnd = s_wndlist.Head()) {
        DeleteWindow(wnd->window);
    }
    return 1;
}

int32_t STORMAPI SMsgDispatchMessage(HWND window, uint32_t message, uint32_t wparam, int32_t lparam, int32_t* useresult, int32_t* result) {
    if (useresult) *useresult = 0;
    if (result) *result = 0;

    PARAMS params;
    params.window = window;
    params.message = message;
    params.wparam = wparam;
    params.lparam = lparam;
    params.notifycode = message == WM_COMMAND ? (wparam >> 16) : 0;
    params.useresult = 0;
    params.result = 0;

    HWND wnd = window;
    while(1) {
        SEvtDispatch(REGISTERTYPE_MESSAGE, reinterpret_cast<uint32_t>(wnd), message, &params);
        switch(message) {
        case WM_KEYDOWN:
            SEvtDispatch(REGISTERTYPE_KEYDOWN, reinterpret_cast<uint32_t>(wnd), wparam, &params);
            break;
        case WM_KEYUP:
            SEvtDispatch(REGISTERTYPE_KEYUP, reinterpret_cast<uint32_t>(wnd), wparam, &params);
            break;
        case WM_COMMAND:
            SEvtDispatch(REGISTERTYPE_COMMAND, reinterpret_cast<uint32_t>(wnd), wparam & 0xFFFF, &params);
            break;
        case WM_SYSCOMMAND:
            SEvtDispatch(REGISTERTYPE_SYSCOMMAND, reinterpret_cast<uint32_t>(wnd), wparam, &params);
            break;
        }

        if (!wnd || wnd != s_defaultwindow) break;
        wnd = nullptr;
    }

    if (message == WM_NCDESTROY) {
        DeleteWindow(window);
    }

    if (useresult) *useresult = params.useresult;
    if (result) *result = params.result;
    return 1;
}

HWND STORMAPI SMsgGetDefaultWindow() {
    return s_defaultwindow;
}

int32_t STORMAPI SMsgGetDefaultWindowRect(RECT* rect) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(rect);
    STORM_VALIDATE_END;

#if defined(WHOA_SYSTEM_WIN)
    if (s_defaultwindowrect.left == 0 && s_defaultwindowrect.top == 0 && s_defaultwindowrect.right == 0 && s_defaultwindowrect.bottom == 0) {
        return GetClientRect(s_defaultwindow, rect);
    }
#endif
    *rect = s_defaultwindowrect;
    return 1;
}

int32_t STORMAPI SMsgPopRegisterState(HWND window) {
    SEvtPopState(REGISTERTYPE_COMMAND, reinterpret_cast<uint32_t>(window));
    SEvtPopState(REGISTERTYPE_SYSCOMMAND, reinterpret_cast<uint32_t>(window));
    SEvtPopState(REGISTERTYPE_KEYDOWN, reinterpret_cast<uint32_t>(window));
    SEvtPopState(REGISTERTYPE_KEYUP, reinterpret_cast<uint32_t>(window));
    SEvtPopState(REGISTERTYPE_MESSAGE, reinterpret_cast<uint32_t>(window));
    return 1;
}

int32_t STORMAPI SMsgPushRegisterState(HWND window) {
    SEvtPushState(REGISTERTYPE_COMMAND, reinterpret_cast<uint32_t>(window));
    SEvtPushState(REGISTERTYPE_SYSCOMMAND, reinterpret_cast<uint32_t>(window));
    SEvtPushState(REGISTERTYPE_KEYDOWN, reinterpret_cast<uint32_t>(window));
    SEvtPushState(REGISTERTYPE_KEYUP, reinterpret_cast<uint32_t>(window));
    SEvtPushState(REGISTERTYPE_MESSAGE, reinterpret_cast<uint32_t>(window));
    return 1;
}

int32_t STORMAPI SMsgRegisterCommand(HWND window, uint32_t id, SMSGHANDLER handler) {
    return InternalRegister(REGISTERTYPE_COMMAND, window, id, handler);
}

int32_t STORMAPI SMsgRegisterKeyDown(HWND window, uint32_t id, SMSGHANDLER handler) {
    return InternalRegister(REGISTERTYPE_KEYDOWN, window, id, handler);
}

int32_t STORMAPI SMsgRegisterKeyUp(HWND window, uint32_t id, SMSGHANDLER handler) {
    return InternalRegister(REGISTERTYPE_KEYUP, window, id, handler);
}

int32_t STORMAPI SMsgRegisterMessage(HWND window, uint32_t id, SMSGHANDLER handler) {
    return InternalRegister(REGISTERTYPE_MESSAGE, window, id, handler);
}

int32_t STORMAPI SMsgRegisterSysCommand(HWND window, uint32_t id, SMSGHANDLER handler) {
    return InternalRegister(REGISTERTYPE_SYSCOMMAND, window, id, handler);
}

int32_t STORMAPI SMsgSetDefaultWindow(HWND window) {
    s_defaultwindow = window;
    return 1;
}

void STORMAPI SMsgSetDefaultWindowRect(RECT* rect) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(rect);
    STORM_VALIDATE_END;

    s_defaultwindowrect = *rect;
}

int32_t STORMAPI SMsgUnregisterCommand(HWND window, uint32_t id, SMSGHANDLER handler) {
    return InternalUnregister(REGISTERTYPE_COMMAND, window, id, handler);
}

int32_t STORMAPI SMsgUnregisterKeyDown(HWND window, uint32_t id, SMSGHANDLER handler) {
    return InternalUnregister(REGISTERTYPE_KEYDOWN, window, id, handler);
}

int32_t STORMAPI SMsgUnregisterKeyUp(HWND window, uint32_t id, SMSGHANDLER handler) {
    return InternalUnregister(REGISTERTYPE_KEYUP, window, id, handler);
}

int32_t STORMAPI SMsgUnregisterMessage(HWND window, uint32_t id, SMSGHANDLER handler) {
    return InternalUnregister(REGISTERTYPE_MESSAGE, window, id, handler);
}

int32_t STORMAPI SMsgUnregisterSysCommand(HWND window, uint32_t id, SMSGHANDLER handler) {
    return InternalUnregister(REGISTERTYPE_SYSCOMMAND, window, id, handler);
}
