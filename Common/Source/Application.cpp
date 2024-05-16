#include "Application.h"

Application::Application() :
	m_Config()
{
}

void Application::Configure(FConfig& Config)
{
	m_Config = Config;
}
