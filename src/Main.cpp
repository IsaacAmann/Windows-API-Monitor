#define CURL_STATICLIB

#include "Main.h"
#include <detours.h>
#include <curl.h>

int main()
{
	std::cout << "test" << '\n';
    DetourAttach(NULL, NULL);
	
	return 0;
}
