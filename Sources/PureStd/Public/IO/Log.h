#pragma once

#include "EngineIO.h"
#include "Types/String.h"

#ifndef __FUNCTION_NAME__
#ifdef WIN32   //WINDOWS
#define __FUNCTION_NAME__   __FUNCTION__
#else          //*NIX
#define __FUNCTION_NAME__   __func__
#endif
#endif

#if _WIN32
	#define LOG(text, ...) Logger::LogDetailed(String::Format(text, __VA_ARGS__), LogVerbosity::VERB_DISPLAY, __LINE__, __FUNCTION_NAME__)
	#define LOG_ERROR(text, ...) Logger::LogDetailed(String::Format(text, __VA_ARGS__), LogVerbosity::VERB_ERROR, __LINE__, __FUNCTION_NAME__)
	#define LOG_WARNING(text, ...) Logger::LogDetailed(String::Format(text, __VA_ARGS__), LogVerbosity::VERB_WARNING, __LINE__, __FUNCTION_NAME__)
	#if _DEBUG
		#define LOG_ASSERT(text, ...) { Logger::LogDetailedFull(String::Format(text, __VA_ARGS__), LogVerbosity::VERB_ASSERT, __LINE__, __FUNCTION_NAME__, __FILE__); __debugbreak(); exit(1); }
	#else
		#define LOG_ASSERT(text, ...) { Logger::LogDetailedFull(String::Format(text, __VA_ARGS__), LogVerbosity::VERB_ASSERT, __LINE__, __FUNCTION_NAME__, __FILE__); exit(1); }
	#endif
#else
	#define LOG(text, ...) Logger::LogDetailed(String::Format(text __VA_OPT__(,) __VA_ARGS__), LogVerbosity::VERB_DISPLAY, __LINE__, __FUNCTION_NAME__)
	#define LOG_ERROR(text, ...) Logger::LogDetailed(String::Format(text __VA_OPT__(,) __VA_ARGS__), LogVerbosity::VERB_ERROR, __LINE__, __FUNCTION_NAME__)
	#define LOG_WARNING(text, ...) Logger::LogDetailed(String::Format(text __VA_OPT__(,) __VA_ARGS__), LogVerbosity::VERB_WARNING, __LINE__, __FUNCTION_NAME__)
	#if _DEBUG
		#define LOG_ASSERT(text, ...) { Logger::LogDetailedFull(String::Format(text __VA_OPT__(,) __VA_ARGS__), LogVerbosity::VERB_ASSERT, __LINE__, __FUNCTION_NAME__, __FILE__); __debugbreak(); exit(1); }
	#else
		#define LOG_ASSERT(text, ...) { Logger::LogDetailedFull(String::Format(text __VA_OPT__(,) __VA_ARGS__), LogVerbosity::VERB_ASSERT, __LINE__, __FUNCTION_NAME__, __FILE__); exit(1); }
	#endif
#endif

enum class LogVerbosity
{
	VERB_DISPLAY = 0,
	VERB_WARNING = 1,
	VERB_ERROR = 2,
	VERB_ASSERT = 3
};

class Logger
{
public:

	static void Log(const String& logText, LogVerbosity verbosity);

	static void LogDetailed(const String& logText, LogVerbosity verbosity, const int& line, String fct);

	static void LogDetailedFull(const String& logText, LogVerbosity verbosity, const int& line, String fct, String File)
	{
		Log("(" + fct + ":" + line + ") : " + logText + String::ENDL + File, verbosity);
	}

};