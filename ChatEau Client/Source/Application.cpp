#include <iostream>

#include "Application.h"
#include "Log.h"

#define BUF_SIZE 4096

Application::Application() :
	m_Connection(Socket()),
	m_Receiver(),
	m_InputReader()
{
}

void Application::Run()
{
	// 1. Setup connection socket
	SetupConnectionSocket();

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

void Application::SetupConnectionSocket()
{
	int Result;
	sockaddr_in Hint;

	// 1. Create socket
	Result = m_Connection.Create();
	if (Result == INVALID_SOCKET)
	{
		LOG(LOG_ERROR, "Failed to create connection socket");
		std::cin.get();
		exit(EXIT_FAILURE);
	}

	// 2. Setup hint data
	Hint.sin_family = AF_INET;
	Hint.sin_port = htons(54000);
	inet_pton(AF_INET, "127.0.0.1", &Hint.sin_addr);

	// 3. Connect socket
	Result = m_Connection.Connect(Hint);
	if (Result == SOCKET_ERROR)
	{
		LOG(LOG_ERROR, "Failed to connect socket");
		std::cin.get();
		exit(EXIT_FAILURE);
	}
}

void Application::Receiver()
{
	int Received;
	char Buf[BUF_SIZE];

	do
	{
		memset(Buf, 0, BUF_SIZE);
		Received = m_Connection.Receive(Buf, BUF_SIZE);
		if (Received > 0)
		{
			std::cout << "SERVER> " << Buf << std::endl;
			std::cout << ">";
		}

	} while (Received >= 0);

	m_Connection.Close();
	m_InputReader.detach();
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
