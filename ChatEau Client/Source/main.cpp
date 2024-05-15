#define WIN32_LEAN_AND_MEAN   
#include <windows.h>

#include <iostream>

#include "Socket.h"
#include "Log.h"

#define BUF_SIZE 4096

int main(void)
{
	int Result;
	sockaddr_in Hint;
	Socket Connection;

	LOG(LOG_INFO, "Starting Client");

	Socket::Init();

	Result = Connection.Create();
	if (Result == INVALID_SOCKET)
	{
		LOG(LOG_ERROR, "Failed to create connection socket");
		std::cin.get();
		exit(EXIT_FAILURE);
	}

	Hint.sin_family = AF_INET;
	Hint.sin_port = htons(54000);
	inet_pton(AF_INET, "127.0.0.1", &Hint.sin_addr);
	Result = Connection.Connect(Hint);
	if (Result == SOCKET_ERROR)
	{
		LOG(LOG_ERROR, "Failed to connect socket");
		std::cin.get();
		exit(EXIT_FAILURE);
	}

	char Buf[BUF_SIZE];
	std::string Input;

	do
	{
		std::cout << ">";
		std::getline(std::cin, Input);

		if (Input.size() > 0)
		{
			Connection.Send(Input.c_str(), Input.size() + 1);

			memset(Buf, 0, BUF_SIZE);
			int Received = Connection.Receive(Buf, BUF_SIZE);
			if (Received <= 0)
			{
				LOG(LOG_ERROR, "Lost connection to server");
				std::cin.get();
				break;
			}
			if (Received > 0)
			{
				std::cout << "SERVER> " << Buf << std::endl;
			}
		}
	} while (Input.size() > 0);

	Connection.Close();

	Socket::Cleanup();

	exit(EXIT_SUCCESS);
}
