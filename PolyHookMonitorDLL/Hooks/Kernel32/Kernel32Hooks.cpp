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

HANDLE(WINAPI* origCreateFileMappingA)(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName) = CreateFileMappingA;

UINT(WINAPI* origGetWindowsDirectoryA)(LPSTR lpBuffer, UINT uSize) = GetWindowsDirectoryA;
UINT(WINAPI* origGetWindowsDirectoryW)(LPWSTR lpBuffer, UINT uSize) = GetWindowsDirectoryW;

BOOL(WINAPI* origSetFileTime)(HANDLE hFile, const FILETIME* lpCreationTime, const FILETIME* lpLastAccessTime, const FILETIME* lpLastWriteTime) = SetFileTime;

LPVOID(WINAPI* origVirtualAlloc)(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect) = VirtualAlloc;

LPVOID(WINAPI* origVirtualAllocEx)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect) = VirtualAllocEx;

BOOL(WINAPI* origVirtualProtect)(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect) = VirtualProtect;

BOOL(WINAPI* origReadProcessMemory)(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesRead) = ReadProcessMemory;

BOOL(WINAPI* origWriteProcessMemory)(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten) = WriteProcessMemory;

HANDLE(WINAPI* origCreateRemoteThread)(HANDLE hProcess, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId) = CreateRemoteThread;

DWORD(WINAPI* origQueueUserAPC)(PAPCFUNC pfnAPC, HANDLE hThread, ULONG_PTR dwData) = QueueUserAPC;

BOOL(WINAPI* origConnectNamedPipe)(HANDLE hNamedPipe, LPOVERLAPPED lpOverlapped) = ConnectNamedPipe;

HANDLE(WINAPI* origCreateNamedPipeA)(LPCSTR lpName, DWORD dwOpenMode, DWORD dwPipeMode, DWORD nMaxInstances, DWORD nOutBufferSize, DWORD nInBufferSize, DWORD nDefaultTimeOut, LPSECURITY_ATTRIBUTES lpSecurityAttributes) = CreateNamedPipeA;

BOOL(WINAPI* origEnumProcesses)(DWORD* lpidProcess, DWORD cb, LPDWORD lpcbNeeded) = EnumProcesses;

BOOL(WINAPI* origEnumProcessModules)(HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded) = EnumProcessModules;

FARPROC(WINAPI* origGetProcAddress)(HMODULE hModule, LPCSTR lpProcName) = GetProcAddress;

DWORD(WINAPI* origGetModuleFileNameA)(HMODULE hModule, LPSTR lpFilename, DWORD nSize) = GetModuleFileNameA;

DWORD(WINAPI* origGetModuleFileNameW)(HMODULE hModule, LPWSTR lpFilename, DWORD nSize) = GetModuleFileNameW;

HMODULE(WINAPI* origGetModuleHandleA)(LPCSTR lpModuleName) = GetModuleHandleA;

HMODULE(WINAPI* origGetModuleHandleW)(LPCWSTR lpModuleName) = GetModuleHandleW;

BOOL(WINAPI* origPeekNamedPipe)(HANDLE hNamedPipe, LPVOID lpBuffer, DWORD nBufferSize, LPDWORD lpBytesRead, LPDWORD lpTotalBytesAvail, LPDWORD lpBytesLeftThisMessage) = PeekNamedPipe;

BOOL(WINAPI* origTerminateProcess)(HANDLE hProcess, UINT uExitCode) = TerminateProcess;

BOOL(WINAPI* origCreateProcessA)(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation) = CreateProcessA;
BOOL(WINAPI* origCreateProcessW)(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation) = CreateProcessW;

