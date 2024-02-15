#pragma once

#include "../Hooks.h"
#include <wininet.h>

void hookwininetAPICalls(std::unordered_map<std::string, APICallCounter*>* hooks, CallCountContainer* callCountContainer);


HINTERNET __stdcall hookInternetOpenA(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags);
HINTERNET __stdcall hookInternetOpenW(LPCWSTR lpszAgent, DWORD dwAccessType, LPCWSTR lpszProxy, LPCWSTR lpszProxyBypass, DWORD dwFlags);

HINTERNET __stdcall hookInternetOpenUrlA(HINTERNET hInternet, LPCSTR lpszUrl, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);
HINTERNET __stdcall hookInternetOpenUrlW(HINTERNET hInternet, LPCWSTR lpszUrl, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);

BOOL __stdcall hookInternetReadFile(HINTERNET hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead);
BOOL __stdcall hookInternetReadFileExA(HINTERNET hFile, LPINTERNET_BUFFERSA lpBuffersOut, DWORD dwFlags, DWORD_PTR dwContext);
BOOL __stdcall hookInternetReadFileExW(HINTERNET hFile, LPINTERNET_BUFFERSW lpBuffersOut, DWORD dwFlags, DWORD_PTR dwContext);

BOOL __stdcall hookInternetWriteFile(HINTERNET hFile, LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpdwNumberOfBytesWritten);
