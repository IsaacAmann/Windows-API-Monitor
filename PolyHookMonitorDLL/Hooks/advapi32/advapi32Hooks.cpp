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

LSTATUS(__stdcall* origRegCreateKeyExA)(
	HKEY hKey,
	LPCSTR lpSubKey,
	DWORD Reserved,
	LPSTR lpClass,
	DWORD dwOptions,
	REGSAM samDesired,
	const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	PHKEY phkResult,
	LPDWORD lpdwDisposition) = RegCreateKeyExA;

LSTATUS(__stdcall* origRegCreateKeyExW)(
	HKEY hKey,
	LPCWSTR lpSubKey,
	DWORD Reserved,
	LPWSTR lpClass,
	DWORD dwOptions,
	REGSAM samDesired,
	const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	PHKEY phkResult,
	LPDWORD lpdwDisposition
	) = RegCreateKeyExW;

LSTATUS(__stdcall* origRegOpenKeyExA)(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult) = RegOpenKeyExA;
LSTATUS(__stdcall* origRegOpenKeyExW)(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult) = RegOpenKeyExW;

LSTATUS(__stdcall* origRegSetValueExA)(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE* lpData, DWORD cbData) = RegSetValueExA;
LSTATUS(__stdcall* origRegSetValueExW)(HKEY hKey, LPCWSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE* lpData, DWORD cbData) = RegSetValueExW;

LSTATUS(__stdcall* origRegDeleteKeyExA)(HKEY hKey, LPCSTR lpSubKey, REGSAM samDesired, DWORD Reserved) = RegDeleteKeyExA;
LSTATUS(__stdcall* origRegDeleteKeyExW)(HKEY hKey, LPCWSTR lpSubKey, REGSAM samDesired, DWORD Reserved) = RegDeleteKeyExW;

LSTATUS(__stdcall* origRegGetValueA)(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData) = RegGetValueA;
LSTATUS(__stdcall* origRegGetValueW)(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData) = RegGetValueW;




