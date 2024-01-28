#include "wininetHooks.h"

const bool PRINT_CALLS = true;

//Function pointers to original Win32 API functions
HINTERNET(__stdcall* origInternetOpenA)(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags) = InternetOpenA;
HINTERNET(__stdcall* origInternetOpenW)(LPCWSTR lpszAgent, DWORD dwAccessType, LPCWSTR lpszProxy, LPCWSTR lpszProxyBypass, DWORD dwFlags) = InternetOpenW;

HINTERNET(__stdcall* origInternetOpenUrlA)(HINTERNET hInternet, LPCSTR lpszUrl, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext) = InternetOpenUrlA;
HINTERNET(__stdcall* origInternetOpenUrlW)(HINTERNET hInternet, LPCWSTR lpszUrl, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext) = InternetOpenUrlW;

BOOL(__stdcall* origInternetReadFile)(HINTERNET hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead) = InternetReadFile;
BOOL(__stdcall* origInternetReadFileExA)(HINTERNET hFile, LPINTERNET_BUFFERSA lpBuffersOut, DWORD dwFlags, DWORD_PTR dwContext) = InternetReadFileExA;
BOOL(__stdcall* origInternetReadFileExW)(HINTERNET hFile, LPINTERNET_BUFFERSW lpBuffersOut, DWORD dwFlags, DWORD_PTR dwContext) = InternetReadFileExW;

BOOL(__stdcall* origInternetWriteFile)(HINTERNET hFile, LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpdwNumberOfBytesWritten) = InternetWriteFile;

void hookwininetAPICalls(std::unordered_map<std::string, APICallCounter*>* hooks)
{

    APICallCounter* currentCounter = NULL;


    //Hooking using Microsoft Detours library
    DetourTransactionBegin();
	DetourRestoreAfterWith();

    currentCounter = new APICallCounter("InternetOpen", hooks);
    DetourAttach(&origInternetOpenA, hookInternetOpenA);
	DetourAttach(&origInternetOpenW, hookInternetOpenW);

	currentCounter = new APICallCounter("InternetOpenUrl", hooks);
	DetourAttach(&origInternetOpenUrlA, hookInternetOpenUrlA);
	DetourAttach(&origInternetOpenUrlW, hookInternetOpenUrlW);

	currentCounter = new APICallCounter("InternetReadFile", hooks);
	DetourAttach(&origInternetReadFile, hookInternetReadFile);
	DetourAttach(&origInternetReadFileExA, hookInternetReadFileExA);
	DetourAttach(&origInternetReadFileExW, hookInternetReadFileExW);

	currentCounter = new APICallCounter("InternetWriteFile", hooks);
	DetourAttach(&origInternetWriteFile, hookInternetWriteFile);

    DetourTransactionCommit();
}

HINTERNET __stdcall hookInternetOpenA(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags)
{
	if (PRINT_CALLS)
		std::cout << "Called InternetOpenA\n";
	counterMap.at("InternetOpen")->incrementCall();

	return origInternetOpenA(lpszAgent, dwAccessType, lpszAgent, lpszProxyBypass, dwFlags);
}

HINTERNET __stdcall hookInternetOpenW(LPCWSTR lpszAgent, DWORD dwAccessType, LPCWSTR lpszProxy, LPCWSTR lpszProxyBypass, DWORD dwFlags)
{
	if (PRINT_CALLS)
		std::cout << "Called InternetOpenW\n";
	counterMap.at("InternetOpen")->incrementCall();

	return origInternetOpenW(lpszAgent, dwAccessType, lpszProxy, lpszProxy, dwFlags);
}

HINTERNET __stdcall hookInternetOpenUrlA(HINTERNET hInternet, LPCSTR lpszUrl, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext)
{
	if (PRINT_CALLS)
		std::cout << "Called InternetOpenUrlA\n";
	counterMap.at("InternetOpenUrl")->incrementCall();

	return origInternetOpenUrlA(hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
}

HINTERNET __stdcall hookInternetOpenUrlW(HINTERNET hInternet, LPCWSTR lpszUrl, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext)
{
	if (PRINT_CALLS)
		std::cout << "Called InternetOpenUrlW\n";
	counterMap.at("InternetOpenUrl")->incrementCall();

	return origInternetOpenUrlW(hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
}

BOOL __stdcall hookInternetReadFile(HINTERNET hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead)
{
	if (PRINT_CALLS)
		std::cout << "Called InternetReadFile\n";
	counterMap.at("InternetReadFile")->incrementCall();

	return origInternetReadFile(hFile, lpBuffer, dwNumberOfBytesToRead, lpdwNumberOfBytesRead);
}

BOOL __stdcall hookInternetReadFileExA(HINTERNET hFile, LPINTERNET_BUFFERSA lpBuffersOut, DWORD dwFlags, DWORD_PTR dwContext)
{
	if (PRINT_CALLS)
		std::cout << "Called InternetReadFileExA\n";
	counterMap.at("InternetReadFile")->incrementCall();

	return origInternetReadFileExA(hFile, lpBuffersOut, dwFlags, dwContext);
}

BOOL __stdcall hookInternetReadFileExW(HINTERNET hFile, LPINTERNET_BUFFERSW lpBuffersOut, DWORD dwFlags, DWORD_PTR dwContext)
{
	if (PRINT_CALLS)
		std::cout << "Called InternetReadFileExW\n";
	counterMap.at("InternetReadFile")->incrementCall();

	return origInternetReadFileExW(hFile, lpBuffersOut, dwFlags, dwContext);
}

BOOL __stdcall hookInternetWriteFile(HINTERNET hFile, LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpdwNumberOfBytesWritten)
{
	if (PRINT_CALLS)
		std::cout << "Called InternetWriteFile\n";
	counterMap.at("InternetWriteFile")->incrementCall();

	return origInternetWriteFile(hFile, lpBuffer, dwNumberOfBytesToWrite, lpdwNumberOfBytesWritten);
}
