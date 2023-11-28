#define CURL_STATICLIB

#include "Main.h"


ProcessMonitor* monitor;
int main()
{
	monitor = new(ProcessMonitor);
	//monitor.scanForProcesses();
	monitor->initialize();



	while (true);
	//Wait for threads to close before exit
	return 0;
}
