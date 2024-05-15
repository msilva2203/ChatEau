#include <iostream>

#include "Socket.h"
#include "Log.h"

#define BUF_SIZE 4096

int main(void)
{
	int Result;
	Socket ListeningSocket;
	Socket ClientSocket;

	LOG(LOG_INFO, "Initializing Application");

	Result = Socket::Init();
	if (Result != 0)
	{
		LOG(LOG_ERROR, "Failed to initialize socket system");
		exit(EXIT_FAILURE);
	}

	Result = ListeningSocket.Create();
	if (Result == INVALID_SOCKET)
	{
		LOG(LOG_ERROR, "Failed to create listening socket");
		exit(EXIT_FAILURE);
	}

	Result = ListeningSocket.Bind();
	Result = ListeningSocket.Listen();

	ListeningSocket.AcceptConnection(&ClientSocket);
	LOG(LOG_INFO, "Accepted connection!");

	ListeningSocket.Close();

	while (true)
	{
		char Buf[BUF_SIZE];
		memset(Buf, 0, BUF_SIZE);

		int Received = ClientSocket.Receive(Buf, BUF_SIZE);
		if (Received == SOCKET_ERROR)
		{
			LOG(LOG_ERROR, "Failed to receive client message");
			break;
		}
		if (Received == 0) break;
		if (Buf[strlen(Buf) - 1] == '\n') Buf[strlen(Buf) - 1] = '\0';

		std::cout << Buf << std::endl;

		ClientSocket.Send(Buf, Received + 1);
	}

	ClientSocket.Close();
	LOG(LOG_INFO, "Lost connection to client!");

	Socket::Cleanup();

	exit(EXIT_SUCCESS);
}
