#define CURL_STATICLIB

#include "Main.h"
void testHTTPRequest();

ProcessMonitor* monitor;
int main()
{
	API_ENDPOINT = "http://192.168.0.19:8080";
	//testHTTPRequest();
	//Get API key
	apiKey = "testKey";

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
		curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.0.19:8080/test");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);
		//char *json = "{\"username\": \"test\", \"email\":\"isaacmnn@gmail.com\", \"password\":\"longpassword\"}";
		char* json = (char*)malloc(200);
		sprintf(json, "{}");
		
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);

		response = curl_easy_perform(curl);
	}
}
