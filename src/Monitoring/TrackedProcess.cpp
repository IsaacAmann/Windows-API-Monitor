#include "TrackedProcess.h"

char libPath[_MAX_PATH] = "C:\\Users\\isaac\\Documents\\programming\\Windows-API-Monitor\\x64\\Debug\\PolyHookMonitorDLL.dll";
//char libPath[_MAX_PATH] = "C:\\Users\\isaac\\Documents\\programming\\Windows-API-Monitor\\x64\\Debug\\MonitorDLL.dll";
TrackedProcess::TrackedProcess(HANDLE processHandle, DWORD PID)
{
	this->processHandle = processHandle;
	this->PID = PID;
	processRunning = true;
	getProcessInfo();
	//printProcessInfo();
	if (PID == 11412)
	{
		attach();
		//std::cout << libPath << std::endl;
		printProcessInfo();
	}
}

//Load dll into process
void TrackedProcess::attach()
{
	std::cout << "attaching\n";
	
	HANDLE threadHandle;
	void* injectedLibAddress;
	HMODULE hkernel32 = GetModuleHandleA("Kernel32");

	//Allocate memory in target process 
	injectedLibAddress = VirtualAllocEx(this->processHandle, NULL, sizeof(libPath), MEM_COMMIT, PAGE_READWRITE);
	//Write DLL path name to memory
	WriteProcessMemory(this->processHandle, injectedLibAddress, (void*)libPath, sizeof(libPath), NULL);
	//Create new thread and load dll into process
	threadHandle = CreateRemoteThread(this->processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(hkernel32, "LoadLibraryA"), injectedLibAddress, 0,  NULL);

	WaitForSingleObject(threadHandle, INFINITE);
	std::cout << "attached!\n";
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

	//List modules
	HMODULE mod[1000];
	DWORD cbNeeded;
	
	if (EnumProcessModules(processHandle, mod, sizeof(mod), &cbNeeded))
	{
		for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR modName[MAX_PATH];
			GetModuleFileNameEx(processHandle, mod[i], modName, sizeof(modName) / sizeof(TCHAR));
			_tprintf(TEXT("%s\n"), modName);
		}
	}
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