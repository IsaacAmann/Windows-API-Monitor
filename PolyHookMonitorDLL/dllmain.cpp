// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include "dllmain.h"


std::vector<PLH::NatDetour*> hooks;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        std::cout << "Attach\n";
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
    //Hook Kernel32.dll functions
    hookKernel32APICalls(&hooks);

}


