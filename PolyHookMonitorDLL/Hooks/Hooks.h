#pragma once

#include <Windows.h>

#include "../include/polyhook2/IHook.hpp"
#include "../include/polyhook2/Detour/NatDetour.hpp"

#include <vector>

class APICallCounter
{
	public:
		APICallCounter(PLH::NatDetour * hook, std::string callName, std::unordered_map<std::string, APICallCounter *> * hooks);
		PLH::NatDetour* hook;
		std::string callName;
		int numberCalls;
	
};