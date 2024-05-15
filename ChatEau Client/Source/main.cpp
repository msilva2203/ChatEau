#define WIN32_LEAN_AND_MEAN   
#include <windows.h>

#include <iostream>

#include "Application.h"
#include "Socket.h"
#include "Log.h"

#define BUF_SIZE 4096

Application App;

int main(void)
{
	int Result;

	LOG(LOG_INFO, "Initializing socket system");

	// 1. Initialize socket system
	Result = Socket::Init();
	if (Result != 0)
	{
		LOG(LOG_ERROR, "Failed to initialize socket system");
		exit(EXIT_FAILURE);
	}

	// 2. Run client application
	App.Run();

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
