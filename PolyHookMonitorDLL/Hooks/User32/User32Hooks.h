#pragma once


#include "../Hooks.h"



void hookUser32APICalls(std::unordered_map<std::string, APICallCounter*>* hooks, CallCountContainer* callCountContainer);


BOOL WINAPI hookRegisterHotKey(HWND hWnd, int id, UINT fsModifiers, UINT vk);

SHORT WINAPI hookGetAsyncKeyState(int vKey);

HHOOK WINAPI hookSetWindowsHookExA(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId);
HHOOK WINAPI hookSetWindowsHookExW(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId);

HWND WINAPI hookGetForegroundWindow();

HDC WINAPI hookGetDC(HWND hWnd);

SHORT hookGetKeyState(int nVirtKey);

UINT hookMapVirtualKeyA(UINT uCode, UINT uMapType);
UINT hookMapVirtualKeyW(UINT uCode, UINT uMapType);