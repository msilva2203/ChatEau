#include <iostream>

#include "Application.h"

#define BUF_SIZE 4096

Application::Application() :
	m_Listening(Socket()),
	m_ListeningThread(std::thread()),
	m_Clients(),
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

void Application::BroadcastMessage(FMessage& Message, Socket* SendingClient)
{
	for (auto& ChatClient : m_Clients)
	{
		if (ChatClient != SendingClient)
			ChatClient->Send(reinterpret_cast<char*>(&Message), MSG_SIZE);
	}
}

void Application::SetupListeningSocket()
{
	int Result;
	sockaddr_in Hint;

	// 1. Create socket
	Result = m_Listening.Create();
	if (Result == INVALID_SOCKET)
	{
		LOG(LOG_ERROR, "Failed to create listening socket");
		return;
	}

	// 2. Setup hint data
	Hint.sin_family = AF_INET;
	Hint.sin_port = htons(PORT);
	Hint.sin_addr.S_un.S_addr = INADDR_ANY;

	// 3. Bind socket
	Result = m_Listening.Bind(Hint);

	// 4. Make socket listen
	Result = m_Listening.Listen();
}

void Application::HandleConnections()
{
	Socket* NewClient;
	int Result;

	LOG(LOG_INFO, "Listening for client connections");

	while (bRunning)
	{
		// 1. Create new client socket
		m_Clients.push_back(new Socket());
		NewClient = m_Clients[m_Clients.size() - 1];

		// 2. Assign cnew connection to last client
		Result = m_Listening.AcceptConnection(NewClient);
		if (Result == INVALID_SOCKET)
		{
			LOG(LOG_ERROR, "Invalid client socket");
			break;
		}

		// 3. Create thread to process new client
		m_ClientThreads.push_back(std::thread(&Application::ProcessClient, this, NewClient, (int)m_Clients.size()));
	}

	LOG(LOG_INFO, "Stopped listening for client connections");
	bRunning = false;
}

void Application::ProcessClient(Socket* Client, int Id)
{
	FMessage Message;
	char Buf[MAX_BUF_SIZE];

	LOG(LOG_INFO, "Processing new client");

	memset(&Message, 0, MSG_SIZE);
	strcpy_s(Message.Name, "SERVER");
	snprintf(Message.Message, MAX_BUF_SIZE, "Client %d joined chat session!", Id);
	BroadcastMessage(Message, Client);

	while (bRunning)
	{
		// 1. Zero the message buffer
		memset(Buf, 0, BUF_SIZE);
		memset(&Message, 0, MSG_SIZE);

		// 2. Wait for client message
		int Received = Client->Receive(Buf, BUF_SIZE);
		if (Received == SOCKET_ERROR)
		{
			LOG(LOG_ERROR, "Failed to receive client message");
			break;
		}
		if (Received == 0) break;

		// 3. Process message
		while ((Buf[strlen(Buf) - 1] == '\n')) Buf[strlen(Buf) - 1] = '\0';
		snprintf(Message.Name, MAX_NAME_SIZE, "CLIENT %d", Id); // Name
		strcpy_s(Message.Message, Buf); // Message

		std::cout << Buf << std::endl;

		// 4. Send message to other clients
		BroadcastMessage(Message, Client);
	}

	memset(&Message, 0, MSG_SIZE);
	strcpy_s(Message.Name, "SERVER");
	snprintf(Message.Message, MAX_BUF_SIZE, "Client %d left the session!", Id);
	BroadcastMessage(Message, Client);

	// 5. Close client socket
	Client->Close();
	LOG(LOG_INFO, "Lost connection to client");
}
