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
	void BroadcastMessage(FMessage& Message, Socket* SendingClient);

private:
	void SetupListeningSocket();
	void HandleConnections();
	void ProcessClient(Socket* Client, int Id);

private:
	Socket m_Listening;
	std::thread m_ListeningThread;
	std::vector<Socket*> m_Clients;
	std::vector<std::thread> m_ClientThreads;
	bool bRunning;

};
