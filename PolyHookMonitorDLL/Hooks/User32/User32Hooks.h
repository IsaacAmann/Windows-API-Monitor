#pragma once


#include "../Hooks.h"



void hookUser32APICalls(std::unordered_map<std::string, APICallCounter*>* hooks);


BOOL WINAPI hookRegisterHotKey(HWND hWnd, int id, UINT fsModifiers, UINT vk);
