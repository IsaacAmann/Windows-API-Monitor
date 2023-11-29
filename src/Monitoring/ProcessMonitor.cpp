#include "ProcessMonitor.h"

ProcessMonitor::~ProcessMonitor()
{
	delete managerThread;

}

bool ProcessMonitor::initialize()
{
	//Create thread for managing tracked processes
	managerThread = new std::thread(managerThreadExecute, this);
	return true;
}

void managerThreadExecute(ProcessMonitor* monitor)
{
	std::cout << "Manager Thread Running " << monitor->number << std::endl;
	monitor->scanForProcesses();
}

void ProcessMonitor::scanForProcesses()
{
	DWORD processes[1024], needed, numberProcess;

	EnumProcesses(processes, sizeof(processes), &needed);

	numberProcess = needed / sizeof(DWORD);

	std::cout << "Processes running: " << numberProcess << std::endl;

	for (int i = 0; i < numberProcess; i++)
	{
		TCHAR currentName[MAX_PATH] = TEXT("Blank");
		TCHAR currentFileName[MAX_PATH] = TEXT("Blank");
		HMODULE mod;
		DWORD cbNeeded;
		//HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
		HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processes[i]);
		std::cout << GetLastError() << std::endl;

		if (EnumProcessModules(process, &mod, sizeof(mod), &cbNeeded))
		{
			GetModuleBaseName(process, mod, currentName, sizeof(currentName) / sizeof(TCHAR));
			GetProcessImageFileName(process, currentFileName, sizeof(currentFileName) / sizeof(TCHAR));

		}

		_tprintf(TEXT("%s  (PID: %u)\n"), currentName, processes[i]);
		_tprintf(TEXT("Executable Path: %s\n"), currentFileName);
		CloseHandle(process);
	}
}