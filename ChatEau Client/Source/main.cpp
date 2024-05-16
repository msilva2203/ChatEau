#define WIN32_LEAN_AND_MEAN   
#include <windows.h>

#include <iostream>

#include "ClientApplication.h"
#include "Socket.h"
#include "Log.h"

ClientApplication App;

int main(int argc, char** argv)
{
	FConfig Config;
	int Result;

	// 1. Initialize configuration elements
	Config.IP = "127.0.0.1";
	Config.Port = PORT;

	if (argc >= 2)
	{
		Config.IP = argv[1];
	}
	if (argc >= 3)
	{
		Config.Port = atoi(argv[2]);
	}

	LOG(LOG_INFO, "Initializing socket system");

	// 2. Initialize socket system
	Result = Socket::Init();
	if (Result != 0)
	{
		LOG(LOG_ERROR, "Failed to initialize socket system");
		exit(EXIT_FAILURE);
	}

	// 3. Configure and run client application
	App.Configure(Config);
	App.Run();

	LOG(LOG_INFO, "Cleaning socket system");

	// 4. Cleanup socket system
	Result = Socket::Cleanup();
	if (Result != 0)
	{
		LOG(LOG_ERROR, "Failed to cleanup socket system");
		exit(EXIT_FAILURE);
	}

	LOG(LOG_INFO, "Exiting program");

	exit(EXIT_SUCCESS);
}
