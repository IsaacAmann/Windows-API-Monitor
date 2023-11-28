
#include "Main.h"
#include <detours.h>

int main()
{
	std::cout << "test" << '\n';
    DetourAttach(NULL, NULL);
	return 0;
}
