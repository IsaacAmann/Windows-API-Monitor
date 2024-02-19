# Windows API Monitor
User-level process monitor that collects Windows API usage data from Windows processes for process behavior analysis

## How it works
The process monitor obtains a list of all processes, creating an object for each of them to contain collected information

### DLL Injection
Each monitored process is injected with a DLL containing code for hooking monitored Windows API calls
and for communicating with the monitor. I used the common technique of opening a remote thread on the process that would then call LoadLibrary
to load the DLL. 

### Function Hooking
Function hooking was implemented using the Microsoft Detours library. The hook simply increments the counter for the corresponding Windows API
function. The Windows API functions hooked can be found in TrackedProcess.h
https://github.com/IsaacAmann/Windows-API-Monitor/blob/master/src/Monitoring/TrackedProcess.h

### Interprocess Communication

