#pragma once

void hookAPICalls();

#include "../include/polyhook2/IHook.hpp"
#include "../include/polyhook2/Detour/NatDetour.hpp"

#include "Hooks/Hooks.h"

#include "Hooks/Kernel32/Kernel32Hooks.h"