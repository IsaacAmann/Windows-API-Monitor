// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include "dllmain.h"

//Switching to Iat hook
//std::vector<PLH::NatDetour*> hooks;
//std::vector<PLH::IatHook*> hooks;
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
        std::cout << pipeName << std::endl;
        std::wstring temp = std::wstring(pipeName.begin(), pipeName.end());
        LPCWSTR fullString = temp.c_str();
        printf("%ls\n", fullString);
        do
        {
            pipeHandle = CreateFile(
                fullString,
                GENERIC_WRITE,
                0,
                NULL,
                OPEN_EXISTING,
                0,
                NULL
            );
            std::cout << "trying to connect" << std::endl;
        } while (pipeHandle == INVALID_HANDLE_VALUE);
        /*
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
        */
        //Hook API calls
        //Some programs seem to hang when being injected
        //Possibly run this in seperate thread with a time out to see if this is where it is hanging
        hookAPICalls();
        //std::thread hookThread = std::thread(hookAPICalls);
        //HANDLE hookThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)hookAPICalls, NULL, 0, NULL);
       // WaitForSingleObject(hookThread, 3000);
    }
        break;
    case DLL_THREAD_ATTACH:
        std::cout << "Thread attach\n";
        break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        //Unhook all API calls
        break;
    }
    return TRUE;
}

void hookAPICalls()
{
    //Hook Kernel32.dll functions
    hookKernel32APICalls(&counterMap);

}


