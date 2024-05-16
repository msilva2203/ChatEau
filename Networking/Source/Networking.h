#pragma once

#ifdef NETWORKING_EXPORT
#define NETWORKING_API __declspec(dllexport)
#else
#define NETWORKING_API __declspec(dllimport)
#endif
