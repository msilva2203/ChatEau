#pragma once

#include <thread>

#include "Socket.h"

class Application
{
public:
	Application();

	void Run();
	void Close();

private:
	void SetupConnectionSocket();
	void Receiver();
	void InputReader();

private:
	Socket m_Connection;
	std::thread m_Receiver, m_InputReader;
};