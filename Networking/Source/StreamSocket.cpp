#include "StreamSocket.h"


int StreamSocket::Create(sockaddr_in& Hint)
{
	m_Hint = Hint;
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	return m_Sock;
}

int StreamSocket::Receive(char* Buffer, int Length)
{
	return recv(m_Sock, Buffer, Length, 0);
}

int StreamSocket::Send(const char* Buffer, int Length)
{
	return send(m_Sock, Buffer, Length, 0);
}

int StreamSocket::Connect()
{
	return connect(m_Sock, (sockaddr*)&m_Hint, sizeof(m_Hint));
}

int StreamSocket::Listen(int Backlog)
{
	return listen(m_Sock, Backlog);
}

int StreamSocket::AcceptConnection(StreamSocket* OutConnection)
{
	if (!OutConnection) return -1;

	int Size = sizeof(OutConnection->m_Hint);
	OutConnection->m_Sock = accept(m_Sock, (sockaddr*)&OutConnection->m_Hint, &Size);
	return OutConnection->m_Sock;
}