void hookadvapi32APICalls(std::unordered_map<std::string, APICallCounter*>* hooks, CallCountContainer* callCountContainer)
{

	APICallCounter* currentCounter = NULL;

	
	//Hooking using Microsoft Detours library
	DetourTransactionBegin();
	DetourRestoreAfterWith();

	currentCounter = new APICallCounter("OpenSCManager", hooks, &(callCountContainer->cOpenSCManager));
	DetourAttach(&origOpenSCManagerA, hookOpenSCManagerA);
	DetourAttach(&origOpenSCManagerW, hookOpenSCManagerW);

	currentCounter = new APICallCounter("CreateService", hooks, &(callCountContainer->cCreateService));
	DetourAttach(&origCreateServiceA, hookCreateServiceA);
	DetourAttach(&origCreateServiceW, hookCreateServiceW);

	currentCounter = new APICallCounter("StartServiceCtrlDispatcher", hooks, &(callCountContainer->cStartServiceCtrlDispatcher));
	DetourAttach(&origStartServiceCtrlDispatcherA, hookStartServiceCtrlDispatcherA);
	DetourAttach(&origStartServiceCtrlDispatcherW, hookStartServiceCtrlDispatcherW);

	currentCounter = new APICallCounter("RegCreateKey", hooks, &(callCountContainer->cRegCreateKey));
	DetourAttach(&origRegCreateKeyExA, hookRegCreateKeyExA);
	DetourAttach(&origRegCreateKeyExW, hookRegCreateKeyExW);

	currentCounter = new APICallCounter("RegOpenKey", hooks, &(callCountContainer->cRegOpenKey));
	DetourAttach(&origRegOpenKeyExA, hookRegOpenKeyExA);
	DetourAttach(&origRegOpenKeyExW, hookRegOpenKeyExW);

	currentCounter = new APICallCounter("RegSetValue", hooks, &(callCountContainer->cRegSetValue));
	DetourAttach(&origRegSetValueExA, hookRegSetValueExA);
	DetourAttach(&origRegSetValueExW, hookRegSetValueExW);

	currentCounter = new APICallCounter("RegDeleteKey", hooks, &(callCountContainer->cRegDeleteKey));
	DetourAttach(&origRegDeleteKeyExA, hookRegDeleteKeyExA);
	DetourAttach(&origRegDeleteKeyExW, hookRegDeleteKeyExW);

	currentCounter = new APICallCounter("RegGetValue", hooks, &(callCountContainer->cRegGetValue));
	DetourAttach(&origRegGetValueA, hookRegGetValueA);
	DetourAttach(&origRegGetValueW, hookRegGetValueW);


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

LSTATUS __stdcall hookRegCreateKeyExA(
	HKEY hKey,
	LPCSTR lpSubKey,
	DWORD Reserved,
	LPSTR lpClass,
	DWORD dwOptions,
	REGSAM samDesired,
	const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	PHKEY phkResult,
	LPDWORD lpdwDisposition
)
{
	if (PRINT_CALLS)
		std::cout << "Called RegCreateKeyExA\n";
	counterMap.at("RegCreateKey")->incrementCall();

	return origRegCreateKeyExA(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
}

LSTATUS __stdcall hookRegCreateKeyExW(
	HKEY hKey,
	LPCWSTR lpSubKey,
	DWORD Reserved,
	LPWSTR lpClass,
	DWORD dwOptions,
	REGSAM samDesired,
	const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	PHKEY phkResult,
	LPDWORD lpdwDisposition
)
{
	if (PRINT_CALLS)
		std::cout << "Called RegCreateKeyExW\n";
	counterMap.at("RegCreateKey")->incrementCall();

	return origRegCreateKeyExW(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
}

LSTATUS __stdcall hookRegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
{
	if (PRINT_CALLS)
		std::cout << "Called RegOpenKeyExA\n";
	counterMap.at("RegOpenKey")->incrementCall();

	return origRegOpenKeyExA(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}

LSTATUS __stdcall hookRegOpenKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
{
	if (PRINT_CALLS)
		std::cout << "Called RegOpenKeyExW\n";
	counterMap.at("RegOpenKey")->incrementCall();

	return origRegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}

LSTATUS __stdcall hookRegSetValueExA(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE* lpData, DWORD cbData)
{
	if (PRINT_CALLS)
		std::cout << "Called RegSetValueExA\n";
	counterMap.at("RegSetValue")->incrementCall();

	return origRegSetValueExA(hKey, lpValueName, Reserved, dwType, lpData, cbData);
}

LSTATUS __stdcall hookRegSetValueExW(HKEY hKey, LPCWSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE* lpData, DWORD cbData)
{
	if (PRINT_CALLS)
		std::cout << "Called RegSetValueExW\n";
	counterMap.at("RegSetValue")->incrementCall();

	return origRegSetValueExW(hKey, lpValueName, Reserved, dwType, lpData, cbData);
}

LSTATUS __stdcall hookRegDeleteKeyExA(HKEY hKey, LPCSTR lpSubKey, REGSAM samDesired, DWORD Reserved)
{
	if (PRINT_CALLS)
		std::cout << "Called RegDeleteValueExA\n";
	counterMap.at("RegDeleteKey")->incrementCall();

	return origRegDeleteKeyExA(hKey, lpSubKey, samDesired, Reserved);
}

LSTATUS __stdcall hookRegDeleteKeyExW(HKEY hKey, LPCWSTR lpSubKey, REGSAM samDesired, DWORD Reserved)
{
	if (PRINT_CALLS)
		std::cout << "Called RegDeleteValueExW\n";
	counterMap.at("RegDeleteKey")->incrementCall();

	return origRegDeleteKeyExW(hKey, lpSubKey, samDesired, Reserved);
}

LSTATUS __stdcall hookRegGetValueA(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData)
{
	if (PRINT_CALLS)
		std::cout << "Called RegGetValueA\n";
	counterMap.at("RegGetValue")->incrementCall();

	return origRegGetValueA(hKey, lpSubKey, lpValue, dwFlags, pdwType, pvData, pcbData);
}

LSTATUS __stdcall hookRegGetValueW(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData)
{
	if (PRINT_CALLS)
		std::cout << "Called RegGetValueW\n";
	counterMap.at("RegGetValue")->incrementCall();

	return origRegGetValueW(hKey, lpSubKey, lpValue, dwFlags, pdwType, pvData, pcbData);
}