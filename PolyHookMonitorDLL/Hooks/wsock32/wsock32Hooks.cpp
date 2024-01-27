
#include "wsock32Hooks.h"

const bool PRINT_CALLS = true;

//Function pointers to original Win32 API functions
SOCKET(WSAAPI * origsocket)(int af, int type, int protocol) = socket;
int(WSAAPI* origbind)(SOCKET s, const sockaddr* name, int namelen) = bind;
int(WSAAPI* origlisten)(SOCKET s, int backlog) = listen;
SOCKET(WSAAPI* origaccept)(SOCKET s, sockaddr* addr, int* addrlen) = accept;
int(WSAAPI* origconnect)(SOCKET s, const sockaddr* name, int namelen) = connect;
int(WSAAPI* origsend)(SOCKET s, const char* buf, int len, int flags) = send;
int(__stdcall* origrecv)(SOCKET s, char* buf, int len, int flags) = recv;
int(__stdcall* origgethostname)(char* name, int namelen) = gethostname;

void hookwinsock32APICalls(std::unordered_map<std::string, APICallCounter*>* hooks)
{

    APICallCounter* currentCounter = NULL;


    //Hooking using Microsoft Detours library
    DetourTransactionBegin();
    currentCounter = new APICallCounter("socket", hooks);
    DetourRestoreAfterWith();
    DetourAttach(&origsocket, hooksocket);

    currentCounter = new APICallCounter("bind", hooks);
    DetourAttach(&origbind, hookbind);

    currentCounter = new APICallCounter("listen", hooks);
    DetourAttach(&origlisten, hooklisten);

    currentCounter = new APICallCounter("accept", hooks);
    DetourAttach(&origaccept, hookaccept);

    currentCounter = new APICallCounter("connect", hooks);
    DetourAttach(&origconnect, hookconnect);

    currentCounter = new APICallCounter("send", hooks);
    DetourAttach(&origsend, hooksend);

    currentCounter = new APICallCounter("recv", hooks);
    DetourAttach(&origrecv, hookrecv);

    currentCounter = new APICallCounter("gethostname", hooks);
    DetourAttach(&origgethostname, hookgethostname);



    DetourTransactionCommit();
}

SOCKET WSAAPI hooksocket(int af, int type, int protocol)
{
    if (PRINT_CALLS)
        std::cout << "Called socket\n";
    counterMap.at("socket")->incrementCall();

    return origsocket(af, type, protocol);
}

int WSAAPI hookbind(SOCKET s, const sockaddr* name, int namelen)
{
    if (PRINT_CALLS)
        std::cout << "Called bind\n";
    counterMap.at("bind")->incrementCall();

    return origbind(s, name, namelen);
}

int WSAAPI hooklisten(SOCKET s, int backlog)
{
    if (PRINT_CALLS)
        std::cout << "Called listen\n";
    counterMap.at("listen")->incrementCall();

    return origlisten(s, backlog);
}

SOCKET WSAAPI hookaccept(SOCKET s, sockaddr* addr, int* addrlen)
{
    if (PRINT_CALLS)
        std::cout << "Called accept\n";
    counterMap.at("accept")->incrementCall();

    return origaccept(s, addr, addrlen);
}

int WSAAPI hookconnect(SOCKET s, const sockaddr* name, int namelen)
{
    if (PRINT_CALLS)
        std::cout << "Called connect\n";
    counterMap.at("connect")->incrementCall();

    return origconnect(s, name, namelen);
}

int WSAAPI hooksend(SOCKET s, const char* buf, int len, int flags)
{
    if (PRINT_CALLS)
        std::cout << "Called send\n";
    counterMap.at("send")->incrementCall();

    return origsend(s, buf, len, flags);
}

int __stdcall hookrecv(SOCKET s, char* buf, int len, int flags) 
{
    if (PRINT_CALLS)
        std::cout << "Called recv\n";
    counterMap.at("recv")->incrementCall();

    return origrecv(s, buf, len, flags);
}

int __stdcall hookgethostname(char* name, int namelen)
{
    if (PRINT_CALLS)
        std::cout << "Called gethostname\n";
    counterMap.at("gethostname")->incrementCall();

    return origgethostname(name, namelen);
}
