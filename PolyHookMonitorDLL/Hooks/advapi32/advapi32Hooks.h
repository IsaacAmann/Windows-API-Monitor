#pragma once


#include "../Hooks.h"
#include <winsvc.h>


void hookadvapi32APICalls(std::unordered_map<std::string, APICallCounter*>* hooks);

SC_HANDLE __stdcall hookOpenSCManagerA(LPCSTR lpMachineName, LPCSTR lpDatabaseName, DWORD dwDesiredAccess);
SC_HANDLE __stdcall hookOpenSCManagerW(LPCWSTR lpMachineName, LPCWSTR lpDatabaseName, DWORD dwDesiredAccess);
