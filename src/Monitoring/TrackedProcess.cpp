#include "TrackedProcess.h"

const int PIPE_BUFFER_SIZE = 100;

//Base name for named pipe, should be appended with PID for the process
//That will be sending data on the pipe
std::string pipeBaseName = "\\\\.\\pipe\\APIMonitor";
char libPath[_MAX_PATH] = "C:\\Users\\isaac\\Documents\\programming\\Windows-API-Monitor\\x64\\Debug\\PolyHookMonitorDLL.dll";
//char libPath[_MAX_PATH] = "C:\\Users\\isaac\\Documents\\programming\\Windows-API-Monitor\\x64\\Debug\\MonitorDLL.dll";
TrackedProcess::TrackedProcess(HANDLE processHandle, DWORD PID)
{
	this->processHandle = processHandle;
	this->PID = PID;
	processRunning = true;
	getProcessInfo();
	//printProcessInfo();
	int testPID = 18548;
	if (PID == testPID)
	{
		attach();
		//std::cout << libPath << std::endl;
		printProcessInfo();
	}
	while (PID == testPID)
	{
		readCountUpdateQueue();
	}
}

void TrackedProcess::readCountUpdateQueue()
{
	BOOL readSuccess = FALSE;
	CountUpdateMessage currentMessage;
	DWORD bytesRead = 0;

	readSuccess = ReadFile(
		pipeHandle, 
		&currentMessage,
		bytesRead,
		NULL,
		NULL
	);
	std::cout << "reading\n";
	while (readSuccess == TRUE)
	{
		//Process message
		std::cout << currentMessage.callName << std::endl;
		std::cout << currentMessage.calls << std::endl;

		//Get next message in queue
		readSuccess = ReadFile(
			pipeHandle,
			&currentMessage,
			bytesRead,
			NULL,
			NULL
		);
	}

}

//Load dll into process
void TrackedProcess::attach()
{
	std::cout << "attaching\n";
	
	HANDLE threadHandle;
	void* injectedLibAddress;
	HMODULE hkernel32 = GetModuleHandleA("Kernel32");

	//Create named pipe for receiving API call totals
	std::string pipeName = pipeBaseName;
	pipeName.append(std::to_string(PID));

	pipeHandle = CreateNamedPipeA(
		pipeName.c_str(),
		PIPE_ACCESS_INBOUND,
		PIPE_TYPE_BYTE | PIPE_WAIT,
		1,
		0,
		PIPE_BUFFER_SIZE * sizeof(CountUpdateMessage),
		0,
		NULL
	);

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