BOOL(WINAPI* origIsDebuggerPresent)() = IsDebuggerPresent;

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
    DetourTransactionBegin();
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

    currentCounter = new APICallCounter("CreateFileMapping", hooks);
    DetourAttach(&origCreateFileMappingA, hookCreateFileMappingA);

    currentCounter = new APICallCounter("GetWindowsDirectory", hooks);
    DetourAttach(&origGetWindowsDirectoryA, hookGetWindowsDirectoryA);
    DetourAttach(&origGetWindowsDirectoryW, hookGetWindowsDirectoryW);

    currentCounter = new APICallCounter("SetFileTime", hooks);
    DetourAttach(&origSetFileTime, hookSetFileTime);

    currentCounter = new APICallCounter("VirtualAlloc", hooks);
    DetourAttach(&origVirtualAlloc, hookVirtualAlloc);

    currentCounter = new APICallCounter("VirtualAllocEx", hooks);
    DetourAttach(&origVirtualAllocEx, hookVirtualAllocEx);

    currentCounter = new APICallCounter("VirtualProtect", hooks);
    DetourAttach(&origVirtualProtect, hookVirtualProtect);

    currentCounter = new APICallCounter("ReadProcessMemory", hooks);
    DetourAttach(&origReadProcessMemory, hookReadProcessMemory);

    currentCounter = new APICallCounter("WriteProcessMemory", hooks);
    DetourAttach(&origWriteProcessMemory, hookWriteProcessMemory);

    currentCounter = new APICallCounter("CreateRemoteThread", hooks);
    DetourAttach(&origCreateRemoteThread, hookCreateRemoteThread);

    currentCounter = new APICallCounter("QueueUserAPC", hooks);
    DetourAttach(&origQueueUserAPC, hookQueueUserAPC);

    currentCounter = new APICallCounter("ConnectNamedPipe", hooks);
    DetourAttach(&origConnectNamedPipe, hookConnectNamedPipe);

    currentCounter = new APICallCounter("CreateNamedPipe", hooks);
    DetourAttach(&origCreateNamedPipeA, hookCreateNamedPipeA);

    currentCounter = new APICallCounter("EnumProcesses", hooks);
    DetourAttach(&origEnumProcesses, hookEnumProcesses);

    currentCounter = new APICallCounter("EnumProcessModules", hooks);
    DetourAttach(&origEnumProcessModules, hookEnumProcessModules);

    currentCounter = new APICallCounter("GetModuleFileName", hooks);
    DetourAttach(&origGetModuleFileNameA, hookGetModuleFileNameA);
    DetourAttach(&origGetModuleFileNameW, hookGetModuleFileNameW);

    currentCounter = new APICallCounter("GetModuleHandle", hooks);
    DetourAttach(&origGetModuleHandleA, hookGetModuleHandleA);
    DetourAttach(&origGetModuleHandleW, hookGetModuleHandleW);

    currentCounter = new APICallCounter("PeekNamedPipe", hooks);
    DetourAttach(&origPeekNamedPipe, hookPeekNamedPipe);

    currentCounter = new APICallCounter("TerminateProcess", hooks);
    DetourAttach(&origTerminateProcess, hookTerminateProcess);

    currentCounter = new APICallCounter("CreateProcess", hooks);
    DetourAttach(&origCreateProcessA, hookCreateProcessA);
    DetourAttach(&origCreateProcessW, hookCreateProcessW);

    currentCounter = new APICallCounter("IsDebuggerPresent", hooks);
    DetourAttach(&origIsDebuggerPresent, hookIsDebuggerPresent);

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

