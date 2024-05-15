#include <iostream>

#include "Application.h"

#define BUF_SIZE 4096

Application::Application() :
	m_Listening(Socket()),
	m_ListeningThread(std::thread()),
	m_ClientThreads(),
	bRunning(true)
{
}

void Application::Run()
{
	LOG(LOG_INFO, "Starting Application");

	// 1. Setup listening socket
	SetupListeningSocket();

	// 2. Accept client connections
	m_ListeningThread = std::thread(&Application::HandleConnections, this);
	m_ListeningThread.join();

	// 3. Close listening socket
	m_Listening.Close();

	// 4. Clear application
	for (auto& Thread : m_ClientThreads)
	{
		Thread.join();
	}
}

void Application::Close()
{
	LOG(LOG_INFO, "Closing Application");
	bRunning = false;
}

void Application::SetupListeningSocket()
{
	int Result;

	// 1. Create socket
	Result = m_Listening.Create();
	if (Result == INVALID_SOCKET)
	{
		LOG(LOG_ERROR, "Failed to create listening socket");
		return;
	}

	// 2. Bind socket
	Result = m_Listening.Bind();

	// 3. Make socket listen
	Result = m_Listening.Listen();
}

void Application::HandleConnections()
{
	Socket Client;
	int Result;

	LOG(LOG_INFO, "Listening for client connections");

	while (bRunning)
	{
		Result = m_Listening.AcceptConnection(&Client);
		if (Result == INVALID_SOCKET)
		{
			LOG(LOG_ERROR, "Invalid client socket");
			break;
		}
		m_ClientThreads.push_back(std::thread(&Application::ProcessClient, this, Client));
	}

	LOG(LOG_INFO, "Stopped listening for client connections");
	bRunning = false;
}

void Application::ProcessClient(Socket Client)
{
	char Buf[BUF_SIZE];

	LOG(LOG_INFO, "Processing new client");

	while (bRunning)
	{
		// 1. Zero the message buffer
		memset(Buf, 0, BUF_SIZE);

		// 2. Wait for client message
		int Received = Client.Receive(Buf, BUF_SIZE);
		if (Received == SOCKET_ERROR)
		{
			LOG(LOG_ERROR, "Failed to receive client message");
			break;
		}
		if (Received == 0) break;

		// 3. Process message
		if (Buf[strlen(Buf) - 1] == '\n') Buf[strlen(Buf) - 1] = '\0';

		std::cout << Buf << std::endl;

		// 4. Send message to other clients
		Client.Send(Buf, Received + 1);
	}

	// 5. Close client socket
	Client.Close();
	LOG(LOG_INFO, "Lost connection to client");
}
