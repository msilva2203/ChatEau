#pragma once

#include <string>

#ifdef COMMON_EXPORT
#define COMMON_API __declspec(dllexport)
#else
#define COMMON_API __declspec(dllimport)
#endif

#define MAX_NAME_SIZE   100
#define MAX_BUF_SIZE    4096

#define PORT            54000

struct FMessage
{
	char Message[MAX_BUF_SIZE];
	char Name[MAX_NAME_SIZE];
};

struct FConfig
{
	char Name[MAX_NAME_SIZE];
	std::string IP;
	int Port;
};

#define MSG_SIZE        sizeof(FMessage)