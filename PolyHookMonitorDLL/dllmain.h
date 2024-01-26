#pragma once

void hookAPICalls();

//#include "../include/polyhook2/IHook.hpp"
//#include "../include/polyhook2/Detour/NatDetour.hpp"
//#include "../include/polyhook2/PE/IatHook.hpp"

#include "Hooks/Hooks.h"

#include "Hooks/Kernel32/Kernel32Hooks.h"


DWORD WINAPI MessengerThreadExecute(LPVOID lpParam);
