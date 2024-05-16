#pragma once

#include <WS2tcpip.h>
#include <string>

#include "Networking.h"

#pragma comment (lib, "ws2_32.lib")

#ifndef _WIN32
typedef int SOCKET;
#endif

class NETWORKING_API Socket
{
protected:
	Socket();

public:
	static int Init();
	static int Cleanup();

public:
	virtual int Create(sockaddr_in& Hint) = 0;
	virtual int Receive(char* Buffer, int Length) = 0;
	virtual int Send(const char* Buffer, int Length) = 0;
	int Bind();
	int Close();

protected:
	SOCKET m_Sock;
	sockaddr_in m_Hint;

};