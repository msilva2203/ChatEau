#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

#include "Log.h"

void Log::Write(const char* Message, EType Type)
{
	auto Now = std::chrono::system_clock::now();
	std::time_t Time = std::chrono::system_clock::to_time_t(Now);
	
	struct tm Local;
	localtime_s(&Local, &Time);

	std::cout << std::setw(2) << std::setfill('0') << Local.tm_hour << ":";
	std::cout << std::setw(2) << std::setfill('0') << Local.tm_min << ":";
	std::cout << std::setw(2) << std::setfill('0') << Local.tm_sec << " [";
	std::cout << TypeStr(Type) << "] ";
	std::cout << Message << std::endl;
}

std::string Log::TypeStr(EType Type)
{
	switch (Type)
	{
	case EType::Info:
		return "INFO";
	case EType::Warning:
		return "WARNING";
	case EType::Error:
		return "ERROR";
	default:
		return std::string();
	}
}