#include "TrackedProcess.h"


TrackedProcess::TrackedProcess(HANDLE processHandle, DWORD PID)
{
	this->processHandle = processHandle;
	this->PID = PID;
	processRunning = true;
	getProcessInfo();
	printProcessInfo();
}

void TrackedProcess::getProcessInfo()
{
	HMODULE mod;
	DWORD cbNeeded;
	//Get process Name
	if (EnumProcessModules(processHandle, &mod, sizeof(mod), &cbNeeded))
	{
		GetModuleBaseName(processHandle, mod, processName, sizeof(processName) / sizeof(TCHAR));
		GetProcessImageFileName(processHandle, processExecutablePath, sizeof(processExecutablePath) / sizeof(TCHAR));
	}
}

void TrackedProcess::printProcessInfo()
{
	_tprintf(TEXT("%s  (PID: %u)\n"), processName, PID);
	_tprintf(TEXT("Executable Path: %s\n"), processExecutablePath);
}

void TrackedProcess::getProcessUpdate()
{
	//Check that process is still running
	DWORD exitCode;
	BOOL exitCodeStatus = GetExitCodeProcess(processHandle, &exitCode);

	if (exitCodeStatus == TRUE)
	{
		if (exitCode == STILL_ACTIVE)
		{
			processRunning = true;
		}
		else
		{
			processRunning = false;
		}
	}
}