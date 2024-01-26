#include "Hooks.h"

/*
APICallCounter::APICallCounter(PLH::NatDetour* hook, std::string callName, std::unordered_map<std::string, APICallCounter*> * hooks)
{
	this->hook = hook;
	this->callName = callName;
	this->numberCalls = 0;

	//Push self to hashmap
	hooks->insert(std::make_pair(callName, this));

	hook->hook();
}
*/
/*
APICallCounter::APICallCounter(PLH::IatHook* hook, std::string callName, std::unordered_map<std::string, APICallCounter*>* hooks)
{
	this->hook = hook;
	this->callName = callName;
	this->numberCalls = 0;

	//Push self to hashmap
	hooks->insert(std::make_pair(callName, this));

	hook->hook();
}
*/

APICallCounter::APICallCounter(std::string callName, std::unordered_map<std::string, APICallCounter*>* hooks)
{
	this->callName = callName;
	this->numberCalls = 0;

	//Push self to hashmap
	hooks->insert(std::make_pair(callName, this));
	
}

void APICallCounter::incrementCall()
{
	numberCalls++;
	//send message
    CountUpdateMessage message;
    message.calls = this->numberCalls;
    memcpy(message.callName, this->callName.c_str(), this->callName.length());
	//add null byte to the end of call name
	message.callName[this->callName.length()] = '\0';
	//Need to change how this is handled, WriteFile blocks and programs spend too much time writing
	//and some will freeze up
    WriteFile(
        pipeHandle,
        &message,
        sizeof(CountUpdateMessage),
        NULL,
        NULL
    );
}