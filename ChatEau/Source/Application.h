#pragma once

#include <vector>
#include <thread>

#include "Socket.h"
#include "Log.h"

class Application
{
public:
	Application();
	void Run(const char* ServerIP);
	void Close();
	void BroadcastMessage(FMessage& Message, Socket* SendingClient);

private:
	void SetupListeningSocket(const char* ServerIP);
	void HandleConnections();
	void ProcessClient(Socket* Client, int Id);

private:
	Socket m_Listening;
	std::thread m_ListeningThread;
	std::vector<Socket*> m_Clients;
	std::vector<std::thread> m_ClientThreads;
	bool bRunning;

};
