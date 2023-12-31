// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include "dllmain.h"


std::vector<PLH::NatDetour*> hooks;
std::unordered_map<std::string, APICallCounter *> counterMap;
std::string pipeBaseName = "\\\\.\\pipe\\APIMonitor";

HANDLE pipeHandle; 

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        std::cout << "Attach\n";
        //Connect to named pipe
        std::string pipeName = pipeBaseName;
        pipeName.append(std::to_string(GetCurrentProcessId()));
        std::wstring temp = std::wstring(pipeName.begin(), pipeName.end());
        LPCWSTR string = temp.c_str();
        pipeHandle = CreateFile(
            string,
            GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );
        CountUpdateMessage test;
        char testString[60] = "test";
        memcpy(test.callName, testString, 60);
        test.calls = 4;
        WriteFile(
            pipeHandle,
            &test,
            sizeof(CountUpdateMessage),
            NULL,
            NULL
        );
        //Hook API calls
        hookAPICalls();
    }
        break;
    case DLL_THREAD_ATTACH:
        std::cout << "Thread attach\n";
        break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void hookAPICalls()
{
    //Hook Kernel32.dll functions
    hookKernel32APICalls(&counterMap);

}


