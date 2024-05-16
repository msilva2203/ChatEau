#include <iostream>

#include "ServerApplication.h"


ServerApplication::ServerApplication() :
	m_Listening(StreamSocket()),
	m_ListeningThread(std::thread()),
	m_Clients(),
	m_ClientThreads(),
	bRunning(true)
{
}

void ServerApplication::Run()
{
	LOG(LOG_INFO, "Starting Application");

	// 1. Setup listening socket
	if (SetupListeningSocket() < 0)
		return;

	// 2. Accept client connections
	m_ListeningThread = std::thread(&ServerApplication::HandleConnections, this);
	m_ListeningThread.join();

	// 3. Close listening socket
	m_Listening.Close();

	// 4. Clear application
	for (auto& Thread : m_ClientThreads)
	{
		Thread.join();
	}
}

void ServerApplication::Close()
{
	LOG(LOG_INFO, "Closing Application");
	bRunning = false;
}

void ServerApplication::BroadcastMessage(FMessage& Message, StreamSocket* SendingClient)
{
	for (auto& ChatClient : m_Clients)
	{
		if (ChatClient != SendingClient)
			ChatClient->Send(reinterpret_cast<char*>(&Message), MSG_SIZE);
	}
}

void ServerApplication::BroadcastServerMessage(const char* Content, ...)
{
	FMessage Message;

	va_list Args;
	va_start(Args, Content);

	memset(&Message, 0, MSG_SIZE);
	snprintf(Message.Name, MAX_NAME_SIZE, "@%s", m_Config.Name);
	snprintf(Message.Message, MAX_BUF_SIZE, Content, Args);

	va_end(Args);

	BroadcastMessage(Message, nullptr);
}

int ServerApplication::SetupListeningSocket()
{
	int Result;
	sockaddr_in Hint;

	// 1. Setup hint data
	Hint.sin_family = AF_INET;
	Hint.sin_port = htons(m_Config.Port);
	Hint.sin_addr.S_un.S_addr = INADDR_ANY;
	//inet_pton(AF_INET, m_Config.IP.c_str(), &Hint.sin_addr);

	// 2. Create socket
	Result = m_Listening.Create(Hint);
	if (Result == INVALID_SOCKET)
	{
		LOG(LOG_ERROR, "Failed to create listening socket");
		return -1;
	}
	// 3. Bind socket
	Result = m_Listening.Bind();
	if (Result == SOCKET_ERROR)
	{
		LOG(LOG_ERROR, "Failed to bind listening socket");
		return -1;
	}
	// 4. Make socket listen
	Result = m_Listening.Listen();
	if (Result == SOCKET_ERROR)
	{
		LOG(LOG_ERROR, "Failed to listen socket");
		return -1;
	}

	LOG(LOG_INFO, "Socket created with IP address: %s", m_Config.IP.c_str());
	return 0;
}

void ServerApplication::HandleConnections()
{
	StreamSocket* NewClient;
	int Result;

	LOG(LOG_INFO, "Listening for client connections");

	while (bRunning)
	{
		// 1. Create new client socket
		m_Clients.push_back(new StreamSocket());
		NewClient = m_Clients[m_Clients.size() - 1];

		// 2. Assign new connection to last client
		Result = m_Listening.AcceptConnection(NewClient);
		if (Result == INVALID_SOCKET)
		{
			LOG(LOG_ERROR, "Invalid client socket");
			break;
		}

		// 3. Create thread to process new client
		m_ClientThreads.push_back(std::thread(&ServerApplication::ProcessClient, this, NewClient, (int)m_Clients.size()));
	}

	LOG(LOG_INFO, "Stopped listening for client connections");
	bRunning = false;
}

void ServerApplication::ProcessClient(StreamSocket* Client, int Id)
{
	FMessage Message;
	char Buf[MAX_BUF_SIZE];

	LOG(LOG_INFO, "Processing new client");

	BroadcastServerMessage("Client %d joined chat session!", Id);
	std::cout << Id << std::endl;

	while (bRunning)
	{
		// 1. Zero the message buffer
		memset(Buf, 0, MAX_BUF_SIZE);
		memset(&Message, 0, MSG_SIZE);

		// 2. Wait for client message
		int Received = Client->Receive(Buf, MAX_BUF_SIZE);
		if (Received == SOCKET_ERROR)
		{
			LOG(LOG_ERROR, "Failed to receive client message");
			break;
		}
		if (Received == 0) break;

		// 3. Process message
		while ((Buf[strlen(Buf) - 1] == '\n')) Buf[strlen(Buf) - 1] = '\0';
		snprintf(Message.Name, MAX_NAME_SIZE, "@client %d", Id); // Name
		strcpy_s(Message.Message, Buf); // Message

		std::cout << Buf << std::endl;

		// 4. Send message to other clients
		BroadcastMessage(Message, Client);
	}

	BroadcastServerMessage("Client %d left the session!", Id);

	// 5. Close client socket
	Client->Close();
	LOG(LOG_INFO, "Lost connection to client");
}
