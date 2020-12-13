#pragma once
#include <Types/String.h>

namespace Utils {
	bool GetOption(int argc, const char* argv[], const String& inOptionName, String& outOptionValue);

	void Log(const String& message);
	void Ensure(bool condition, const String& message);

	bool IsStartingWith(const char* Data, const char* Start);

	bool IsVoidChar(const char Char);
}