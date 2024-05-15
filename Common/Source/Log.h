#pragma once

#include <string>
#include <string.h>

#include "Common.h"

#define LOG_BUF_SIZE  4096

class COMMON_API Log
{
public:
	enum class EType : unsigned char
	{
		Info,
		Warning,
		Error
	};

	Log() = delete;
	static void Write(const char* Message, EType Type = EType::Info);

private:
	static std::string TypeStr(EType Type);
};

#define LOG_INFO          Log::EType::Info
#define LOG_WARNING       Log::EType::Warning
#define LOG_ERROR         Log::EType::Error

#define LOG(Type, Message, ...) \
{ \
	char _b[LOG_BUF_SIZE]; \
	snprintf(_b, LOG_BUF_SIZE, Message, __VA_ARGS__); \
	Log::Write(_b, Type); \
} \
