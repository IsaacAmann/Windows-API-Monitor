#include "ProcessMonitor.h"

using json = nlohmann::json;

//Extern variables defined in Main.h
extern std::string clientId;
extern std::string apiKey;
extern std::string API_ENDPOINT;

const bool USE_TEST_PID = false;
const int TEST_PID = 10308;

ProcessMonitor::~ProcessMonitor()
{
	delete managerThread;

}

bool ProcessMonitor::initialize()
{
	isRunning = true;
	//Create thread for managing tracked processes
	managerThread = new std::thread(managerThreadExecute, this);
	//Add self to excluded PIDs
	ExcludedPID.push_back((int)GetCurrentProcessId());
	//Add excluded process names
	ExcludedProcessNames.push_back(std::string("explorer.exe"));
	ExcludedProcessNames.push_back(std::string("devenv.exe"));
	ExcludedProcessNames.push_back(std::string("dropbox.exe"));

	//ExcludedProcessNames.push_back(std::string("TestConsoleApp.exe"));

	return true;
}

void ProcessMonitor::stop()
{
	isRunning = false;

}

void managerThreadExecute(ProcessMonitor* monitor)
{
	std::cout << "Manager Thread Running " << monitor->number << std::endl;
	std::vector<DWORD> pidRemoveList;

	while (monitor->isRunning)
	{
		monitor->scanForProcesses();
		for (auto iterator : monitor->processList)
		{
			TrackedProcess* currentProcess = iterator.second;
			//std::cout << "reading" << iterator.second->PID << std::endl;
			currentProcess->readCountUpdateQueue();
			currentProcess->getProcessUpdate();
			//Check for process exit (need to send datapoint and delete object if exit)
			if(currentProcess->processRunning == false)
			{
				pidRemoveList.push_back(currentProcess->PID);
			}
		}
		//Remove processes that have exited
		for (int i = 0; i < pidRemoveList.size(); i++)
		{
			TrackedProcess* currentProcess = monitor->processList.at(pidRemoveList[i]);
			//Remove from process list
			monitor->processList.erase(currentProcess->PID);
			//Make HTTP request to server 
			sendDataPoint(currentProcess);
			//Free object
			delete currentProcess;
		}
		//Clear remove list
		pidRemoveList.clear();
	}
	
}

