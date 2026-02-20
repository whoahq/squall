#include <storm/Core.hpp>

int32_t STORMAPI StormDestroy() { return 0; }
HINSTANCE STORMAPI StormGetInstance() { return 0; }

#include <storm/Big.hpp>

void STORMAPI SBigAdd(BigData*, BigData*, BigData*) {}
void STORMAPI SBigAnd(BigData*, BigData*, BigData*) {}
int32_t STORMAPI SBigCompare(BigData*, BigData*) { return 0; }
void STORMAPI SBigCopy(BigData*, BigData*) {}
void STORMAPI SBigDec(BigData*, BigData*) {}
void STORMAPI SBigDel(BigData*) {}
void STORMAPI SBigDiv(BigData*, BigData*, BigData*) {}
void STORMAPI SBigFromBinary(BigData*, const void*, uint32_t) {}
void STORMAPI SBigFromStr(BigData*, const char*) {}
void STORMAPI SBigFromUnsigned(BigData*, uint32_t) {}
void STORMAPI SBigInc(BigData*, BigData*) {}
int32_t STORMAPI SBigIsEven(BigData*) {return 0;}
int32_t STORMAPI SBigIsOdd(BigData*) {return 0;}
int32_t STORMAPI SBigIsOne(BigData*) {return 0;}
int32_t STORMAPI SBigIsZero(BigData*) {return 0;}
void STORMAPI SBigMod(BigData*, BigData*, BigData*) {}
void STORMAPI SBigMul(BigData*, BigData*, BigData*) {}
void STORMAPI SBigNew(BigData**) {}
void STORMAPI SBigNot(BigData*, BigData*) {}
void STORMAPI SBigOr(BigData*, BigData*, BigData*) {}
void STORMAPI SBigPowMod(BigData*, BigData*, BigData*, BigData*) {}
void STORMAPI SBigShl(BigData*, BigData*, uint32_t) {}
void STORMAPI SBigShr(BigData*, BigData*, uint32_t) {}
void STORMAPI SBigSquare(BigData*, BigData*) {}
void STORMAPI SBigSub(BigData*, BigData*, BigData*) {}
void STORMAPI SBigToBinaryBuffer(BigData*, uint8_t*, uint32_t, uint32_t*) {}
void STORMAPI SBigXor(BigData*, BigData*, BigData*) {}
void STORMAPI SBigToUnsigned(BigData*, uint32_t*) {}

#include <storm/Error.hpp>

void STORMCDECL SErrDisplayAppFatal(const char* format, ...) {}
int32_t STORMAPI SErrDisplayError(uint32_t, const char*, int32_t, const char*, int32_t, uint32_t) { return 0; }
int32_t STORMCDECL SErrDisplayErrorFmt(uint32_t, const char*, int32_t, int32_t, uint32_t, const char*, ...) { return 0; }
int32_t STORMAPI SErrIsDisplayingError() { return 0; }
void STORMAPI SErrPrepareAppFatal(const char*, int32_t) {}
void STORMAPI SErrSetLastError(uint32_t) {}
uint32_t STORMAPI SErrGetLastError() { return 0; }
void STORMAPI SErrSuppressErrors(int32_t) {}

#include <storm/Event.hpp>

int32_t STORMAPI SEvtBreakHandlerChain(void*) { return 0; }
int32_t STORMAPI SEvtDestroy() { return 0; }
int32_t STORMAPI SEvtDispatch(uint32_t, uint32_t, uint32_t, void*) { return 0; }
int32_t STORMAPI SEvtPopState(uint32_t, uint32_t) { return 0; }
int32_t STORMAPI SEvtPushState(uint32_t, uint32_t) { return 0; }
int32_t STORMAPI SEvtRegisterHandler(uint32_t, uint32_t, uint32_t, uint32_t, SEVTHANDLER) { return 0; }
int32_t STORMAPI SEvtUnregisterHandler(uint32_t, uint32_t, uint32_t, SEVTHANDLER) { return 0; }
int32_t STORMAPI SEvtUnregisterType(uint32_t, uint32_t) { return 0; }

