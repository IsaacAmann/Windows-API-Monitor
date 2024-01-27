// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include "dllmain.h"

const DWORD MESSENGER_SLEEP_TIME = 10000;

//Switching to Iat hook
//std::vector<PLH::NatDetour*> hooks;
//std::vector<PLH::IatHook*> hooks;
std::unordered_map<std::string, APICallCounter *> counterMap;
std::string pipeBaseName = "\\\\.\\pipe\\APIMonitor";

HANDLE pipeHandle; 
HANDLE messengerThread;

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
  
        //Hook API calls
        hookAPICalls();
       
        //Start messenger thread
        messengerThread = CreateThread(NULL, 0, MessengerThreadExecute, NULL, 0, NULL);
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
    hookwinsock32APICalls(&counterMap);
}

DWORD WINAPI MessengerThreadExecute(LPVOID lpParam)
{
    while (true)
    {
        //Send update for all counters to monitor
        for (auto iterator : counterMap)
        {
            APICallCounter* currentCounter = iterator.second;
            //Create message
            CountUpdateMessage message;
            message.calls = currentCounter->numberCalls;
            //Copy API call name
            memcpy(message.callName, currentCounter->callName.c_str(), currentCounter->callName.length());
            //Add null byte to the end
            message.callName[currentCounter->callName.length()] = '\0';

            //Send messenge over named pipe
            WriteFile(
                pipeHandle,
                &message,
                sizeof(CountUpdateMessage),
                NULL,
                NULL
            );
        }

        //Wait before sending again
        Sleep(MESSENGER_SLEEP_TIME);
    }
}
