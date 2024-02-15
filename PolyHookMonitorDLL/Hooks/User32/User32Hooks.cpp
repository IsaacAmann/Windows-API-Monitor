#include "User32Hooks.h"

const bool PRINT_CALLS = true;

//Function pointers to original Win32 API functions
BOOL(WINAPI* origRegisterHotKey)(HWND hWnd, int id, UINT fsModifiers, UINT vk) = RegisterHotKey;

SHORT(WINAPI* origGetAsyncKeyState)(int vKey) = GetAsyncKeyState;

HHOOK(WINAPI* origSetWindowsHookExA)(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId) = SetWindowsHookExA;
HHOOK(WINAPI* origSetWindowsHookExW)(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId) = SetWindowsHookExW;

HWND(WINAPI* origGetForegroundWindow)() = GetForegroundWindow;

HDC(WINAPI* origGetDC)(HWND hWnd) = GetDC;

SHORT(WINAPI* origGetKeyState)(int nVirtKey) = GetKeyState;

UINT(WINAPI* origMapVirtualKeyA)(UINT uCode, UINT uMapType) = MapVirtualKeyA;
UINT(WINAPI* origMapVirtualKeyW)(UINT uCode, UINT uMapType) = MapVirtualKeyW;


void hookUser32APICalls(std::unordered_map<std::string, APICallCounter*>* hooks, CallCountContainer* callCountContainer)
{

	APICallCounter* currentCounter = NULL;

	//Hooking using Microsoft Detours library
	DetourTransactionBegin();
	DetourRestoreAfterWith();

	currentCounter = new APICallCounter("RegisterHotKey", hooks, &(callCountContainer->cRegisterHotKey));
	DetourAttach(&origRegisterHotKey, hookRegisterHotKey);

	currentCounter = new APICallCounter("GetAsyncKeyState", hooks, &(callCountContainer->cGetAsyncKeyState));
	DetourAttach(&origGetAsyncKeyState, hookGetAsyncKeyState);

	currentCounter = new APICallCounter("SetWindowsHookEx", hooks, &(callCountContainer->cSetWindowsHookEx));
	DetourAttach(&origSetWindowsHookExA, hookSetWindowsHookExA);
	DetourAttach(&origSetWindowsHookExW, hookSetWindowsHookExW);

	currentCounter = new APICallCounter("GetForegroundWindow", hooks, &(callCountContainer->cGetForegroundWindow));
	DetourAttach(&origGetForegroundWindow, hookGetForegroundWindow);

	currentCounter = new APICallCounter("GetDC", hooks, &(callCountContainer->cGetDC));
	DetourAttach(&origGetDC, hookGetDC);

	currentCounter = new APICallCounter("GetKeyState", hooks, &(callCountContainer->cGetKeyState));
	DetourAttach(&origGetKeyState, hookGetKeyState);

	currentCounter = new APICallCounter("MapVirtualKey", hooks, &(callCountContainer->cMapVirtualKey));
	DetourAttach(&origMapVirtualKeyA, hookMapVirtualKeyA);
	DetourAttach(&origMapVirtualKeyW, hookMapVirtualKeyW);

	DetourTransactionCommit();
}

BOOL WINAPI hookRegisterHotKey(HWND hWnd, int id, UINT fsModifiers, UINT vk)
{
	if (PRINT_CALLS)
		std::cout << "Called RegisterHotKey\n";
	counterMap.at("RegisterHotKey")->incrementCall();

	return origRegisterHotKey(hWnd, id, fsModifiers, vk);
}

SHORT WINAPI hookGetAsyncKeyState(int vKey)
{
	if (PRINT_CALLS)
		std::cout << "Called GetAsyncKeyState\n";
	counterMap.at("GetAsyncKeyState")->incrementCall();

	return origGetAsyncKeyState(vKey);
}

HHOOK WINAPI hookSetWindowsHookExA(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId)
{
	if (PRINT_CALLS)
		std::cout << "Called SetWindowsHookExA\n";
	counterMap.at("SetWindowsHookEx")->incrementCall();

	return origSetWindowsHookExA(idHook, lpfn, hmod, dwThreadId);
}

HHOOK WINAPI hookSetWindowsHookExW(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId)
{
	if (PRINT_CALLS)
		std::cout << "Called SetWindowsHookExW\n";
	counterMap.at("SetWindowsHookEx")->incrementCall();

	return origSetWindowsHookExW(idHook, lpfn, hmod, dwThreadId);
}

HWND WINAPI hookGetForegroundWindow()
{
	if (PRINT_CALLS)
		std::cout << "Called GetForegroundWindow\n";
	counterMap.at("GetForegroundWindow")->incrementCall();

	return origGetForegroundWindow();
}

HDC WINAPI hookGetDC(HWND hWnd)
{
	if (PRINT_CALLS)
		std::cout << "Called GetDC\n";
	counterMap.at("GetDC")->incrementCall();

	return origGetDC(hWnd);
}

SHORT hookGetKeyState(int nVirtKey)
{
	if (PRINT_CALLS)
		std::cout << "Called GetKeyState\n";
	counterMap.at("GetKeyState")->incrementCall();

	return origGetKeyState(nVirtKey);
}

UINT hookMapVirtualKeyA(UINT uCode, UINT uMapType)
{
	if (PRINT_CALLS)
		std::cout << "Called MapVirtualKeyA\n";
	counterMap.at("MapVirtualKey")->incrementCall();

	return origMapVirtualKeyA(uCode, uMapType);
}
UINT hookMapVirtualKeyW(UINT uCode, UINT uMapType)
{
	if (PRINT_CALLS)
		std::cout << "Called MapVirtualKeyW\n";
	counterMap.at("MapVirtualKey")->incrementCall();

	return origMapVirtualKeyW(uCode, uMapType);
}