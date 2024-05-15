#pragma once

#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

#ifndef _WIN32
typedef SOCKET int;
#endif

class Socket
{
public:
	static int Init();
	static void Cleanup();

public:
	int Create();
	int Bind();
	int Listen();
	int AcceptConnection(Socket* OutConnection);
	int Receive(char* Buffer, int Length);
	int Send(const char* Buffer, int Length);
	int Close();

private:
	SOCKET m_Sock;
	sockaddr_in m_Hint;

};