HANDLE WINAPI hookCreateFileMappingA(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName)
{
    if (PRINT_CALLS)
        std::cout << "Called CreateFileMappingA\n";
    counterMap.at("CreateFileMapping")->incrementCall();

    return origCreateFileMappingA(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
}

UINT WINAPI hookGetWindowsDirectoryA(LPSTR lpBuffer, UINT uSize)
{
    if (PRINT_CALLS)
        std::cout << "Called GetWindowsDirectoryA\n";
    counterMap.at("GetWindowsDirectory")->incrementCall();

    return origGetWindowsDirectoryA(lpBuffer, uSize);
}

UINT WINAPI hookGetWindowsDirectoryW(LPWSTR lpBuffer, UINT uSize)
{
    if (PRINT_CALLS)
        std::cout << "Called GetWindowsDirectoryW\n";
    counterMap.at("GetWindowsDirectory")->incrementCall();

    return origGetWindowsDirectoryW(lpBuffer, uSize);
}

BOOL WINAPI hookSetFileTime(HANDLE hFile, const FILETIME* lpCreationTime, const FILETIME* lpLastAccessTime, const FILETIME* lpLastWriteTime)
{
    if (PRINT_CALLS)
        std::cout << "Called SetFileTime\n";
    counterMap.at("SetFileTime")->incrementCall();
    
    return origSetFileTime(hFile, lpCreationTime, lpLastAccessTime, lpLastWriteTime);
}

LPVOID WINAPI hookVirtualAlloc(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect)
{
    if (PRINT_CALLS)
        std::cout << "Called VirtualAlloc\n";
    counterMap.at("VirtualAlloc")->incrementCall();

    return origVirtualAlloc(lpAddress, dwSize, flAllocationType, flProtect);
}

LPVOID WINAPI hookVirtualAllocEx(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect)
{
    if (PRINT_CALLS)
        std::cout << "Called VirtualAllocEx\n";
    counterMap.at("VirtualAllocEx")->incrementCall();

    return origVirtualAllocEx(hProcess, lpAddress, dwSize, flAllocationType, flProtect);
}

BOOL WINAPI hookVirtualProtect(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect)
{
    if (PRINT_CALLS)
        std::cout << "Called VirtualProtect\n";
    counterMap.at("VirtualProtect")->incrementCall();

    return origVirtualProtect(lpAddress, dwSize, flNewProtect, lpflOldProtect);
}

BOOL WINAPI hookReadProcessMemory(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesRead)
{
    if (PRINT_CALLS)
        std::cout << "Called ReadProcessMemory\n";
    counterMap.at("ReadProcessMemory")->incrementCall();

    return origReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
}

BOOL WINAPI hookWriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten)
{
    if (PRINT_CALLS)
        std::cout << "Called WriteProcessMemory\n";
    counterMap.at("WriteProcessMemory")->incrementCall();

    return origWriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
}

HANDLE WINAPI hookCreateRemoteThread(HANDLE hProcess, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId)
{
    if (PRINT_CALLS)
        std::cout << "Called CreateRemoteThread\n";
    counterMap.at("CreateRemoteThread")->incrementCall();

    return origCreateRemoteThread(hProcess, lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
}

DWORD WINAPI hookQueueUserAPC(PAPCFUNC pfnAPC, HANDLE hThread, ULONG_PTR dwData)
{
    if (PRINT_CALLS)
        std::cout << "Called QueueUserAPC\n";
    counterMap.at("QueueUserAPC")->incrementCall();

    return origQueueUserAPC(pfnAPC, hThread, dwData);
}

BOOL WINAPI hookConnectNamedPipe(HANDLE hNamedPipe, LPOVERLAPPED lpOverlapped)
{
    if (PRINT_CALLS)
        std::cout << "Called ConnectNamedPipe\n";
    counterMap.at("ConnectNamedPipe")->incrementCall();

    return origConnectNamedPipe(hNamedPipe, lpOverlapped);
}

HANDLE WINAPI hookCreateNamedPipeA(LPCSTR lpName, DWORD dwOpenMode, DWORD dwPipeMode, DWORD nMaxInstances, DWORD nOutBufferSize, DWORD nInBufferSize, DWORD nDefaultTimeOut, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
    if (PRINT_CALLS)
        std::cout << "Called CreateNamedPipeA\n";
    counterMap.at("CreateNamedPipe")->incrementCall();

    return origCreateNamedPipeA(lpName, dwOpenMode, dwPipeMode, nMaxInstances, nOutBufferSize, nInBufferSize, nDefaultTimeOut, lpSecurityAttributes);
}

BOOL WINAPI hookEnumProcesses(DWORD* lpidProcess, DWORD cb, LPDWORD lpcbNeeded)
{
    if (PRINT_CALLS)
        std::cout << "Called EnumProcesses\n";
    counterMap.at("EnumProcesses")->incrementCall();

    return origEnumProcesses(lpidProcess, cb, lpcbNeeded);
}

BOOL WINAPI hookEnumProcessModules(HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded)
{
    if (PRINT_CALLS)
        std::cout << "Called EnumProcessModules\n";
    counterMap.at("EnumProcessModules")->incrementCall();

    return origEnumProcessModules(hProcess, lphModule, cb, lpcbNeeded);
}

FARPROC WINAPI hookGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
    if (PRINT_CALLS)
        std::cout << "Called GetProcAddress\n";
    counterMap.at("GetProcAddress")->incrementCall();

    return origGetProcAddress(hModule, lpProcName);
}

