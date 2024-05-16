#define WIN32_LEAN_AND_MEAN   
#include <windows.h>

#include "ServerApplication.h"
#include "Log.h"


#define SERVER_NAME   "server"

BOOL WINAPI ConsoleHandler(DWORD);

ServerApplication App;

int main(int argc, char** argv)
{
	FConfig Config;
	int Result;

	// 1. Initialize configuration elements
	Config.IP = "127.0.0.1";
	Config.Port = PORT;
	strcpy_s(Config.Name, SERVER_NAME);

	if (argc >= 2)
	{
		strcpy_s(Config.Name, argv[1]);
	}
	if (argc >= 3)
	{
		Config.IP = argv[2];
	}
	if (argc >= 4)
	{
		Config.Port = atoi(argv[3]);
	}

	LOG(LOG_INFO, "Initializing socket system");

	// 2. Initialize socket system
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

	// 3. Configure and run the server application
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
