#pragma once

#include <thread>

#include "Socket.h"

class Application
{
public:
	Application();

	void Run(const char* ServerIP);
	void Close();

private:
	void SetupConnectionSocket(const char* ServerIP);
	void Receiver();
	void InputReader();

private:
	Socket m_Connection;
	std::thread m_Receiver, m_InputReader;
};