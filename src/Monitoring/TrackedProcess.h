#include <iostream>
#include <vector>
#include <windows.h>
#include <Psapi.h>
#include <processthreadsapi.h>
#include <memoryapi.h>
#include <tchar.h>
#include <string>
#include <thread>
#include <mutex>



class TrackedProcess
{
	public:
		TrackedProcess(HANDLE processHandle, DWORD PID);
		

		bool processRunning;
		TCHAR processName[MAX_PATH];
		TCHAR processExecutablePath[MAX_PATH];
		HANDLE processHandle;
		DWORD PID;
		void printProcessInfo();
		void attach();

	private:
		//Get process info on startup
		void getProcessInfo();
		//Update fields including processRunning
		void getProcessUpdate();

};