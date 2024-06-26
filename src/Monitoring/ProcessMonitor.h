#pragma once

#include <windows.h>
#include <curl/curl.h>

#include <iostream>
#include <vector>
#include <Psapi.h>
#include <tchar.h>
#include <process.h>
#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <json.hpp>
#include "TrackedProcess.h"
#include <appmodel.h>




class ProcessMonitor
{
	public:
		~ProcessMonitor();
		bool initialize();
		//Call EnumProcesses and update process list with new entries
		void scanForProcesses();

		void printProcessList();

		std::vector<std::string> ExcludedProcessNames;
		std::vector<int> ExcludedPID;
		void stop();

		std::thread* managerThread;
		int number;
		bool isRunning;
		std::unordered_map<DWORD, TrackedProcess*> processList;
		
	
};

void managerThreadExecute(ProcessMonitor* monitor);

void sendDataPoint(TrackedProcess* process);


