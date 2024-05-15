#pragma once

#include <vector>
#include <thread>

#include "Socket.h"
#include "Log.h"

class Application
{
public:
	Application();
	void Run();
	void Close();

private:
	void SetupListeningSocket();
	void HandleConnections();
	void ProcessClient(Socket Client);

private:
	Socket m_Listening;
	std::thread m_ListeningThread;
	std::vector<std::thread> m_ClientThreads;
	bool bRunning;

};
