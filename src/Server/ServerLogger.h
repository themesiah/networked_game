#pragma once

#ifndef H_SERVER_LOGGER
#define H_SERVER_LOGGER

#include <Utils\Singleton.h>
#include <iostream>
#include <Windows.h>

#define LOGGER CServerLogger::GetInstance()

#define PRINT(color) SetConsoleTextAttribute(hConsole, color);\
va_list argptr;\
va_start(argptr, format);\
vfprintf(stderr, format, argptr);\
va_end(argptr);\
printf("\n");\

class CServerLogger : public base::utils::CSingleton<CServerLogger> {
public:
	enum LoggerColors {
		GREEN = 10,
		BLUE,
		RED,
		PURPLE,
		YELLOW,
		WHITE
	};

	void Info(const char* format, ...) {
		PRINT(LoggerColors::WHITE);
	}

	void Log(const char* format, ...) {
		PRINT(LoggerColors::WHITE);
	}

	void Success(const char* format, ...) {
		PRINT(LoggerColors::GREEN);
	}

	void Warning(const char* format, ...) {
		PRINT(LoggerColors::YELLOW);
	}

	void Error(const char* format, ...) {
		PRINT(LoggerColors::RED);
	}

	void ProgressMessage(int current, int max, bool finished, LoggerColors color, const char* format, ...) {
		SetConsoleTextAttribute(hConsole, color);
		va_list argptr;
		va_start(argptr, format);
		printf("[%d/%d] ", current, max);
		vfprintf(stderr, format, argptr);
		va_end(argptr);
		if (finished) {
			printf("\n");
		}
		else {
			printf("\r");
		}
	}
protected:
	CServerLogger() {
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	friend class base::utils::CSingleton<CServerLogger>;
private:
	HANDLE hConsole;
};
#endif