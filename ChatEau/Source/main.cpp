#define WIN32_LEAN_AND_MEAN   
#include <windows.h>

#include "Application.h"
#include "Log.h"

BOOL WINAPI ConsoleHandler(DWORD);

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

	// 2. Capture CTRL-C signal
	//if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE)) {
	//	LOG(LOG_ERROR, "Failed to install console handler");
	//	exit(EXIT_FAILURE);
	//}

	// 3. Run application
	App.Run(ServerIP.c_str());

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

BOOL WINAPI ConsoleHandler(DWORD Type)
{
	switch (Type)
	{
	case CTRL_C_EVENT:
		LOG(LOG_INFO, "CTRL-C captured");
		App.Close();
		break;
	default:
		break;
	}
	return TRUE;
}
