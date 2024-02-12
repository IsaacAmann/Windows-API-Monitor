#include "User32Hooks.h"

const bool PRINT_CALLS = true;

//Function pointers to original Win32 API functions
BOOL(WINAPI* origRegisterHotKey)(HWND hWnd, int id, UINT fsModifiers, UINT vk) = RegisterHotKey;

SHORT(WINAPI* origGetAsyncKeyState)(int vKey) = GetAsyncKeyState;

HHOOK(WINAPI* origSetWindowsHookExA)(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId) = SetWindowsHookExA;
HHOOK(WINAPI* origSetWindowsHookExW)(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId) = SetWindowsHookExW;

HWND(WINAPI* origGetForegroundWindow)() = GetForegroundWindow;

void hookUser32APICalls(std::unordered_map<std::string, APICallCounter*>* hooks)
{

	APICallCounter* currentCounter = NULL;


	//Hooking using Microsoft Detours library
	DetourTransactionBegin();
	DetourRestoreAfterWith();

	currentCounter = new APICallCounter("RegisterHotKey", hooks);
	DetourAttach(&origRegisterHotKey, hookRegisterHotKey);

	currentCounter = new APICallCounter("GetAsyncKeyState", hooks);
	DetourAttach(&origGetAsyncKeyState, hookGetAsyncKeyState);

	currentCounter = new APICallCounter("SetWindowsHookEx", hooks);
	DetourAttach(&origSetWindowsHookExA, hookSetWindowsHookExA);
	DetourAttach(&origSetWindowsHookExW, hookSetWindowsHookExW);

	currentCounter = new APICallCounter("GetForegroundWindow", hooks);
	DetourAttach(&origGetForegroundWindow, hookGetForegroundWindow);

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