#include <storm/Memory.hpp>

void* STORMAPI SMemAlloc(size_t, const char*, int32_t, uint32_t) { return 0; }
int STORMAPI SMemCmp(void*, void*, size_t) { return 0; }
void STORMAPI SMemCopy(void*, void*, size_t) {}
void STORMAPI SMemFill(void*, size_t, uint8_t) {}
void STORMAPI SMemFree(void*, const char*, int32_t, uint32_t) {}
void STORMAPI SMemMove(void*, void*, size_t) {}
void* STORMAPI SMemReAlloc(void*, size_t, const char*, int32_t, uint32_t) { return 0; }
void STORMAPI SMemZero(void*, size_t) {}

#include <storm/Region.hpp>

void STORMAPI SRgnClear(HSRGN) {}
void STORMAPI SRgnCombineRectf(HSRGN, const RECTF*, void*, int32_t) {}
void STORMAPI SRgnCombineRecti(HSRGN, const RECT*, void*, int32_t) {}
void STORMAPI SRgnCreate(HSRGN*, uint32_t) {}
void STORMAPI SRgnDelete(HSRGN) {}
void STORMAPI SRgnDuplicate(HSRGN, HSRGN*, uint32_t) {}
void STORMAPI SRgnGetBoundingRectf(HSRGN, RECTF*) {}
void STORMAPI SRgnGetBoundingRecti(HSRGN, RECT*) {}
void STORMAPI SRgnGetRectParamsf(HSRGN, const RECTF*, uint32_t*, void**) {}
void STORMAPI SRgnGetRectParamsi(HSRGN, const RECT*, uint32_t*, void**) {}
void STORMAPI SRgnGetRectsf(HSRGN, uint32_t*, RECTF*) {}
void STORMAPI SRgnGetRectsi(HSRGN, uint32_t*, RECT*) {}
int32_t STORMAPI SRgnIsPointInRegionf(HSRGN, float, float) { return 0; }
int32_t STORMAPI SRgnIsPointInRegioni(HSRGN, int32_t, int32_t) { return 0; }
int32_t STORMAPI SRgnIsRectInRegionf(HSRGN, const RECTF*) { return 0; }
int32_t STORMAPI SRgnIsRectInRegioni(HSRGN, const RECT*) { return 0; }
void STORMAPI SRgnOffsetf(HSRGN, float, float) {}
void STORMAPI SRgnOffseti(HSRGN, int32_t, int32_t) {}

#include <storm/String.hpp>

char* STORMAPI SStrChr(char*, char) { return 0; }
const char* STORMAPI SStrChr(const char*, char) { return 0; }
const char* STORMAPI SStrChrBidir(const char*, char, int32_t) { return 0; }
char* STORMAPI SStrChrR(char*, char) { return 0; }
const char* STORMAPI SStrChrR(const char*, char) { return 0; }
int32_t STORMAPI SStrCmp(const char*, const char*, size_t) { return 0; }
int32_t STORMAPI SStrCmpI(const char*, const char*, size_t) { return 0; }
size_t STORMAPI SStrCopy(char*, const char*, size_t) { return 0; }
char* STORMAPI SStrDupA(const char*, const char*, uint32_t) { return 0; }
uint32_t STORMAPI SStrHash(const char*, uint32_t, uint32_t) { return 0; }
uint32_t STORMAPI SStrHashHT(const char*) { return 0; }
int64_t STORMAPI SStrHash64(const char*, uint32_t, int64_t) { return 0; }
size_t STORMAPI SStrLen(const char*) { return 0; }
void STORMAPI SStrLower(char*) {}
uint32_t STORMAPI SStrPack(char*, const char*, uint32_t) { return 0; }
size_t STORMCDECL SStrPrintf(char*, size_t, const char*, ...) { return 0; }
size_t STORMCDECL SStrVPrintf(char*, size_t, const char*, va_list) { return 0; }
char* STORMAPI SStrStr(char*, const char*) { return 0; }
const char* STORMAPI SStrStr(const char*, const char*) { return 0; }
char* STORMAPI SStrStrI(char*, const char*) { return 0; }
const char* STORMAPI SStrStrI(const char*, const char*) { return 0; }
void STORMAPI SStrTokenize(const char**, char*, size_t, const char*, int32_t*) {}
double STORMAPI SStrToDouble(const char*) { return 0; }
float STORMAPI SStrToFloat(const char*) { return 0; }
int32_t STORMAPI SStrToInt(const char*) { return 0; }
uint32_t STORMAPI SStrToUnsigned(const char*) { return 0; }
void STORMAPI SStrUpper(char*) {}

