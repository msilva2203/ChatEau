#pragma once

#include <thread>

#include "Application.h"
#include "StreamSocket.h"

class ClientApplication : public Application
{
public:
	ClientApplication();
	virtual void Run() override;
	virtual void Close() override;

private:
	void SetupConnectionSocket();
	void Receiver();
	void InputReader();

private:
	StreamSocket m_Connection;
	std::thread m_Receiver, m_InputReader;
};