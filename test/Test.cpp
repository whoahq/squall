#define CATCH_CONFIG_RUNNER
#include "test/Test.hpp"

#if defined(WHOA_TEST_WOWREAL)
#include <Windows.h>
#include <shellapi.h>

#include "storm/Core.hpp"
#include "wowwrapper/loader.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

void STORMAPI StormInitialize();

std::vector<std::string> GetCommandLineArgs() {
    std::vector<std::string> args;

    int argc = 0;
    LPWSTR *argvw = CommandLineToArgvW(GetCommandLineW(), &argc);
    char szStr[1024] = {};
    for (int i = 0; i < argc; i++) {
        WideCharToMultiByte(CP_ACP, 0, argvw[i], -1, szStr, sizeof(szStr), nullptr, nullptr);
        args.emplace_back(szStr);
    }
    LocalFree(argvw);
    return args;
}

int RunTests() {
    const std::vector<std::string> args = GetCommandLineArgs();
    std::vector<const char*> argv;
    for (auto& str : args) {
        argv.emplace_back(str.c_str());
    }
    return Catch::Session().run(argv.size(), argv.data());
}

DWORD WINAPI RunThread(LPVOID) {
    StormInitialize();
    int result = RunTests();
    StormDestroy();
    std::exit(result);
    return result;
}

void ConnectConsole() {
    AttachConsole(ATTACH_PARENT_PROCESS);

    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONIN$",  "r", stdin);
    std::ios::sync_with_stdio();
    std::printf("Console attached\n");
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    static bool initialized = false;
    if (fdwReason == DLL_PROCESS_ATTACH && !initialized) {
        ConnectConsole();
        WoWLoader::Init();
        CreateThread(nullptr, 0, RunThread, hinstDLL, 0, nullptr);
        initialized = true;
    }
    return TRUE;
}

#else

int main(int argc, char* argv[]) {
    return Catch::Session().run( argc, argv );
}

#endif
