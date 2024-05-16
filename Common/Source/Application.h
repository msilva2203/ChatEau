#pragma once

#include "Common.h"

class COMMON_API Application
{
public:
	Application();
	virtual void Run() = 0;
	virtual void Close() = 0;
	void Configure(FConfig& Config);

protected:
	FConfig m_Config;

};