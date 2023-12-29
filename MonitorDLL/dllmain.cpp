// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

void hookAPICallSimple();

DWORD RedirectGetCurrentProcessId();

char buffer[5] = { 0 };

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    
    //Hook api calls
    

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        std::cout << "Attach\n";
        hookAPICallSimple();
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

void hookAPICallSimple()
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
   for (int i = 0; i < 5; i++)
   {
       std::cout << buffer[i] << std::endl;
   }

   proxyAddress = &RedirectGetCurrentProcessId;
   source = (DWORD)functionAddress + 5;
   destination = (DWORD)proxyAddress;
   relativeOffset = (DWORD*)(destination - source);
   std::cout << "First: " << functionAddress << std::endl;
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
    std::cout << "Second: " << functionAddress << std::endl;
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)functionAddress, buffer, 5, NULL);
    
    

    return GetCurrentProcessId();
}