#include <storm/Transparency.hpp>

int32_t STORMAPI STransBlt(uint8_t*, int32_t, int32_t, int32_t, HSTRANS) { return 0; }
int32_t STORMAPI STransBltUsingMask(uint8_t*, uint8_t*, int32_t, int32_t, HSTRANS) { return 0; }
int32_t STORMAPI STransCombineMasks(HSTRANS, HSTRANS, int32_t, int32_t, uint32_t, HSTRANS*) { return 0; }
int32_t STORMAPI STransCreateE(uint8_t*, int32_t, int32_t, int32_t, RECT*, uint32_t, HSTRANS*) { return 0; }
int32_t STORMAPI STransCreateI(uint8_t*, int32_t, int32_t, int32_t, RECT*, uint32_t, HSTRANS*) { return 0; }
int32_t STORMAPI STransCreateMaskE(uint8_t*, int32_t, int32_t, int32_t, RECT*, uint32_t, HSTRANS*) { return 0; }
int32_t STORMAPI STransCreateMaskI(uint8_t*, int32_t, int32_t, int32_t, RECT*, uint32_t, HSTRANS*) { return 0; }
int32_t STORMAPI STransDelete(HSTRANS) { return 0; }
int32_t STORMAPI STransDestroy() { return 0; }
int32_t STORMAPI STransDuplicate(HSTRANS, HSTRANS*) { return 0; }
int32_t STORMAPI STransIntersectDirtyArray(HSTRANS, uint8_t*, uint8_t, HSTRANS*) { return 0; }
int32_t STORMAPI STransInvertMask(HSTRANS, HSTRANS*) { return 0; }
int32_t STORMAPI STransIsPixelInMask(HSTRANS, int32_t, int32_t) { return 0; }
int32_t STORMAPI STransSetDirtyArrayInfo(int32_t, int32_t, int32_t, int32_t) { return 0; }
int32_t STORMAPI STransUpdateDirtyArray(uint8_t*, uint8_t, int32_t, int32_t, HSTRANS, int32_t) { return 0; }

#include <storm/Unicode.hpp>

ptrdiff_t STORMAPI SUniConvertUTF16ToDos(char*, const char16_t*, uint32_t) { return 0; };
ptrdiff_t STORMAPI SUniConvertUTF16ToMac(char*, const char16_t*, uint32_t) { return 0; };
ptrdiff_t STORMAPI SUniConvertUTF16ToWin(char*, const char16_t*, uint32_t) { return 0; };
ptrdiff_t STORMAPI SUniConvertDosToUTF16(char16_t*, const char*, uint32_t) { return 0; };
ptrdiff_t STORMAPI SUniConvertMacToUTF16(char16_t*, const char*, uint32_t) { return 0; };
ptrdiff_t STORMAPI SUniConvertWinToUTF16(char16_t*, const char*, uint32_t) { return 0; };
int32_t STORMAPI SUniFindAfterUTF8Chr(const char*, int32_t) { return 0; };
int32_t STORMAPI SUniFindUTF8ChrStart(const char*, int32_t) { return 0; };
uint32_t STORMAPI SUniSGetUTF8(const uint8_t*, int32_t*) { return 0; }
void STORMAPI SUniSPutUTF8(uint32_t, char*) {}
