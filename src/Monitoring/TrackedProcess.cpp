#include "TrackedProcess.h"

const int PIPE_BUFFER_SIZE = 1000;

//Base name for named pipe, should be appended with PID for the process
//That will be sending data on the pipe
std::string pipeBaseName = "Local\\APIMonitor";
char libPath[_MAX_PATH] = "C:\\Users\\isaac\\Documents\\programming\\Windows-API-Monitor\\x64\\Debug\\PolyHookMonitorDLL.dll";
//char libPath[_MAX_PATH] = "C:\\Users\\isaac\\Documents\\programming\\Windows-API-Monitor\\x64\\Debug\\MonitorDLL.dll";
TrackedProcess::TrackedProcess(HANDLE processHandle, DWORD PID)
{
	this->processHandle = processHandle;
	this->PID = PID;
	processRunning = true;
	getProcessInfo();
	//printProcessInfo();
	int testPID = 1612;
	/*
	//Create named pipe for receiving API call totals
	std::string pipeName = pipeBaseName;
	pipeName.append(std::to_string(PID));
	std::cout << pipeName << std::endl;
	pipeHandle = CreateNamedPipeA(
		pipeName.c_str(),
		PIPE_ACCESS_INBOUND,
		PIPE_TYPE_BYTE | PIPE_NOWAIT,
		1,
		0,
		PIPE_BUFFER_SIZE * sizeof(CountUpdateMessage),
		0,
		NULL
	);
	*/
	//Create shared memory to pass counts from tracked process
	std::string pipeName = pipeBaseName;
	pipeName.append(std::to_string(PID));
	std::cout << pipeName << std::endl;
	pipeHandle = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(CallCountContainer), pipeName.c_str());
	if (pipeHandle == NULL)
	{
		_tprintf(TEXT("Could not create file mapping (%d)\n"), GetLastError());
	}
	callCountContainer = (CallCountContainer*)MapViewOfFile(pipeHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CallCountContainer));

	attach();
	//std::cout << libPath << std::endl;
	//printProcessInfo();
	readCountUpdateQueue();

}

TrackedProcess::~TrackedProcess()
{
	//Close process Handle
	CloseHandle(processHandle);
}

void TrackedProcess::readCountUpdateQueue()
{
	std::cout << callCountContainer->test << std::endl;
	/*
	BOOL readSuccess = FALSE;
	CountUpdateMessage currentMessage;
	DWORD bytesRead = 0;

	readSuccess = ReadFile(
		pipeHandle,
		&currentMessage,
		sizeof(CountUpdateMessage),
		&bytesRead,
		NULL
	);
	//std::cout << "reading\n";
	while (readSuccess == TRUE && bytesRead != 0)
	{
		//Process message
		std::cout << "PID: " << PID << std::endl;
		std::cout << "\tAPI Call: " << currentMessage.callName << std::endl;
		std::cout << "\tTotal Calls: " << currentMessage.calls << std::endl;
		//Update count
		//Check for existing counter
		auto iterator = callCounters.find(currentMessage.callName);
		if (iterator == callCounters.end())
		{
			//First call to this API call, need to create counter
			callCounters.insert(std::make_pair(currentMessage.callName, currentMessage));
		}
		else
		{
			//Counter exists, update it
			callCounters.at(currentMessage.callName).calls = currentMessage.calls;
		}

		//Get next message in queue
		readSuccess = ReadFile(
			pipeHandle,
			&currentMessage,
			sizeof(CountUpdateMessage),
			&bytesRead,
			NULL
		);
	}
	*/
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

	//Probably don't need to wait
	//WaitForSingleObject(threadHandle, 1000);

	//ConnectNamedPipe(pipeHandle, NULL);
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
			std::cout << PID << " Has exited \n";
		}
	}
}