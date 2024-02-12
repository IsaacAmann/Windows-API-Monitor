#pragma once
#include "../Hooks.h"


void hookKernel32APICalls(std::unordered_map<std::string, APICallCounter *> * hooks);

DWORD WINAPI hookGetCurrentProcessId();

HANDLE WINAPI hookOpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);

BOOL WINAPI hookWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);

BOOL WINAPI hookWriteFileEx(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionroutine);

HANDLE WINAPI hookCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

HANDLE WINAPI hookCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

HANDLE WINAPI hookCreateFileMappingA(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName);

UINT WINAPI hookGetWindowsDirectoryA(LPSTR lpBuffer, UINT uSize);

UINT WINAPI hookGetWindowsDirectoryW(LPWSTR lpBuffer, UINT uSize);

BOOL WINAPI hookSetFileTime(HANDLE hFile, const FILETIME* lpCreationTime, const FILETIME* lpLastAccessTime, const FILETIME* lpLastWriteTime);

LPVOID WINAPI hookVirtualAlloc(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
LPVOID WINAPI hookVirtualAllocEx(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);

BOOL WINAPI hookVirtualProtect(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);

BOOL WINAPI hookReadProcessMemory(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesRead);

BOOL WINAPI hookWriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten);

HANDLE WINAPI hookCreateRemoteThread(HANDLE hProcess, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);

DWORD WINAPI hookQueueUserAPC(PAPCFUNC pfnAPC, HANDLE hThread, ULONG_PTR dwData);

BOOL WINAPI hookConnectNamedPipe(HANDLE hNamedPipe, LPOVERLAPPED lpOverlapped);

HANDLE WINAPI hookCreateNamedPipeA(LPCSTR lpName, DWORD dwOpenMode, DWORD dwPipeMode, DWORD nMaxInstances, DWORD nOutBufferSize, DWORD nInBufferSize, DWORD nDefaultTimeOut, LPSECURITY_ATTRIBUTES lpSecurityAttributes);

BOOL WINAPI hookEnumProcesses(DWORD* lpidProcess, DWORD cb, LPDWORD lpcbNeeded);

BOOL WINAPI hookEnumProcessModules(HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded);

FARPROC WINAPI hookGetProcAddress(HMODULE hModule, LPCSTR lpProcName);

DWORD WINAPI hookGetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize);
DWORD WINAPI hookGetModuleFileNameW(HMODULE hModule, LPWSTR lpFilename, DWORD nSize);

HMODULE WINAPI hookGetModuleHandleA(LPCSTR lpModuleName);
HMODULE WINAPI hookGetModuleHandleW(LPCWSTR lpModuleName);

BOOL WINAPI hookPeekNamedPipe(HANDLE hNamedPipe, LPVOID lpBuffer, DWORD nBufferSize, LPDWORD lpBytesRead, LPDWORD lpTotalBytesAvail, LPDWORD lpBytesLeftThisMessage);

BOOL WINAPI hookTerminateProcess(HANDLE hProcess, UINT ExitCode);

BOOL WINAPI hookCreateProcessA(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
BOOL WINAPI hookCreateProcessW(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);