// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include "dllmain.h"

void hookAPICalls();
DWORD __stdcall hookGetCurrentProcessId();

PLH::NatDetour *detourGetCurrentProcessId = NULL;
uint64_t trampolineGetCurrentProcessId;
uint64_t targetAddress = (uint64_t)GetProcAddress(LoadLibraryA("Kernel32.dll"), "GetCurrentProcessId");

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        std::cout << "Attaacch\n";
        hookAPICalls();
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
    
    uint64_t callBack = (uint64_t)hookGetCurrentProcessId;
    
    
    detourGetCurrentProcessId = new PLH::NatDetour(targetAddress, callBack, &trampolineGetCurrentProcessId);

    detourGetCurrentProcessId->hook();

}

DWORD __stdcall hookGetCurrentProcessId()
{
    std::cout << "Called get id\n";

    return PLH::FnCast(trampolineGetCurrentProcessId, &GetCurrentProcessId)();
}
