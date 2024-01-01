#include "Kernel32Hooks.h"

uint64_t trampolineGetCurrentProcessId;

void hookKernel32APICalls(std::vector<PLH::NatDetour*>* hooks)
{
    //Hook GetCurrentProcessId
    uint64_t callBack = (uint64_t)hookGetCurrentProcessId;
    uint64_t targetAddress = (uint64_t)GetProcAddress(LoadLibraryA("Kernel32.dll"), "GetCurrentProcessId");
    PLH::NatDetour* currentHook = NULL;

    currentHook = new PLH::NatDetour(targetAddress, callBack, &trampolineGetCurrentProcessId);
    currentHook->hook();
    hooks->push_back(currentHook);


}

//Hook function for GetCurrentProcessId
DWORD __stdcall hookGetCurrentProcessId()
{
    std::cout << "Called get id\n";

    return PLH::FnCast(trampolineGetCurrentProcessId, &GetCurrentProcessId)();
}