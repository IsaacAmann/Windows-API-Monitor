// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include "dllmain.h"

const DWORD MESSENGER_SLEEP_TIME = 10000;

//Switching to Iat hook
//std::vector<PLH::NatDetour*> hooks;
//std::vector<PLH::IatHook*> hooks;
std::unordered_map<std::string, APICallCounter *> counterMap;
std::string pipeBaseName = "Local\\APIMonitor";

HANDLE pipeHandle; 
HANDLE messengerThread;

HANDLE sharedMemoryHandle;
CallCountContainer* callCountContainer;

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
     
        //Open shared memory from monitor process
        std::string pipeName = pipeBaseName;
        pipeName.append(std::to_string(GetCurrentProcessId()));
        std::cout << pipeName << std::endl;
        std::wstring temp = std::wstring(pipeName.begin(), pipeName.end());
        LPCWSTR fullString = temp.c_str();
        sharedMemoryHandle = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, fullString);
        //Catch that sharedMemoryHandle not void, would cause a crash on attached process :(
        if (sharedMemoryHandle == NULL)
        {
            return true;
        }
        callCountContainer = (CallCountContainer*)MapViewOfFile(sharedMemoryHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CallCountContainer));

        //Hook API calls
        hookAPICalls();
       
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
    hookKernel32APICalls(&counterMap, callCountContainer);
    hookwinsock32APICalls(&counterMap, callCountContainer);
    hookwininetAPICalls(&counterMap, callCountContainer);
    hookadvapi32APICalls(&counterMap, callCountContainer);
    hookUser32APICalls(&counterMap, callCountContainer);
}

