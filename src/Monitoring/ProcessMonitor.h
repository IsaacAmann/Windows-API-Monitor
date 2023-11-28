#include <iostream>
#include <vector>
#include <windows.h>
#include <Psapi.h>
#include <tchar.h>
#include "TrackedProcess.h"

class ProcessMonitor
{
	public:
		bool initialize();
		void printProcessList();
	private:
		std::vector<TrackedProcess> processList;

};