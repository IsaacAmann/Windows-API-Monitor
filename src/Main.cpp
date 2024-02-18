#define CURL_STATICLIB

#include "Main.h"
void testHTTPRequest();

ProcessMonitor* monitor;
int main()
{
	API_ENDPOINT = "http://localhost:8080";
	testHTTPRequest();
	//Get API key
	//Check registry to see if ID and key are present already
	DWORD keyDisposition;
	HKEY uuidKey, tokenKey;

	std::string enteredUUID, enteredToken;

	LSTATUS registryOpenResult = RegCreateKeyExA(
		HKEY_CURRENT_USER,
		"WindowsAPIMonitor/Credentials/ClientUUID",
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&uuidKey,
		&keyDisposition
	);

	registryOpenResult = RegCreateKeyExA(
		HKEY_CURRENT_USER,
		"WindowsAPIMonitor/Credentials/ClientToken",
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&tokenKey,
		&keyDisposition
	);

	//No credentials, prompt for new credentials
	if (keyDisposition == REG_CREATED_NEW_KEY)
	{
		bool valid = false;
		while (!valid)
		{
			std::cout << "First time setup\n";
			std::cout << "Enter client UUID >> ";
			std::cin >> enteredUUID;

			std::cout << "Enter API token >> ";
			std::cin >> enteredToken;

			//Verify credentials with server
			std::cout << "Validating credentials with server...\n";
			//Add function for making a http request to server later
			valid = true;
		}
		//Set value of registry keys
		const char* uuid = enteredUUID.c_str();
		const char* token = enteredToken.c_str();
		RegSetValueExA(
			uuidKey,
			NULL,
			0,
			REG_SZ,
			(LPBYTE)uuid,
			strlen(uuid)
		);
		RegSetValueExA(
			tokenKey,
			NULL,
			0,
			REG_SZ,
			(LPBYTE)token,
			strlen(token)
		);
	}
	else
	{
		std::cout << "Opened existing key\n";
	}

	
	apiKey = "testKey";
	clientId = "testClientId";
	//Verify key with server

	//Start monitoring
	monitor = new(ProcessMonitor);
	//monitor.scanForProcesses();
	monitor->initialize();
	std::cout << "main thread" << std::endl;


	while (true);
	//Wait for threads to close before exit
	return 0;
}

void testHTTPRequest()
{
	CURL* curl;
	CURLcode response;

	curl = curl_easy_init();

	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "charset: utf-8");

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");
		curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/test");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);
		//char *json = "{\"username\": \"test\", \"email\":\"isaacmnn@gmail.com\", \"password\":\"longpassword\"}";
		char* json = (char*)malloc(200);
		sprintf(json, "{}");
		
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);

		response = curl_easy_perform(curl);
	}
}
