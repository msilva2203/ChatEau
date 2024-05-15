#define WIN32_LEAN_AND_MEAN   
#include <windows.h>

#include <iostream>

#include "Application.h"
#include "Socket.h"
#include "Log.h"

Application App;

int main(int argc, char** argv)
{
	std::string ServerIP = "127.0.0.1";
	int Result;

	if (argc >= 2)
	{
		ServerIP = argv[1];
	}

	LOG(LOG_INFO, "Initializing socket system");

	// 1. Initialize socket system
	Result = Socket::Init();
	if (Result != 0)
	{
		LOG(LOG_ERROR, "Failed to initialize socket system");
		exit(EXIT_FAILURE);
	}

	// 2. Run client application
	LOG(LOG_INFO, "Using IP: %s", ServerIP.c_str());
	App.Run(ServerIP.c_str());

	LOG(LOG_INFO, "Cleaning socket system");

	// 3. Cleanup socket system
	Result = Socket::Cleanup();
	if (Result != 0)
	{
		LOG(LOG_ERROR, "Failed to cleanup socket system");
		exit(EXIT_FAILURE);
	}

	LOG(LOG_INFO, "Exiting program");

	exit(EXIT_SUCCESS);
}
