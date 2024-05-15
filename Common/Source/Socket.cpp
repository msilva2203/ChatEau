#include "Socket.h"

int Socket::Init()
{
	WSADATA WsaData;
	WORD Version = MAKEWORD(2, 2);
	int Result = WSAStartup(Version, &WsaData);
	return Result;
}

int Socket::Cleanup()
{
	return WSACleanup();
}

int Socket::Create()
{
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	return m_Sock;
}

int Socket::Bind(sockaddr_in Hint)
{
	m_Hint = Hint;
	return bind(m_Sock, (sockaddr*)&m_Hint, sizeof(m_Hint));
}

int Socket::Connect(sockaddr_in Hint)
{
	m_Hint = Hint;
	return connect(m_Sock, (sockaddr*)&m_Hint, sizeof(m_Hint));
}

int Socket::Listen()
{
	return listen(m_Sock, SOMAXCONN);
}

int Socket::AcceptConnection(Socket* OutConnection)
{
	int Size = sizeof(OutConnection->m_Hint);
	OutConnection->m_Sock = accept(m_Sock, (sockaddr*)&OutConnection->m_Hint, &Size);
	return OutConnection->m_Sock;
}

int Socket::Receive(char* Buffer, int Length)
{
	return recv(m_Sock, Buffer, Length, 0);
}

int Socket::Send(const char* Buffer, int Length)
{
	return send(m_Sock, Buffer, Length, 0);
}

int Socket::Close()
{
#ifdef _WIN32
	return closesocket(m_Sock);
#else
	return close(m_Sock);
#endif
}