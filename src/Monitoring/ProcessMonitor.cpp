#include "ProcessMonitor.h"

using json = nlohmann::json;

extern std::string clientId;
extern std::string apiKey;
extern std::string API_ENDPOINT;

const bool USE_TEST_PID = true;
const int TEST_PID = 2916;

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

	std::string url = API_ENDPOINT + "/test";
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
		payload["clientId"] = clientId;
		payload["token"] = apiKey;

		//Add each Win32 API call count to json
		for (auto iterator : process->callCounters)
		{
			CountUpdateMessage currentCounter =  iterator.second;
			
			payload["WinAPICounts"][currentCounter.callName] = currentCounter.calls;
		}
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
			currentTrackedProcess = new TrackedProcess(process, processes[i]);
			//currentTrackedProcess->printProcessInfo();
			//Add to hashmap
			processList.insert(std::make_pair(processes[i], currentTrackedProcess));

		}
	}
}