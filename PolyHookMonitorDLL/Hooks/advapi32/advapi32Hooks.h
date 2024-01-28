#pragma once


#include "../Hooks.h"
#include <winsvc.h>


void hookadvapi32APICalls(std::unordered_map<std::string, APICallCounter*>* hooks);

SC_HANDLE __stdcall hookOpenSCManagerA(LPCSTR lpMachineName, LPCSTR lpDatabaseName, DWORD dwDesiredAccess);
SC_HANDLE __stdcall hookOpenSCManagerW(LPCWSTR lpMachineName, LPCWSTR lpDatabaseName, DWORD dwDesiredAccess);

SC_HANDLE __stdcall hookCreateServiceA(
	SC_HANDLE hSCManager, 
	LPCSTR lpServiceName, 
	LPCSTR lpDisplayName,
	DWORD dwDesiredAccess,
	DWORD dwServiceType,
	DWORD dwStartType,
	DWORD dwErrorControl,
	LPCSTR lpBinaryPathName,
	LPCSTR lpLoadOrderGroup,
	LPDWORD lpdwTagId,
	LPCSTR lpDependencies,
	LPCSTR lpServiceStartName,
	LPCSTR lpPassword
);

SC_HANDLE __stdcall hookCreateServiceW(
	SC_HANDLE hSCManager,
	LPCWSTR lpServiceName,
	LPCWSTR lpDisplayName,
	DWORD dwDesiredAccess,
	DWORD dwServiceType,
	DWORD dwStartType,
	DWORD dwErrorControl,
	LPCWSTR lpBinaryPathName,
	LPCWSTR lpLoadOrderGroup,
	LPDWORD lpdwTagId,
	LPCWSTR lpDependencies,
	LPCWSTR lpServiceStartName,
	LPCWSTR lpPassword
);

BOOL hookStartServiceCtrlDispatcherA(const SERVICE_TABLE_ENTRYA* lpServiceStartTable);
BOOL hookStartServiceCtrlDispatcherW(const SERVICE_TABLE_ENTRYW* lpServiceStartTable);


