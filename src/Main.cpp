#define CURL_STATICLIB

#include "Main.h"


ProcessMonitor* monitor;
int main()
{
	monitor = new(ProcessMonitor);
	//monitor.scanForProcesses();
	monitor->initialize();
	std::cout << "main thread" << std::endl;


	while (true);
	//Wait for threads to close before exit
	return 0;
}
