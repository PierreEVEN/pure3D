#pragma once
#include <Types/String.h>
#include <filesystem>

namespace Utils {
	bool GetOption(int argc, const char* argv[], const String& inOptionName, String& outOptionValue);

	void Log(const String& message);
	void Ensure(bool condition, const String& message);

	bool IsStartingWith(const char* Data, const char* Start);

	bool IsVoidChar(const char Char);

	bool IsTypeArray(const String& inTypeName);
	String GetTemplateInnerType(const String& inTypeName);

	inline static bool PHT_DEBUG_MODE;
	size_t GenURID();

    void SourceToIntermediate(String& OutPublic, String& OutPrivate, const std::filesystem::path& SourcePath, String IntermediatesDir);
}