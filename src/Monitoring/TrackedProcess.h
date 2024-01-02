#pragma once

#include <iostream>
#include <vector>
#include <windows.h>
#include <Psapi.h>
#include <processthreadsapi.h>
#include <memoryapi.h>
#include <tchar.h>
#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>


typedef struct CountUpdateMessage
{
	char callName[60];
	int calls;

} CountUpdateMessage;

class TrackedProcess
{
	public:
		TrackedProcess(HANDLE processHandle, DWORD PID);
		
		std::unordered_map<std::string, CountUpdateMessage> callCounters;
		bool processRunning;
		TCHAR processName[MAX_PATH];
		TCHAR processExecutablePath[MAX_PATH];
		HANDLE processHandle;
		HANDLE pipeHandle;
		DWORD PID;
		void printProcessInfo();
		void readCountUpdateQueue();
		void attach();
		void detach();

	private:
		//Get process info on startup
		void getProcessInfo();
		//Update fields including processRunning
		void getProcessUpdate();

};
