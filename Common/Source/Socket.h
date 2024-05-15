#pragma once

#include <WS2tcpip.h>
#include <string>

#include "Common.h"

#pragma comment (lib, "ws2_32.lib")

#ifndef _WIN32
typedef SOCKET int;
#endif

class COMMON_API Socket
{
public:
	static int Init();
	static int Cleanup();

public:
	int Create();
	int Bind(sockaddr_in Hint);
	int Connect(sockaddr_in Hint);
	int Listen();
	int AcceptConnection(Socket* OutConnection);
	int Receive(char* Buffer, int Length);
	int Send(const char* Buffer, int Length);
	int Close();

private:
	SOCKET m_Sock;
	sockaddr_in m_Hint;

};