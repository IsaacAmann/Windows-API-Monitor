#include "User32Hooks.h"

const bool PRINT_CALLS = true;

//Function pointers to original Win32 API functions
BOOL(WINAPI* origRegisterHotKey)(HWND hWnd, int id, UINT fsModifiers, UINT vk) = RegisterHotKey;

void hookUser32APICalls(std::unordered_map<std::string, APICallCounter*>* hooks)
{

	APICallCounter* currentCounter = NULL;


	//Hooking using Microsoft Detours library
	DetourTransactionBegin();
	DetourRestoreAfterWith();

	currentCounter = new APICallCounter("RegisterHotKey", hooks);
	DetourAttach(&origRegisterHotKey, hookRegisterHotKey);

	DetourTransactionCommit();
}

BOOL WINAPI hookRegisterHotKey(HWND hWnd, int id, UINT fsModifiers, UINT vk)
{
	if (PRINT_CALLS)
		std::cout << "Called RegisterHotKey\n";
	counterMap.at("RegisterHotKey")->incrementCall();

	return origRegisterHotKey(hWnd, id, fsModifiers, vk);
}
