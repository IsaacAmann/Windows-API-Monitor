
#include "wsock32Hooks.h"

const bool PRINT_CALLS = true;

//Function pointers to original Win32 API functions
SOCKET(WSAAPI * origsocket)(int af, int type, int protocol) = socket;

void hookwinsock32APICalls(std::unordered_map<std::string, APICallCounter*>* hooks)
{

    APICallCounter* currentCounter = NULL;


    //Hooking using Microsoft Detours library
    DetourTransactionBegin();
    currentCounter = new APICallCounter("socket", hooks);
    DetourRestoreAfterWith();
    DetourAttach(&origsocket, hooksocket);


    DetourTransactionCommit();
}

SOCKET WSAAPI hooksocket(int af, int type, int protocol)
{
    if (PRINT_CALLS)
        std::cout << "Called socket\n";
    counterMap.at("socket")->incrementCall();

    return origsocket(af, type, protocol);
}