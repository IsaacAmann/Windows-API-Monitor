#include "advapi32Hooks.h"

const bool PRINT_CALLS = true;

//Function pointers to original Win32 API functions
SC_HANDLE(__stdcall* origOpenSCManagerA)(LPCSTR lpMachineName, LPCSTR lpDatabaseName, DWORD dwDesiredAccess) = OpenSCManagerA;
SC_HANDLE(__stdcall* origOpenSCManagerW)(LPCWSTR lpMachineName, LPCWSTR lpDatabaseName, DWORD dwDesiredAccess) = OpenSCManagerW;

SC_HANDLE(__stdcall* origCreateServiceA)(
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
	LPCSTR lpPassword) = CreateServiceA;
SC_HANDLE(__stdcall* origCreateServiceW)(
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
	LPCWSTR lpPassword) = CreateServiceW;

BOOL(__stdcall* origStartServiceCtrlDispatcherA)(const SERVICE_TABLE_ENTRYA* lpServiceStartTable) = StartServiceCtrlDispatcherA;
BOOL(__stdcall* origStartServiceCtrlDispatcherW)(const SERVICE_TABLE_ENTRYW* lpServiceStartTable) = StartServiceCtrlDispatcherW;


void hookadvapi32APICalls(std::unordered_map<std::string, APICallCounter*>* hooks)
{

	APICallCounter* currentCounter = NULL;


	//Hooking using Microsoft Detours library
	DetourTransactionBegin();
	DetourRestoreAfterWith();

	currentCounter = new APICallCounter("OpenSCManager", hooks);
	DetourAttach(&origOpenSCManagerA, hookOpenSCManagerA);
	DetourAttach(&origOpenSCManagerW, hookOpenSCManagerW);

	currentCounter = new APICallCounter("CreateService", hooks);
	DetourAttach(&origCreateServiceA, hookCreateServiceA);
	DetourAttach(&origCreateServiceW, hookCreateServiceW);

	currentCounter = new APICallCounter("StartServiceCtrlDispatcher", hooks);
	DetourAttach(&origStartServiceCtrlDispatcherA, hookStartServiceCtrlDispatcherA);
	DetourAttach(&origStartServiceCtrlDispatcherW, hookStartServiceCtrlDispatcherW);

	DetourTransactionCommit();
}

SC_HANDLE __stdcall hookOpenSCManagerA(LPCSTR lpMachineName, LPCSTR lpDatabaseName, DWORD dwDesiredAccess)
{
	if (PRINT_CALLS)
		std::cout << "OpenScManagerA Called\n";
	counterMap.at("OpenSCManager")->incrementCall();

	return origOpenSCManagerA(lpMachineName, lpDatabaseName, dwDesiredAccess);
}

SC_HANDLE __stdcall hookOpenSCManagerW(LPCWSTR lpMachineName, LPCWSTR lpDatabaseName, DWORD dwDesiredAccess)
{
	if (PRINT_CALLS)
		std::cout << "OpenScManagerW Called\n";
	counterMap.at("OpenSCManager")->incrementCall();

	return origOpenSCManagerW(lpMachineName, lpDatabaseName, dwDesiredAccess);
}

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
)
{
	if (PRINT_CALLS)
		std::cout << "Called CreateServiceA\n";
	counterMap.at("CreateService")->incrementCall();

	return origCreateServiceA(
		hSCManager, lpServiceName,
		lpDisplayName, dwDesiredAccess, 
		dwServiceType, dwStartType, 
		dwErrorControl, lpBinaryPathName,
		lpLoadOrderGroup, lpdwTagId,
		lpDependencies, lpServiceName,
		lpPassword
	);
}

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
)
{
	if (PRINT_CALLS)
		std::cout << "Called CreateServiceW\n";
	counterMap.at("CreateService")->incrementCall();

	return origCreateServiceW(
		hSCManager, lpServiceName,
		lpDisplayName, dwDesiredAccess,
		dwServiceType, dwStartType,
		dwErrorControl, lpBinaryPathName,
		lpLoadOrderGroup, lpdwTagId,
		lpDependencies, lpServiceName,
		lpPassword
	);
}

BOOL hookStartServiceCtrlDispatcherA(const SERVICE_TABLE_ENTRYA* lpServiceStartTable)
{
	if (PRINT_CALLS)
		std::cout << "Called StartServiceCtrlDispatcherA\n";
	counterMap.at("StartCtrlDispatcher")->incrementCall();

	return origStartServiceCtrlDispatcherA(lpServiceStartTable);
}

BOOL hookStartServiceCtrlDispatcherW(const SERVICE_TABLE_ENTRYW* lpServiceStartTable)
{
	if (PRINT_CALLS)
		std::cout << "Called StartServiceCtrlDispatcherW\n";
	counterMap.at("StartCtrlDispatcher")->incrementCall();

	return origStartServiceCtrlDispatcherW(lpServiceStartTable);
}