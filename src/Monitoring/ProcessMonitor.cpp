#include "ProcessMonitor.h"

ProcessMonitor::~ProcessMonitor()
{
	delete managerThread;

}

bool ProcessMonitor::initialize()
{
	isRunning = true;
	//Create thread for managing tracked processes
	managerThread = new std::thread(managerThreadExecute, this);
	return true;
}

void ProcessMonitor::stop()
{
	isRunning = false;

}

void managerThreadExecute(ProcessMonitor* monitor)
{
	std::cout << "Manager Thread Running " << monitor->number << std::endl;
	while (monitor->isRunning)
	{
		monitor->scanForProcesses();
		for (auto iterator : monitor->processList)
		{
			//std::cout << "reading" << iterator.second->PID << std::endl;
			iterator.second->readCountUpdateQueue();
		}
	}
	
}

void ProcessMonitor::scanForProcesses()
{
	DWORD processes[1024], needed, numberProcess;

	EnumProcesses(processes, sizeof(processes), &needed);

	numberProcess = needed / sizeof(DWORD);

	//std::cout << "Processes running: " << numberProcess << std::endl;

	for (int i = 0; i < numberProcess; i++)
	{
		TCHAR currentName[MAX_PATH] = TEXT("Blank");
		TCHAR currentFileName[MAX_PATH] = TEXT("Blank");
		HMODULE mod;
		DWORD cbNeeded;
		//HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
		//std::cout << GetLastError() << std::endl;

		TrackedProcess* currentTrackedProcess;
		//Look up process by PID on processList hashmap
		if (processList.find(processes[i]) != processList.end())
		{
			currentTrackedProcess = processList.at(processes[i]);
		}
		else
		{
			currentTrackedProcess = NULL;
		}
		if (currentTrackedProcess != NULL)
		{
			//Check that the process in the trackedProcess object has not closed (Catches the case of a collision of an old PID)
			if (currentTrackedProcess->processRunning == true)
			{
				//Do not need to add to hashmap
			}
			else
			{
				//Process at PID has closed

				//Handle TrackedProcess clean up

				//Delete TrackedProcess object and remove from hashmap

				//Add new TrackedProcess object to hashmap
			}
		}
		else
		{
			//new Process and no collision with old PID
			//Create TrackedProcess object
			HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processes[i]);
			currentTrackedProcess = new TrackedProcess(process, processes[i]);
			//currentTrackedProcess->printProcessInfo();
			//Add to hashmap
			processList.insert(std::make_pair(processes[i], currentTrackedProcess));

		}
	}
}