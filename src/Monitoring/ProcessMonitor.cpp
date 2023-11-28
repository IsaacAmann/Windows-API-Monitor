#include "ProcessMonitor.h"
int ProcessMonitor::getNumber()
{
	return this->number;
}
bool ProcessMonitor::initialize()
{
	//Create thread for managing tracked processes
	struct ManagerThreadParameter* param = (struct ManagerThreadParameter*) malloc(sizeof(struct ManagerThreadParameter));
	number = 2;
	param->monitor = this;
	managerThread = (HANDLE)CreateThread(NULL, 0, managerThreadExecute, this, 0, NULL);
	//WaitForSingleObject(managerThread, INFINITE);
	return true;
}

 DWORD WINAPI managerThreadExecute(LPVOID lpParam)
{
	TCHAR messageBuffer[300];
	size_t cchStringSize;
	DWORD dwChars;

	struct ManagerThreadParameter* param = (struct ManagerThreadParameter*)lpParam;

	
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	StringCchPrintf(messageBuffer, 300, TEXT("Test from manager thread"));
	StringCchLength(messageBuffer, 300, &cchStringSize);
	WriteConsole(hStdout, messageBuffer, (DWORD)cchStringSize, &dwChars, NULL);

	std::cout << "Unsafe print" << std::endl;
	param->monitor->scanForProcesses();
	//std::cout << param->monitor->testString << std::endl;
	printf("%d\n", param->monitor->getNumber());
	return 0;
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
		HMODULE mod;
		DWORD cbNeeded;
		//HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
		HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processes[i]);
		std::cout << GetLastError() << std::endl;

		if (EnumProcessModules(process, &mod, sizeof(mod), &cbNeeded))
		{
			GetModuleBaseName(process, mod, currentName, sizeof(currentName) / sizeof(TCHAR));
		}

		_tprintf(TEXT("%s  (PID: %u)\n"), currentName, i);

		CloseHandle(process);
	}
}