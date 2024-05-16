#include "Socket.h"

Socket::Socket() :
	m_Sock(0)
{
	memset(&m_Hint, 0, sizeof(m_Hint));
}

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

int Socket::Bind()
{
	return bind(m_Sock, (sockaddr*)&m_Hint, sizeof(m_Hint));
}

int Socket::Close()
{
#ifdef _WIN32
	return closesocket(m_Sock);
#else
	return close(m_Sock);
#endif
}