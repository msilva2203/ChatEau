#pragma once

#include <vector>
#include <thread>

#include "Application.h"
#include "StreamSocket.h"
#include "Log.h"

class ServerApplication : public Application
{
public:
	ServerApplication();
	virtual void Run() override;
	virtual void Close() override;

	void BroadcastMessage(FMessage& Message, StreamSocket* SendingClient);
	void BroadcastServerMessage(const char* Content, ...);

private:
	int SetupListeningSocket();
	void HandleConnections();
	void ProcessClient(StreamSocket* Client, int Id);

private:
	StreamSocket m_Listening;
	std::thread m_ListeningThread;
	std::vector<StreamSocket*> m_Clients;
	std::vector<std::thread> m_ClientThreads;
	bool bRunning;

};
