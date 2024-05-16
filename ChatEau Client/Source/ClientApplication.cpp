#include <iostream>

#include "ClientApplication.h"
#include "Log.h"

ClientApplication::ClientApplication() :
	m_Connection(StreamSocket()),
	m_Receiver(),
	m_InputReader()
{
}

void ClientApplication::Run()
{
	// 1. Setup connection socket
	SetupConnectionSocket();

	// 2. Create receiver thread
	m_Receiver = std::thread(&ClientApplication::Receiver, this);

	// 3. Create input reader thread
	m_InputReader = std::thread(&ClientApplication::InputReader, this);

	// 4. Join threads
	m_Receiver.join();
	m_InputReader.join();
}

void ClientApplication::Close()
{
	LOG(LOG_INFO, "Closing client application");

}

void ClientApplication::SetupConnectionSocket()
{
	int Result;
	sockaddr_in Hint;

	// 1. Setup hint data
	Hint.sin_family = AF_INET;
	Hint.sin_port = htons(m_Config.Port);
	inet_pton(AF_INET, m_Config.IP.c_str(), &Hint.sin_addr);

	// 2. Create socket
	Result = m_Connection.Create(Hint);
	if (Result == INVALID_SOCKET)
	{
		LOG(LOG_ERROR, "Failed to create connection socket");
		exit(EXIT_FAILURE);
	}

	// 3. Connect socket
	Result = m_Connection.Connect();
	if (Result == SOCKET_ERROR)
	{
		LOG(LOG_ERROR, "Failed to connect socket");
		exit(EXIT_FAILURE);
	}
}

void ClientApplication::Receiver()
{
	int Received;
	FMessage Message;

	do
	{
		memset(&Message, 0, MSG_SIZE);
		Received = m_Connection.Receive(reinterpret_cast<char*>(&Message), MSG_SIZE);
		if (Received > 0)
		{
			std::cout << Message.Name << ": " << Message.Message << std::endl;
			std::cout << ">";
		}

	} while (Received >= 0);

	m_Connection.Close();
}

void ClientApplication::InputReader()
{
	int Result;
	std::string Input;

	while (true)
	{
		std::cout << ">";
		std::getline(std::cin, Input);

		if (Input.size() > 0)
		{
			Result = m_Connection.Send(Input.c_str(), Input.size() + 1);
			if (Result == SOCKET_ERROR)
			{
				LOG(LOG_ERROR, "Failed to send message");
				break;
			}
		}
	}
	m_Connection.Close();
}
