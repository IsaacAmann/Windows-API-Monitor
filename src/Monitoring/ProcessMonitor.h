#include <iostream>
#include <vector>
#include <windows.h>
#include <Psapi.h>
#include <tchar.h>
#include <process.h>
#include <strsafe.h>
#include <string>
#include "TrackedProcess.h"

DWORD WINAPI managerThreadExecute(LPVOID lpParam);

class ProcessMonitor
{
	public:
		bool initialize();
		//Call EnumProcesses and update process list with new entries
		void scanForProcesses();

		void printProcessList();

		HANDLE managerThread;
		int number;
		int getNumber();
	private:
		std::vector<TrackedProcess> processList;
};

struct ManagerThreadParameter
{
	ProcessMonitor* monitor;

};