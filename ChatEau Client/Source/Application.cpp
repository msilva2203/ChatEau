#include <iostream>

#include "Application.h"
#include "Log.h"

Application::Application() :
	m_Connection(Socket()),
	m_Receiver(),
	m_InputReader()
{
}

void Application::Run(const char* ServerIP)
{
	// 1. Setup connection socket
	SetupConnectionSocket(ServerIP);

	// 2. Create receiver thread
	m_Receiver = std::thread(&Application::Receiver, this);

	// 3. Create input reader thread
	m_InputReader = std::thread(&Application::InputReader, this);

	// 4. Join threads
	m_Receiver.join();
	m_InputReader.join();
}

void Application::Close()
{
	LOG(LOG_INFO, "Closing client application");

}

void Application::SetupConnectionSocket(const char* ServerIP)
{
	int Result;
	sockaddr_in Hint;

	// 1. Create socket
	Result = m_Connection.Create();
	if (Result == INVALID_SOCKET)
	{
		LOG(LOG_ERROR, "Failed to create connection socket");
		exit(EXIT_FAILURE);
	}

	// 2. Setup hint data
	Hint.sin_family = AF_INET;
	Hint.sin_port = htons(PORT);
	inet_pton(AF_INET, ServerIP, &Hint.sin_addr);

	// 3. Connect socket
	Result = m_Connection.Connect(Hint);
	if (Result == SOCKET_ERROR)
	{
		LOG(LOG_ERROR, "Failed to connect socket");
		exit(EXIT_FAILURE);
	}
}

void Application::Receiver()
{
	int Received;
	FMessage Message;

	do
	{
		memset(&Message, 0, MSG_SIZE);
		Received = m_Connection.Receive(reinterpret_cast<char*>(&Message), MSG_SIZE);
		if (Received > 0)
		{
			std::cout << Message.Name << "> " << Message.Message << std::endl;
			std::cout << ">";
		}

	} while (Received >= 0);

	m_Connection.Close();
}

void Application::InputReader()
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
