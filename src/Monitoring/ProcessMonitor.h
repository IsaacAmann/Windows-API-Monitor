#include <iostream>
#include <vector>
#include <windows.h>
#include <Psapi.h>
#include <tchar.h>
#include <process.h>
#include <string>
#include <thread>
#include <mutex>
#include "TrackedProcess.h"


class ProcessMonitor
{
	public:
		~ProcessMonitor();
		bool initialize();
		//Call EnumProcesses and update process list with new entries
		void scanForProcesses();

		void printProcessList();


		std::thread* managerThread;
		int number;
	private:
		std::vector<TrackedProcess> processList;
};

void managerThreadExecute(ProcessMonitor* monitor);


