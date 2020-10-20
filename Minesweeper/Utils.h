#pragma once

#include <Windows.h>
#include <iostream>
#include <sstream>

#define LOG_ERROR(a) Logger::Get().LogError(a)
#define LOG_WARNING(a) Logger::Get().LogWarning(a)
#define LOG_MESSAGE(a) Logger::Get().LogMessage(a)

static void PrintOut(const char* s)
{
	std::wostringstream os_;
	os_ << s;
	OutputDebugStringW(os_.str().c_str());
}

static void DebugOut(const char* file, const int line, const char* s)
{
	std::wostringstream os_;
	os_ << file << "(" << line << "): ";
	os_ << s << std::endl;
	OutputDebugStringW(os_.str().c_str());
}

#define DEBUG_OUT(s) DebugOut(__FILE__, __LINE__, s);
#define PRINT_OUT(s) PrintOut(s);

class Logger
{
	enum Level
	{
		LOG_ERROR, LOG_WARNING, LOG_INFO
	};

public:
	void LogError(const char* error) { Log(error, Level::LOG_ERROR); }
	void LogWarning(const char* warning) { Log(warning, Level::LOG_WARNING); }
	void LogMessage(const char* msg) { Log(msg, Level::LOG_INFO); }

	static Logger& Get()
	{
		static Logger instance;
		return instance;
	}

private:
	Logger() = default;

	void Log(const char* msg, Level level = LOG_INFO)
	{
#if _DEBUG
		switch (level)
		{
			case Level::LOG_ERROR:
			{
				PRINT_OUT("<ERROR> ");
				break;
			}
			case Level::LOG_WARNING:
			{
				PRINT_OUT("<WARNING> ");
				break;
			}
			case Level::LOG_INFO:
			{
				PRINT_OUT("<INFO> ");
				break;
			}
		};

		DEBUG_OUT(msg);
#endif // DEBUG
	}
};

template<typename T>
void SafeDelete(T*& a)
{
	if (a)
	{
		delete a;
		a = nullptr;
	}
}
