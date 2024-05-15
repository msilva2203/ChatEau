#pragma once

#ifdef COMMON_EXPORT
#define COMMON_API __declspec(dllexport)
#else
#define COMMON_API __declspec(dllimport)
#endif

#define MAX_NAME_SIZE   100
#define MAX_BUF_SIZE    4096

#define PORT            9876

struct COMMON_API FMessage
{
	char Message[MAX_BUF_SIZE];
	char Name[MAX_NAME_SIZE];
};

#define MSG_SIZE        sizeof(FMessage)