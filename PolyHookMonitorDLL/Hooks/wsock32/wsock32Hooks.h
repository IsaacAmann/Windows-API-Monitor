#pragma once

#include <WinSock2.h>
#include "../Hooks.h"


void hookwinsock32APICalls(std::unordered_map<std::string, APICallCounter*>* hooks);

SOCKET WSAAPI hooksocket(int af, int type, int protocol);

