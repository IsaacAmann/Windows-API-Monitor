#pragma once

#include "../Hooks.h"
#include <wininet.h>

void hookwininetAPICalls(std::unordered_map<std::string, APICallCounter*>* hooks);


HINTERNET __stdcall hookInternetOpenA(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags);
HINTERNET __stdcall hookInternetOpenW(LPCWSTR lpszAgent, DWORD dwAccessType, LPCWSTR lpszProxy, LPCWSTR lpszProxyBypass, DWORD dwFlags);