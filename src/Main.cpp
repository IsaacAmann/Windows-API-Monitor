#define CURL_STATICLIB

#include "Main.h"

int main()
{
	std::cout << "test" << '\n';
    DetourAttach(NULL, NULL);
	DWORD processes[1024], needed, numberProcess;

	EnumProcesses(processes, sizeof(processes), &needed);

	numberProcess = needed / sizeof(DWORD);

	std::cout << "Processes running: " << numberProcess << std::endl;

	for (int i = 0; i < numberProcess; i++)
	{
		TCHAR currentName[MAX_PATH] = TEXT("Blank");
		HMODULE mod;
		DWORD cbNeeded;

		HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);

		if (EnumProcessModules(process, &mod, sizeof(mod), &cbNeeded))
		{
			GetModuleBaseName(process, mod, currentName, sizeof(currentName) / sizeof(TCHAR));
		}

		_tprintf(TEXT("%s  (PID: %u)\n"), currentName, i);

		CloseHandle(process);
	}

	return 0;
}
