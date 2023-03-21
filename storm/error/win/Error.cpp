#include "storm/Error.hpp"
#include "storm/String.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <Windows.h>

void GetExceptionNameWin32(DWORD dwMessageId, char* lpBuffer, DWORD nSize) {
    switch (dwMessageId) {
    case EXCEPTION_GUARD_PAGE:
        SStrCopy(lpBuffer, "GUARD_PAGE", nSize);
        break;

    case EXCEPTION_DATATYPE_MISALIGNMENT:
        SStrCopy(lpBuffer, "DATATYPE_MISALIGNMENT", nSize);
        break;

    case EXCEPTION_BREAKPOINT:
        SStrCopy(lpBuffer, "BREAKPOINT", nSize);
        break;

    case EXCEPTION_SINGLE_STEP:
        SStrCopy(lpBuffer, "SINGLE_STEP", nSize);
        break;

    case EXCEPTION_ACCESS_VIOLATION:
        SStrCopy(lpBuffer, "ACCESS_VIOLATION", nSize);
        break;

    case EXCEPTION_IN_PAGE_ERROR:
        SStrCopy(lpBuffer, "IN_PAGE_ERROR", nSize);
        break;

    case EXCEPTION_INVALID_HANDLE:
        SStrCopy(lpBuffer, "INVALID_HANDLE", nSize);
        break;

    case EXCEPTION_ILLEGAL_INSTRUCTION:
        SStrCopy(lpBuffer, "ILLEGAL_INSTRUCTION", nSize);
        break;

    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        SStrCopy(lpBuffer, "NONCONTINUABLE_EXCEPTION", nSize);
        break;

    case EXCEPTION_INVALID_DISPOSITION:
        SStrCopy(lpBuffer, "INVALID_DISPOSITION", nSize);
        break;

    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        SStrCopy(lpBuffer, "ARRAY_BOUNDS_EXCEEDED", nSize);
        break;

    case EXCEPTION_FLT_DENORMAL_OPERAND:
        SStrCopy(lpBuffer, "FLT_DENORMAL_OPERAND", nSize);
        break;

    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        SStrCopy(lpBuffer, "FLT_DIVIDE_BY_ZERO", nSize);
        break;

    case EXCEPTION_FLT_INEXACT_RESULT:
        SStrCopy(lpBuffer, "FLT_INEXACT_RESULT", nSize);
        break;

    case EXCEPTION_FLT_OVERFLOW:
        SStrCopy(lpBuffer, "FLT_OVERFLOW", nSize);
        break;

    case EXCEPTION_FLT_STACK_CHECK:
        SStrCopy(lpBuffer, "FLT_STACK_CHECK", nSize);
        break;

    case EXCEPTION_FLT_UNDERFLOW:
        SStrCopy(lpBuffer, "FLT_UNDERFLOW", nSize);
        break;

    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        SStrCopy(lpBuffer, "INT_DIVIDE_BY_ZERO", nSize);
        break;

    case EXCEPTION_INT_OVERFLOW:
        SStrCopy(lpBuffer, "INT_OVERFLOW", nSize);
        break;

    case EXCEPTION_PRIV_INSTRUCTION:
        SStrCopy(lpBuffer, "PRIV_INSTRUCTION", nSize);
        break;

    case EXCEPTION_STACK_OVERFLOW:
        SStrCopy(lpBuffer, "STACK_OVERFLOW", nSize);
        break;

    default:
        HMODULE lpSource = GetModuleHandle("ntdll.dll");
        if (!lpSource || !FormatMessage(FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS, lpSource, dwMessageId, 0, lpBuffer, nSize, nullptr)) {
            SStrCopy(lpBuffer, "unknown exception", nSize);
        }
        break;
    }
}
