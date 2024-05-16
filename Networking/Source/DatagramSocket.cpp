#include "DatagramSocket.h"


int DatagramSocket::Create(sockaddr_in& Hint)
{
	m_Hint = Hint;
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	return m_Sock;
}

int DatagramSocket::Receive(char* Buffer, int Length)
{
	return recv(m_Sock, Buffer, Length, 0);
}

int DatagramSocket::Send(const char* Buffer, int Length)
{
	return send(m_Sock, Buffer, Length, 0);
}
