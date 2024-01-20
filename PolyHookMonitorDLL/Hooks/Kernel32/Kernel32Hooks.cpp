#include "Kernel32Hooks.h"

//trampoline function addresses
uint64_t trampolineGetCurrentProcessId;
uint64_t trampolineOpenProcess;

//Function pointers to original Win32 API functions
DWORD (WINAPI * origGetCurrentProcessId)() = GetCurrentProcessId;

void hookKernel32APICalls(std::unordered_map<std::string, APICallCounter *> * hooks)
{
    /*
    HMODULE kernel32 = LoadLibraryA("Kernel32.dll");
    //Hook GetCurrentProcessId
    uint64_t callBack = (uint64_t)hookGetCurrentProcessId;
    uint64_t targetAddress = (uint64_t)GetProcAddress(kernel32, "GetCurrentProcessId");
    PLH::NatDetour* currentHook = NULL;
    APICallCounter* currentCounter = NULL;

    currentHook = new PLH::NatDetour(targetAddress, callBack, &trampolineGetCurrentProcessId);
    currentCounter = new APICallCounter(currentHook, "GetCurrentProcessId", hooks);


    //Hook OpenProcess
    callBack = (uint64_t)hookOpenProcess;
    targetAddress = (uint64_t)GetProcAddress(kernel32, "OpenProcess");
    currentHook = new PLH::NatDetour(targetAddress, callBack, &trampolineOpenProcess);
    currentCounter = new APICallCounter(currentHook, "OpenProcess", hooks);
    */
    APICallCounter* currentCounter = NULL;


    //Hooking using Microsoft Detours library
    currentCounter = new APICallCounter("GetCurrentProcessId", hooks);
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&origGetCurrentProcessId, hookGetCurrentProcessId);
    DetourTransactionCommit();

}

/*
//Hook function for GetCurrentProcessId
DWORD __stdcall hookGetCurrentProcessId()
{
    std::cout << "Called get id\n";
    counterMap.at("GetCurrentProcessId")->incrementCall();
    return PLH::FnCast(trampolineGetCurrentProcessId, &GetCurrentProcessId)();
}


//Hook function for OpenProcess
HANDLE __stdcall hookOpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId)
{
    std::cout << "Called OpenProcess\n";
    counterMap.at("OpenProcess")->incrementCall();

    return PLH::FnCast(trampolineOpenProcess, &OpenProcess)(dwDesiredAccess, bInheritHandle, dwDesiredAccess);
}
*/

DWORD WINAPI hookGetCurrentProcessId()
{
    std::cout << "Called get id\n";
    counterMap.at("GetCurrentProcessId")->incrementCall();
//    DWORD returnValue = origGetCurrentProcessId();
    return origGetCurrentProcessId();
}