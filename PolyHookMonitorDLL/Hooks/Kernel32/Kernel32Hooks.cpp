#include "Kernel32Hooks.h"

const bool PRINT_CALLS = true;

//trampoline function addresses
uint64_t trampolineGetCurrentProcessId;
uint64_t trampolineOpenProcess;

//Function pointers to original Win32 API functions
DWORD (WINAPI * origGetCurrentProcessId)() = GetCurrentProcessId;
HANDLE(WINAPI * origOpenProcess)(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId) = OpenProcess;
//WriteFile
BOOL(WINAPI * origWriteFile)(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) = WriteFile;
BOOL(WINAPI * origWriteFileEx)(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionroutine) = WriteFileEx;
//CreateFile
HANDLE(WINAPI * origCreateFileA)(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) = CreateFileA;
HANDLE(WINAPI * origCreateFileW)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) = CreateFileW;


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
    DetourAttach(&origGetCurrentProcessId, hookGetCurrentProcessId);

    currentCounter = new APICallCounter("OpenProcess", hooks);
    DetourAttach(&origOpenProcess, hookOpenProcess);


    //Hooking WriteFile, WriteFileEx (using 1 counter for both)
    currentCounter = new APICallCounter("WriteFile", hooks);
    //Probably not going to hook WriteFile, DLL uses write file to send updated count over named pipe
    //resulting in a stack over flow from it infinitely calling the hook function
    /*
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&origWriteFile, hookWriteFile);
    DetourTransactionCommit();
    */
    DetourAttach(&origWriteFileEx, hookWriteFileEx);


    //Hooking CreateFileA, CreateFileW (using 1 counter for both)
    currentCounter = new APICallCounter("CreateFile", hooks);
    DetourAttach(&origCreateFileA, hookCreateFileA);
   
    DetourAttach(&origCreateFileW, hookCreateFileW);

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
    if (PRINT_CALLS)
        std::cout << "Called get id\n";
    counterMap.at("GetCurrentProcessId")->incrementCall();

    return origGetCurrentProcessId();
}

HANDLE WINAPI hookOpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId)
{
    if (PRINT_CALLS)
        std::cout << "Called OpenProcess\n";
    counterMap.at("OpenProcess")->incrementCall();

    return origOpenProcess(dwDesiredAccess, bInheritHandle, dwDesiredAccess);
}

BOOL WINAPI hookWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
    if (PRINT_CALLS)
        std::cout << "Called WriteFile\n";
    counterMap.at("WriteFile")->incrementCall();

    return origWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

BOOL WINAPI hookWriteFileEx(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionroutine)
{
    if (PRINT_CALLS)
        std::cout << "Called WriteFileEx\n";
    counterMap.at("WriteFile")->incrementCall();

    return origWriteFileEx(hFile, lpBuffer, nNumberOfBytesToWrite, lpOverlapped, lpCompletionroutine);
}

HANDLE WINAPI hookCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    if (PRINT_CALLS)
        std::cout << "Called CreateFileA\n";
    counterMap.at("CreateFile")->incrementCall();

    return origCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HANDLE WINAPI hookCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    if (PRINT_CALLS)
        std::cout << "Called CreateFileW\n";
    counterMap.at("CreateFile")->incrementCall();

    return origCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}
