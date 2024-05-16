#pragma once

#include "Socket.h"

class NETWORKING_API StreamSocket : public Socket
{
public:

	virtual int Create(sockaddr_in& Hint) override;
	virtual int Receive(char* Buffer, int Length) override;
	virtual int Send(const char* Buffer, int Length) override;
	int Connect();
	int Listen(int Backlog = SOMAXCONN);
	int AcceptConnection(StreamSocket* OutConnection);

private:

};