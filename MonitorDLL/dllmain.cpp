// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "kernel32Redirect.h"

DWORD RedirectGetCurrentProcessId();

VOID* buffer;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    buffer = (VOID*)malloc(5);
    //Hook api calls
    hookAPICalls();

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

void hookAPICalls()
{
    VOID* proxyAddress;
    DWORD* relativeOffset;
    DWORD source;
    DWORD destination;
    CHAR patch[5] = { 0 };
    SIZE_T bytesRead;

   HINSTANCE hKernel32 = LoadLibraryA("kernel32.dll");
   VOID* functionAddress = GetProcAddress(hKernel32, "GetCurrentProcessId");

   ReadProcessMemory(GetCurrentProcess(), functionAddress, buffer, 5, &bytesRead);
   std::cout << "READ: " << bytesRead << std::endl;

   proxyAddress = &RedirectGetCurrentProcessId;
   source = (DWORD)functionAddress + 5;
   destination = (DWORD)proxyAddress;
   relativeOffset = (DWORD*)(destination - source);
   std::cout << "First: " << functionAddress;
   memcpy_s(patch, 1, "\xE9", 1);
   memcpy_s(patch + 1, 4, &relativeOffset, 4);

   WriteProcessMemory(GetCurrentProcess(), (LPVOID)functionAddress, patch, 5, NULL);
}

DWORD _stdcall RedirectGetCurrentProcessId()
{
    std::cout << "GetCurrentProcessId called\n";

    //unhook
    HINSTANCE hKernel32 = LoadLibraryA("kernel32.dll");
    VOID* functionAddress = GetProcAddress(hKernel32, "GetCurrentProcessId");
    std::cout << "Second: " << functionAddress;
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)functionAddress, buffer, 5, NULL);
    
    

    return GetCurrentProcessId();
}

