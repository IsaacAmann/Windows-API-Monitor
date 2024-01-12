#include "Hooks.h"

APICallCounter::APICallCounter(PLH::NatDetour* hook, std::string callName, std::unordered_map<std::string, APICallCounter*> * hooks)
{
	this->hook = hook;
	this->callName = callName;
	this->numberCalls = 0;

	//Push self to hashmap
	hooks->insert(std::make_pair(callName, this));

	hook->hook();
}

void APICallCounter::incrementCall()
{
	numberCalls++;
	//send message
    CountUpdateMessage message;
    message.calls = this->numberCalls;
    memcpy(message.callName, this->callName.c_str(), this->callName.length());
    WriteFile(
        pipeHandle,
        &message,
        sizeof(CountUpdateMessage),
        NULL,
        NULL
    );
}