void sendDataPoint(TrackedProcess* process)
{
	CURL* curl;
	CURLcode response;

	std::string url = API_ENDPOINT + "/postDatapoint";
	std::cout << "url: " << url <<std::endl ;
	curl = curl_easy_init();

	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "charset: utf-8");
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);
		
		std::cout << "Posting datapoint \n";

		json payload;

		//Set clientId and apiKey for json
		payload["uuid"] = clientId;
		payload["token"] = apiKey;

		//Set process info
		char imageName[MAX_PATH];
		char executablePath[MAX_PATH];

		GetProcessImageFileNameA(process->processHandle, executablePath, MAX_PATH);

		payload["executablePath"] = std::string(executablePath);

		//Add each Win32 API call count to json
		//Kernel32
		payload["WinAPICounts"]["GetCurrentProcessId"] = process->callCountContainer->cGetCurrentProcessId;
		payload["WinAPICounts"]["OpenProcess"] = process->callCountContainer->cOpenProcess;
		payload["WinAPICounts"]["WriteFile"] = process->callCountContainer->cWriteFile;
		payload["WinAPICounts"]["CreateFile"] = process->callCountContainer->cCreateFile;
		payload["WinAPICounts"]["CreateFileMapping"] = process->callCountContainer->cCreateFileMapping;
		payload["WinAPICounts"]["GetWindowsDirectory"] = process->callCountContainer->cGetWindowsDirectory;
		payload["WinAPICounts"]["SetFileTime"] = process->callCountContainer->cSetFileTime;
		payload["WinAPICounts"]["VirtualAlloc"] = process->callCountContainer->cVirtualAlloc;
		payload["WinAPICounts"]["VirtualAllocEx"] = process->callCountContainer->cVirtualAllocEx;
		payload["WinAPICounts"]["VirtualProtect"] = process->callCountContainer->cVirtualProtect;
		payload["WinAPICounts"]["ReadProcessMemory"] = process->callCountContainer->cReadProcessMemory;
		payload["WinAPICounts"]["WriteProcessMemory"] = process->callCountContainer->cWriteProcessMemory;
		payload["WinAPICounts"]["CreateRemoteThread"] = process->callCountContainer->cCreateRemoteThread;
		payload["WinAPICounts"]["QueueUserAPC"] = process->callCountContainer->cQueueUserAPC;
		payload["WinAPICounts"]["ConnectNamedPipe"] = process->callCountContainer->cConnectNamedPipe;
		payload["WinAPICounts"]["CreateNamedPipe"] = process->callCountContainer->cCreateNamedPipe;
		payload["WinAPICounts"]["EnumProcesses"] = process->callCountContainer->cEnumProcesses;
		payload["WinAPICounts"]["EnumProcessModules"] = process->callCountContainer->cEnumProcessModules;
		payload["WinAPICounts"]["GetModuleFileName"] = process->callCountContainer->cGetModuleFileName;
		payload["WinAPICounts"]["GetModuleHandle"] = process->callCountContainer->cGetModuleHandle;
		payload["WinAPICounts"]["PeekNamedPipe"] = process->callCountContainer->cPeekNamedPipe;
		payload["WinAPICounts"]["TerminateProcess"] = process->callCountContainer->cTerminateProcess;
		payload["WinAPICounts"]["CreateProcess"] = process->callCountContainer->cCreateProcess;
		payload["WinAPICounts"]["IsDebuggerPresent"] = process->callCountContainer->cIsDebuggerPresent;
		//wsock32
		payload["WinAPICounts"]["socket"] = process->callCountContainer->csocket;
		payload["WinAPICounts"]["bind"] = process->callCountContainer->cbind;
		payload["WinAPICounts"]["listen"] = process->callCountContainer->clisten;
		payload["WinAPICounts"]["accept"] = process->callCountContainer->caccept;
		payload["WinAPICounts"]["connect"] = process->callCountContainer->cconnect;
		payload["WinAPICounts"]["send"] = process->callCountContainer->csend;
		payload["WinAPICounts"]["recv"] = process->callCountContainer->crecv;
		payload["WinAPICounts"]["gethostname"] = process->callCountContainer->cgethostname;
		//wininet
		payload["WinAPICounts"]["InternetOpen"] = process->callCountContainer->cInternetOpen;
		payload["WinAPICounts"]["InternetOpenUrl"] = process->callCountContainer->cInternetOpenUrl;
		payload["WinAPICounts"]["InternetReadFile"] = process->callCountContainer->cInternetReadFile;
		payload["WinAPICounts"]["InternetWriteFile"] = process->callCountContainer->cInternetWriteFile;
		//advapi32
		payload["WinAPICounts"]["OpenSCManager"] = process->callCountContainer->cOpenSCManager;
		payload["WinAPICounts"]["CreateService"] = process->callCountContainer->cCreateService;
		payload["WinAPICounts"]["StartServiceCtrlDispatcher"] = process->callCountContainer->cStartServiceCtrlDispatcher;
		payload["WinAPICounts"]["RegCreateKey"] = process->callCountContainer->cRegCreateKey;
		payload["WinAPICounts"]["RegOpenKey"] = process->callCountContainer->cRegOpenKey;
		payload["WinAPICounts"]["RegSetValue"] = process->callCountContainer->cRegSetValue;
		payload["WinAPICounts"]["RegDeleteKey"] = process->callCountContainer->cRegDeleteKey;
		payload["WinAPICounts"]["RegGetValue"] = process->callCountContainer->cRegGetValue;
		//user32
		payload["WinAPICounts"]["RegisterHotKey"] = process->callCountContainer->cRegisterHotKey;
		payload["WinAPICounts"]["GetAsyncKeyState"] = process->callCountContainer->cGetAsyncKeyState;
		payload["WinAPICounts"]["SetWindowsHookEx"] = process->callCountContainer->cSetWindowsHookEx;
		payload["WinAPICounts"]["GetForegroundWindow"] = process->callCountContainer->cGetForegroundWindow;
		payload["WinAPICounts"]["GetDC"] = process->callCountContainer->cGetDC;
		payload["WinAPICounts"]["GetKeyState"] = process->callCountContainer->cGetKeyState;
		payload["WinAPICounts"]["MapVirtualKey"] = process->callCountContainer->cMapVirtualKey;

		std::cout << payload.dump() << std::endl;
		curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, payload.dump().c_str());

		response = curl_easy_perform(curl);
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

		//Check for debugging targeting only one process (skip if not target PID)
		if (USE_TEST_PID && TEST_PID != processes[i])
			continue;

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

			//Check that process is not on the exclusion list
			bool excluded = false;
			char imageName[MAX_PATH];
			GetProcessImageFileNameA(process, imageName, MAX_PATH);
			//Check for excluded image names
			for (int j = 0; j < ExcludedProcessNames.size(); j++)
			{
				std::string imageNameString = std::string(imageName);
				if (imageNameString.find(ExcludedProcessNames[j]) != std::string::npos)
				{
					//std::cout << "byname" << imageNameString << ExcludedProcessNames[j] << std::endl;

					excluded = true;
					break;
				}
			}
			//Check for excluded PID's
			for (int j = 0; j < ExcludedPID.size(); j++)
			{
				if (ExcludedPID[j] == processes[i])
				{
					//std::cout << "byPID\n";
					excluded = true;
					break;
				}
			}

			//Check that process is not a Windows store app
			//They are sandboxed and crash upon DLL injection
			//Using GetPackageFamilyName to see if it returns error, see following stack overflow post
			//https://stackoverflow.com/questions/52207484/determine-if-c-application-is-running-as-a-uwp-app-with-legacy-support
			UINT32 packageNameLength = 0;
			LONG packageError = GetPackageFamilyName(process, &packageNameLength, NULL);
			if (packageError == ERROR_INSUFFICIENT_BUFFER)
			{
				std::cout << "Skipping UWP process\n";
			}
			
			
			if (excluded == true)
			{
				CloseHandle(process);
				continue;
			}
			
			currentTrackedProcess = new TrackedProcess(process, processes[i]);
			//currentTrackedProcess->printProcessInfo();
			//Add to hashmap
			processList.insert(std::make_pair(processes[i], currentTrackedProcess));

		}
	}
}