#pragma once

void hookAPICalls();

//#include "../include/polyhook2/IHook.hpp"
//#include "../include/polyhook2/Detour/NatDetour.hpp"
//#include "../include/polyhook2/PE/IatHook.hpp"

#include "Hooks/Hooks.h"

#include "Hooks/Kernel32/Kernel32Hooks.h"
#include "Hooks/wsock32/wsock32Hooks.h"
#include "Hooks/wininet/wininetHooks.h"
#include "Hooks/advapi32/advapi32Hooks.h"
#include "Hooks/User32/User32Hooks.h"
