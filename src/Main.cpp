#define CURL_STATICLIB

#include "Main.h"
#include <detours.h>
#include <curl.h>
#include <Windows.h>
#include <Psapi.h>

int main()
{
	std::cout << "test" << '\n';
    DetourAttach(NULL, NULL);
	DWORD processes[1024], needed, numberProcess;

	EnumProcesses(processes, sizeof(processes), &needed);

	numberProcess = needed / sizeof(DWORD);

	std::cout << "Processes running: " << numberProcess << std::endl;

	return 0;
}
