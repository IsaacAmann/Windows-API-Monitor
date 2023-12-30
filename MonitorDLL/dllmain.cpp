// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

typedef  DWORD (__stdcall *defTrampolineFunc)();

typedef void(voidTrampolineFunc)();

void hookAPICallSimple();

void hookWithTrampoline();

void* createTrampoline(void* functionAddress);

DWORD RedirectGetCurrentProcessId();
DWORD _stdcall trampolineGetCurrentProcessId();

char hookAPICallSimpleBuffer[5] = { 0 };
LPVOID trampolineAddress; 

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    
    

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        std::cout << "Attach\n";
       // hookAPICallSimple();
        hookWithTrampoline();
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

   ReadProcessMemory(GetCurrentProcess(), functionAddress, hookAPICallSimpleBuffer, 5, &bytesRead);
   std::cout << "READ: " << bytesRead << std::endl;
   for (int i = 0; i < 5; i++)
   {
       std::cout << hookAPICallSimpleBuffer[i] << std::endl;
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

void hookWithTrampoline()
{
    HINSTANCE libHandle;
    VOID* proxyAddress;
    DWORD* relativeOffset;
    DWORD* hookAddress;
    DWORD source;
    DWORD destination;
    CHAR patch[5] = { 0 };
    char buffer[5];
    FARPROC functionAddress = NULL;

    libHandle = LoadLibraryA("Kernel32.dll");
    functionAddress = GetProcAddress(libHandle, "GetCurrentProcessId");

    ReadProcessMemory(GetCurrentProcess(), functionAddress, buffer, 5, NULL);

    proxyAddress = &trampolineGetCurrentProcessId;
    source = (DWORD)functionAddress + 5; 
    destination = (DWORD)proxyAddress;
    relativeOffset = (DWORD*)(destination - source);
    memcpy(patch, "\xE9", 1);
    memcpy(patch + 1, &relativeOffset, 4);
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)functionAddress, patch, 5, NULL);

    //Could not get this method of creating the trampoline function to work
    //Was getting access violations 
    /*
    trampolineAddress = VirtualAlloc(NULL, 11, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    hookAddress = (DWORD*)((DWORD)functionAddress + 5);
    memcpy((BYTE*)trampolineAddress, &buffer, 5);
    //x68 is PUSH assembly instruction
    memcpy((BYTE*)trampolineAddress + 5, "\x68", 1);
    memcpy((BYTE*)trampolineAddress + 6, &hookAddress, 4);
    //xC3 is RETN assembly instruction
    memcpy((BYTE*)trampolineAddress + 10, "\xC3", 1);
    std::cout << "trampoline address: " << trampolineAddress << std::endl;
    */
    trampolineAddress = createTrampoline((void*)functionAddress);
}

void* createTrampoline(void* functionAddress)
{
    uint8_t* trampoline = (uint8_t*) VirtualAlloc(NULL, 10, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(trampoline, functionAddress, 5);

    trampoline[5] = 0xE9;

    uint32_t destination = (uint32_t)functionAddress + 5;
    uint32_t source = (uint32_t)trampoline + 10;
    uint32_t delta = destination - source;
    memcpy(trampoline + 6, &delta, 4);

    //VirtualProtect(trampoline, 10, PAGE_EXECUTE_READ, NULL);

    return trampoline;
}

DWORD _stdcall trampolineGetCurrentProcessId()
{
    std::cout << "GetCurrentProcessId Called" << std::endl;
    std::cout << "trampoline address: " << trampolineAddress << std::endl;

    defTrampolineFunc trampoline = (defTrampolineFunc) trampolineAddress;
    return 1;
}

DWORD _stdcall RedirectGetCurrentProcessId()
{
    std::cout << "GetCurrentProcessId called\n";

    //unhook
    HINSTANCE hKernel32 = LoadLibraryA("kernel32.dll");
    VOID* functionAddress = GetProcAddress(hKernel32, "GetCurrentProcessId");
    std::cout << "Second: " << functionAddress << std::endl;
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)functionAddress, hookAPICallSimpleBuffer, 5, NULL);
    
    

    return GetCurrentProcessId();
}

