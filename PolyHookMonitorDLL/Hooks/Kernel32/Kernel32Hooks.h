#pragma once

#include <Windows.h>

#include "../include/polyhook2/IHook.hpp"
#include "../include/polyhook2/Detour/NatDetour.hpp"

#include "../Hooks.h"
#include <vector>

void hookKernel32APICalls(std::vector<PLH::NatDetour *> *hooks);

DWORD __stdcall hookGetCurrentProcessId();