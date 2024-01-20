#pragma once
#include "../Hooks.h"


void hookKernel32APICalls(std::unordered_map<std::string, APICallCounter *> * hooks);

DWORD WINAPI hookGetCurrentProcessId();

HANDLE __stdcall hookOpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);