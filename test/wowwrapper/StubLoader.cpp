#include <windows.h>

#include <cstdio>
#include <cstring>
#include <string>

const char EXE_BINARY_NAME[] = "WoW.exe";
const char TEST_BINARY_NAME[] = "StormTestActual.dll";
extern const uintptr_t ADDR_WINMAIN;

template<class... Args>
bool error(const char* format, Args... args) {
    std::fprintf(stderr, format, args...);
    return false;
}

std::string GetFullPath(const std::string& path) {
    char basepath[MAX_PATH];
    GetModuleFileNameA(nullptr, basepath, sizeof(basepath));

    std::string result {basepath};
    return result.substr(0, result.find_last_of("\\/")) + "\\" + path;
}

std::string GetExePath() {
    return GetFullPath(EXE_BINARY_NAME);
}

std::string GetTestPath() {
    return GetFullPath(TEST_BINARY_NAME);
}

bool Inject(HANDLE hProcess) {
    void* remoteStr = VirtualAllocEx(hProcess, 0, MAX_PATH, MEM_COMMIT, PAGE_READWRITE);
    if (!remoteStr) return false;

    std::string testPath = GetTestPath();
    if (!WriteProcessMemory(hProcess, remoteStr, testPath.c_str(), testPath.length() + 1, 0)) {
        return false;
    }

    LPTHREAD_START_ROUTINE load = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32"), "LoadLibraryA");
    HANDLE hNewThread = CreateRemoteThread(hProcess, nullptr, 0, load, remoteStr, 0, nullptr);
    return hNewThread != nullptr;
}

BOOL MakePageWritable(HANDLE hProcess, uintptr_t addr, uint32_t size) {
    DWORD oldProt; // useless var needed or it fails
    return VirtualProtectEx(hProcess, reinterpret_cast<void*>(addr), size, PAGE_EXECUTE_READWRITE, &oldProt);
}

uintptr_t GetBaseAddress(HANDLE hProcess, HANDLE hThread) {
    CONTEXT ctx = {};
    ctx.ContextFlags = CONTEXT_INTEGER;
    GetThreadContext(hThread, &ctx);

    uintptr_t result = 0;
    // Hack where ebx = PEB
    ReadProcessMemory(hProcess, reinterpret_cast<void*>(ctx.Ebx + 8), &result, sizeof(result), nullptr);
    return result;
}

bool Hijack(HANDLE hProcess, HANDLE hThread) {
    uintptr_t base = GetBaseAddress(hProcess, hThread);
    uintptr_t winmain_ptr = base + ADDR_WINMAIN;

    void* runFlagAddr = VirtualAllocEx(hProcess, nullptr, 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!runFlagAddr) {
        return error("Failed to allocate run flag\n");
    }

    // Prevent executing WinMain by looping infinitely
    uint8_t sleepforever[] = {
        0xB8, 0, 0, 0, 0,   // mov eax, flag
        0xC6, 0x00, 0x01,   // mov byte ptr [eax], 1
        0xF4,               // hlt (no 100% CPU)
        0xEB, 0xFD          // jmp $-3
    };
    *reinterpret_cast<void**>(&sleepforever[1]) = runFlagAddr;

    if (!MakePageWritable(hProcess, winmain_ptr, sizeof(sleepforever))) {
        return error("Failed to make mem writable: %08X\n", GetLastError());
    }

    if (!WriteProcessMemory(hProcess, reinterpret_cast<void*>(winmain_ptr), sleepforever, sizeof(sleepforever), nullptr)) {
        return error("Failed to write memory\n");
    }

    // Run to WinMain
    ResumeThread(hThread);

    uint8_t flag = 0;
    while (!flag) {
        ReadProcessMemory(hProcess, runFlagAddr, &flag, sizeof(flag), nullptr);
    }
    return true;
}

bool LaunchProcess(PROCESS_INFORMATION* pi) {
    std::string cmd = GetExePath();
    std::string dir = cmd.substr(0, cmd.find_last_of("/\\"));

    const char* args = GetCommandLineA();
    args = std::strchr(args, ' ');
    if (args != nullptr) {
        cmd += args;
    }

    STARTUPINFOA si = { sizeof(STARTUPINFOA) };
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    char mutablecmd[8192];
    std::strncpy(mutablecmd, cmd.c_str(), sizeof(mutablecmd));
    mutablecmd[sizeof(mutablecmd)-1] = '\0';

    if (!CreateProcessA(0, mutablecmd, 0, 0, TRUE, CREATE_SUSPENDED, 0, dir.c_str(), &si, pi)) {
        return false;
    }
    return true;
}

bool RunLoader() {
    // Launch the process in a suspended state
    PROCESS_INFORMATION pi = {};
    if (!LaunchProcess(&pi)) {
        return error("Failed to start process; error %d\n", GetLastError());
    }

    // Hijack the process to finish loading and permanently stop at WinMain
    if (!Hijack(pi.hProcess, pi.hThread)) {
        return error("Hijacking failed\n");
    }

    // Inject the test module into the process
    if (!Inject(pi.hProcess)) {
        return error("Inject failed\n");
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    return true;
}

int main(int argc, char* argv[]) {
    if (!RunLoader()) return -1;
    return 0;
}
