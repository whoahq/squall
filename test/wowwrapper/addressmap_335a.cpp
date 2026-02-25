#include "addressdef.hpp"

#include <type_traits>

#include <storm/Core.hpp>
#include <storm/Crypto.hpp>
#include <storm/Big.hpp>
#include <storm/Error.hpp>
#include <storm/Event.hpp>
#include <storm/Memory.hpp>
#include <storm/Region.hpp>
#include <storm/Transparency.hpp>
#include <storm/Unicode.hpp>

// Hack to match only 1 ver of SStr*
#define SStrChr SStrChr_nocollide
#define SStrChrR SStrChrR_nocollide
#define SStrStr SStrStr_nocollide
#define SStrStrI SStrStrI_nocollide
#include <storm/String.hpp>
#undef SStrChr
#undef SStrChrR
#undef SStrStr
#undef SStrStrI

void STORMAPI StormInitialize();
const char* SSTRCHR_CALL SStrChr(const char* string, char search);
const char* SSTRCHR_CALL SStrChrR(const char* string, char search);
const char* SSTRCHR_CALL SStrStr(const char* string, const char* search);
const char* SSTRCHR_CALL SStrStrI(const char* string, const char* search);

#define BASE_ADDR 0x00400000
extern const uintptr_t ADDR_WINMAIN = 0x40B7F0 - BASE_ADDR; // use relative addr in loader

#define WOW_FN_STDCALL(addr,ret,T,...) const WowFn<decltype(T)> T ## _Addr(0x ## addr - BASE_ADDR); \
__declspec(naked) \
ret STORMAPI T(__VA_ARGS__) { __asm { jmp dword ptr [T ## _Addr] }}

#define WOW_FN_CDECL(addr,ret,T,...) const WowFn<decltype(T)> T ## _Addr(0x ## addr - BASE_ADDR); \
__declspec(naked) \
ret STORMCDECL T(__VA_ARGS__) { __asm { jmp dword ptr [T ## _Addr] }}


WOW_FN_STDCALL(007739E0, void, StormInitialize);
WOW_FN_STDCALL(00773A00, void, StormDestroy);

WOW_FN_STDCALL(0077EB10, int32_t, SEvtUnregisterType, uint32_t, uint32_t);

WOW_FN_STDCALL(00777980, void, SRgnCreate, HSRGN*, uint32_t);
WOW_FN_STDCALL(00777940, void, SRgnDelete, HSRGN);
WOW_FN_STDCALL(00777590, void, SRgnGetBoundingRectf, HSRGN, RECTF*);
WOW_FN_STDCALL(00777420, void, SRgnCombineRectf, HSRGN, const RECTF*, void*, int32_t);

WOW_FN_STDCALL(0076E540, void*, SMemAlloc, size_t, const char*, int32_t, uint32_t);
WOW_FN_STDCALL(0076E5A0, void, SMemFree, void*, const char*, int32_t, uint32_t);
WOW_FN_STDCALL(0076E5E0, void*, SMemReAlloc, void*, size_t, const char*, int32_t, uint32_t);

WOW_FN_STDCALL(007717E0, uint32_t, SErrGetLastError);
WOW_FN_STDCALL(007717F0, int32_t, SErrIsDisplayingError);
WOW_FN_STDCALL(00771800, void, SErrPrepareAppFatal, const char*, int32_t);
WOW_FN_STDCALL(00771870, void, SErrSetLastError, uint32_t);
WOW_FN_STDCALL(00771D10, int32_t, SErrDisplayError, uint32_t, const char*, int32_t, const char*, int32_t, uint32_t, uint32_t);
WOW_FN_CDECL(00772AA0, void, SErrDisplayAppFatal, const char* format, ...);
WOW_FN_CDECL(00772AC0, int32_t, SErrDisplayErrorFmt, uint32_t, const char*, int32_t, int32_t, uint32_t, const char*, ...);

WOW_FN_STDCALL(0077B9E0, void, SBigAdd, BigData*, BigData*, BigData*);
WOW_FN_STDCALL(0077BA00, void, SBigBitLen, BigData*, uint32_t*);
WOW_FN_STDCALL(0077AE60, int32_t, SBigCompare, BigData*, BigData*);
WOW_FN_STDCALL(0077C6A0, void, SBigDel, BigData*);
WOW_FN_STDCALL(0077BA60, void, SBigFromBinary, BigData*, const void*, uint32_t);
WOW_FN_STDCALL(0077BA80, void, SBigFromUnsigned, BigData*, uint32_t);
WOW_FN_STDCALL(0077BEE0, void, SBigMod, BigData*, BigData*, BigData*);
WOW_FN_STDCALL(0077C700, void, SBigMul, BigData*, BigData*, BigData*);
WOW_FN_STDCALL(0077BF20, void, SBigNew, BigData**);
WOW_FN_STDCALL(0077C980, void, SBigPowMod, BigData*, BigData*, BigData*, BigData*);
WOW_FN_STDCALL(0077BAA0, void, SBigSub, BigData*, BigData*, BigData*);
WOW_FN_STDCALL(0077BF90, void, SBigToBinaryBuffer, BigData*, uint8_t*, uint32_t, uint32_t*);

WOW_FN_CDECL(00468EE0, void, SARC4PrepareKey, const void*, uint32_t, SARC4Key*);
WOW_FN_CDECL(00774EA0, void, SARC4ProcessBuffer, void*, uint32_t, const SARC4Key*, SARC4Key*);

WOW_FN_CDECL(0047BD40, uint32_t, SUniSGetUTF8, const uint8_t*, int32_t*);

WOW_FN_CDECL(0076E6E0, const char*, SStrChr, const char*, char);
WOW_FN_CDECL(0076E720, const char*, SStrChrR, const char*, char);
WOW_FN_STDCALL(0076E760, int32_t, SStrCmp, const char*, const char*, size_t);
WOW_FN_STDCALL(0076E780, int32_t, SStrCmpI, const char*, const char*, size_t);
WOW_FN_STDCALL(0076ED20, size_t, SStrCopy, char*, const char*, size_t);
WOW_FN_STDCALL(0076F9E0, char*, SStrDupA, const char*, const char*, uint32_t);
WOW_FN_STDCALL(0076F340, uint32_t, SStrHash, const char*, uint32_t, uint32_t);
WOW_FN_STDCALL(0076F640, uint32_t, SStrHashHT, const char*);
WOW_FN_STDCALL(0076EE30, size_t, SStrLen, const char*);
WOW_FN_STDCALL(0076F6E0, void, SStrLower, char*);
WOW_FN_STDCALL(0076EF70, uint32_t, SStrPack, char*, const char*, uint32_t);
WOW_FN_CDECL(0076F070, size_t, SStrPrintf, char*, size_t, const char*, ...);
WOW_FN_CDECL(0076F700, const char*, SStrStr, const char*, const char*);
WOW_FN_CDECL(0076F770, const char*, SStrStrI, const char*, const char*);
WOW_FN_STDCALL(0076F1E0, void, SStrTokenize, const char**, char*, size_t, const char*, int32_t*);
WOW_FN_STDCALL(0076FB80, float, SStrToFloat, const char*);
WOW_FN_STDCALL(0076F0D0, int32_t, SStrToInt, const char*);
WOW_FN_STDCALL(0076F140, uint32_t, SStrToUnsigned, const char*);
WOW_FN_STDCALL(0076F6C0, void, SStrUpper, char*);
WOW_FN_CDECL(0076F0A0, size_t, SStrVPrintf, char*, size_t, const char*, va_list);
