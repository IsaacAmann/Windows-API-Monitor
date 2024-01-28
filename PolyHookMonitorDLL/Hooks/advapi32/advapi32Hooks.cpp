#include "advapi32Hooks.h"

const bool PRINT_CALLS = true;

//Function pointers to original Win32 API functions
SC_HANDLE(__stdcall* origOpenSCManagerA)(LPCSTR lpMachineName, LPCSTR lpDatabaseName, DWORD dwDesiredAccess) = OpenSCManagerA;
SC_HANDLE(__stdcall* origOpenSCManagerW)(LPCWSTR lpMachineName, LPCWSTR lpDatabaseName, DWORD dwDesiredAccess) = OpenSCManagerW;

void hookadvapi32APICalls(std::unordered_map<std::string, APICallCounter*>* hooks)
{

	APICallCounter* currentCounter = NULL;


	//Hooking using Microsoft Detours library
	DetourTransactionBegin();
	DetourRestoreAfterWith();

	currentCounter = new APICallCounter("OpenSCManager", hooks);
	DetourAttach(&origOpenSCManagerA, hookOpenSCManagerA);
	DetourAttach(&origOpenSCManagerW, hookOpenSCManagerW);

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