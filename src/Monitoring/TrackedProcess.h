#pragma once

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
#include <unordered_map>
#include <cstring>


typedef struct CallCountContainer
{
	//Kernel32
	int cGetCurrentProcessId;
	int cOpenProcess;
	int cWriteFile;
	int cCreateFile;
	int cCreateFileMapping;
	int cGetWindowsDirectory;
	int cSetFileTime;
	int cVirtualAlloc;
	int cVirtualAllocEx;
	int cVirtualProtect;
	int cReadProcessMemory;
	int cWriteProcessMemory;
	int cCreateRemoteThread;
	int cQueueUserAPC;
	int cConnectNamedPipe;
	int cCreateNamedPipe;
	int cEnumProcesses;
	int cEnumProcessModules;
	int cGetModuleFileName;
	int cGetModuleHandle;
	int cPeekNamedPipe;
	int cTerminateProcess;
	int cCreateProcess;
	int cIsDebuggerPresent;
	//wsock32
	int csocket;
	int cbind;
	int clisten;
	int caccept;
	int cconnect;
	int csend;
	int crecv;
	int cgethostname;
	//wininet
	int cInternetOpen;
	int cInternetOpenUrl;
	int cInternetReadFile;
	int cInternetWriteFile;
	//advapi32
	int cOpenSCManager; 
	int cCreateService;
	int cStartServiceCtrlDispatcher;
	int cRegCreateKey;
	int cRegOpenKey;
	int cRegSetValue;
	int cRegDeleteKey;
	int cRegGetValue;
	//user32
	int cRegisterHotKey;
	int cGetAsyncKeyState;
	int cSetWindowsHookEx;
	int cGetForegroundWindow;
	int cGetDC;
	int cGetKeyState;
	int cMapVirtualKey;

} CallCountContainer;

class TrackedProcess
{
	public:
		TrackedProcess(HANDLE processHandle, DWORD PID);
		~TrackedProcess();
		bool processRunning;
		TCHAR processName[MAX_PATH];
		TCHAR processExecutablePath[MAX_PATH];
		HANDLE processHandle;
		HANDLE pipeHandle;
		CallCountContainer* callCountContainer;


		DWORD PID;
		void printProcessInfo();
		void readCountUpdateQueue();
		void attach();
		void detach();
		//Update fields including processRunning
		void getProcessUpdate();

	private:
		//Get process info on startup
		void getProcessInfo();
		

};
