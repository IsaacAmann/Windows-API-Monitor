#pragma once

#include <Windows.h>
#include "../../include/detours.h"

/*
#include "../include/polyhook2/IHook.hpp"
#include "../include/polyhook2/Detour/NatDetour.hpp"
#include "../include/polyhook2/PE/IatHook.hpp"
*/

#include "../../src/Monitoring/TrackedProcess.h"

#include <vector>

extern HANDLE pipeHandle;

class APICallCounter
{
	/*
	public:
		APICallCounter(PLH::NatDetour * hook, std::string callName, std::unordered_map<std::string, APICallCounter *> * hooks);
		PLH::NatDetour* hook;
		std::string callName;
		void incrementCall();
	private:
		int numberCalls;
	*/
	/*
	public:
		APICallCounter(PLH::IatHook * hook, std::string callName, std::unordered_map<std::string, APICallCounter *> * hooks);
		PLH::IatHook* hook;
		std::string callName;
		void incrementCall();
	private:
		int numberCalls;

	*/
	public:
		APICallCounter(std::string callName, std::unordered_map<std::string, APICallCounter*>* hooks);
		
		std::string callName;
		void incrementCall();
	private:
		int numberCalls;
};

extern std::unordered_map<std::string, APICallCounter*> counterMap;
