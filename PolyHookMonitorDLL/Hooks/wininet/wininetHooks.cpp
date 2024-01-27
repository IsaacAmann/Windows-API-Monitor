#include "wininetHooks.h"

const bool PRINT_CALLS = true;

//Function pointers to original Win32 API functions
HINTERNET(__stdcall* origInternetOpenA)(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags) = InternetOpenA;
HINTERNET(__stdcall* origInternetOpenW)(LPCWSTR lpszAgent, DWORD dwAccessType, LPCWSTR lpszProxy, LPCWSTR lpszProxyBypass, DWORD dwFlags) = InternetOpenW;

void hookwininetAPICalls(std::unordered_map<std::string, APICallCounter*>* hooks)
{

    APICallCounter* currentCounter = NULL;


    //Hooking using Microsoft Detours library
    DetourTransactionBegin();
	DetourRestoreAfterWith();

    currentCounter = new APICallCounter("InternetOpen", hooks);
    DetourAttach(&origInternetOpenA, hookInternetOpenA);
	DetourAttach(&origInternetOpenW, hookInternetOpenW);

    

    DetourTransactionCommit();
}

HINTERNET hookInternetOpenA(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags)
{
	if (PRINT_CALLS)
		std::cout << "Called InternetOpenA\n";
	counterMap.at("InternetOpen")->incrementCall();

	return origInternetOpenA(lpszAgent, dwAccessType, lpszAgent, lpszProxyBypass, dwFlags);
}

HINTERNET hookInternetOpenW(LPCWSTR lpszAgent, DWORD dwAccessType, LPCWSTR lpszProxy, LPCWSTR lpszProxyBypass, DWORD dwFlags)
{
	if (PRINT_CALLS)
		std::cout << "Called InternetOpenW\n";
	counterMap.at("InternetOpen")->incrementCall();

	return origInternetOpenW(lpszAgent, dwAccessType, lpszProxy, lpszProxy, dwFlags);
}