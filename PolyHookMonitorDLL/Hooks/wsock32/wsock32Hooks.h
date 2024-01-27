#pragma once

#include <WinSock2.h>
#include "../Hooks.h"


void hookwinsock32APICalls(std::unordered_map<std::string, APICallCounter*>* hooks);

SOCKET WSAAPI hooksocket(int af, int type, int protocol);

int WSAAPI hookbind(SOCKET s, const sockaddr* name, int namelen);

int WSAAPI hooklisten(SOCKET s, int backlog);

SOCKET WSAAPI hookaccept(SOCKET s, sockaddr* addr, int* addrlen);

int WSAAPI hookconnect(SOCKET s, const sockaddr* name, int namelen);

int WSAAPI hooksend(SOCKET s, const char* buf, int len, int flags);

int __stdcall hookrecv(SOCKET s, char *buf, int len, int flags);

int __stdcall hookgethostname(char* name, int namelen);