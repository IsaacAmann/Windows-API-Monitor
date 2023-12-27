// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    std::cout << "test\n";
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        std::cout << "Attach\n";
        break;
    case DLL_THREAD_ATTACH:
        std::cout << "Thread Attach\n";
    case DLL_THREAD_DETACH:
        std::cout << "Thread detach\n";
        break;
    case DLL_PROCESS_DETACH:
        std::cout << "Process detach\n";
        break;
    }
    return TRUE;
}

