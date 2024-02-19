# Windows API Monitor
User-level process monitor that collects Windows API usage data from Windows processes for process behavior analysis

## Motivation
I wanted an interesting data set to work with for a research project I am contributing to involving cluster analysis. 
Using data collected, it may be possible to group processes by their behavior through clustering. Possible uses for this include
malware detection. For example, ransomware would make a very large number of file access API calls. If another process had a similar 
number of file access calls, it may also be ransomware. 

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
Initially used a named pipe to use message passing for communication. Switched over to using named shared memory for communication.

### Posting Datapoints
Collected data is posted to the server through HTTP. The source code for the server application can be found at the following repository: https://github.com/IsaacAmann/Windows-API-Monitor-Collection-Server