DWORD WINAPI hookGetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize)
{
    if (PRINT_CALLS)
        std::cout << "Called GetModuleFileNameA\n";
    counterMap.at("GetModuleFileName")->incrementCall();

    return origGetModuleFileNameA(hModule, lpFilename, nSize);
}

DWORD WINAPI hookGetModuleFileNameW(HMODULE hModule, LPWSTR lpFilename, DWORD nSize)
{
    if (PRINT_CALLS)
        std::cout << "Called GetModuleFileNameW\n";
    counterMap.at("GetModuleFileName")->incrementCall();

    return origGetModuleFileNameW(hModule, lpFilename, nSize);
}

HMODULE WINAPI hookGetModuleHandleA(LPCSTR lpModuleName)
{
    if (PRINT_CALLS)
        std::cout << "Called GetModuleHandleA\n";
    counterMap.at("GetModuleHandle")->incrementCall();

    return origGetModuleHandleA(lpModuleName);
}

HMODULE WINAPI hookGetModuleHandleW(LPCWSTR lpModuleName)
{
    if (PRINT_CALLS)
        std::cout << "Called GetModuleHandleW\n";
    counterMap.at("GetModuleHandle")->incrementCall();

    return origGetModuleHandleW(lpModuleName);
}

BOOL WINAPI hookPeekNamedPipe(HANDLE hNamedPipe, LPVOID lpBuffer, DWORD nBufferSize, LPDWORD lpBytesRead, LPDWORD lpTotalBytesAvail, LPDWORD lpBytesLeftThisMessage)
{
    if (PRINT_CALLS)
        std::cout << "Called PeekNamedPipe\n";
    counterMap.at("PeekNamedPipe")->incrementCall();

    return origPeekNamedPipe(hNamedPipe, lpBuffer, nBufferSize, lpBytesRead, lpTotalBytesAvail, lpBytesLeftThisMessage);
}

BOOL WINAPI hookTerminateProcess(HANDLE hProcess, UINT ExitCode)
{
    if (PRINT_CALLS)
        std::cout << "Called TerminateProcess\n";
    counterMap.at("TerminateProcess")->incrementCall();

    return origTerminateProcess(hProcess, ExitCode);
}

BOOL WINAPI hookCreateProcessA(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
    if (PRINT_CALLS)
        std::cout << "Called CreateProcessA\n";
    counterMap.at("CreateProcess")->incrementCall();

    return origCreateProcessA(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
}
BOOL WINAPI hookCreateProcessW(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
    if (PRINT_CALLS)
        std::cout << "Called CreateProcessW\n";
    counterMap.at("CreateProcess")->incrementCall();

    return origCreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
}

BOOL WINAPI hookIsDebuggerPresent()
{
    if (PRINT_CALLS)
        std::cout << "Called IsDebuggerPresent\n";
    counterMap.at("IsDebuggerPresent")->incrementCall();

    return origIsDebuggerPresent();
}








