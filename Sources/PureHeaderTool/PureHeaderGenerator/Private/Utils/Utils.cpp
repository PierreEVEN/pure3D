#include "Utils/Utils.h"
#include <IO/Log.h>
#include <iostream>
#include <filesystem>

bool Utils::GetOption(int argc, const char* argv[], const String& inOptionName, String& outOptionValue) {
	for (int i = 1; i < argc; ++i) {
		String Option;
		String Value;

		String::SplitString(String(argv[i]), { '=' }, Option, Value);

		if (Option == ('-' + inOptionName)) {
			outOptionValue = Value;
			return true;
		}
	}
	return false;
}

void Utils::Log(const String& message) {
	std::cout << "[PHT] - " << message.GetData() << std::endl;
}

void Utils::Ensure(bool condition, const String& message) {
	if (!condition) {
		Log(message);
		exit(EXIT_FAILURE);
	}
}


bool Utils::IsStartingWith(const char* Data, const char* Start) {
	for (int i = 0; Start[i] != '\0'; ++i) {
		if (Data[i] == '\0' || Data[i] != Start[i]) return false;
	}
	return true;
}

bool Utils::IsVoidChar(const char Char) {
	return Char == ' ' || Char == '\n' || Char == '\t' || Char == '\r';
}

bool Utils::IsTypeArray(const String& inTypeName)
{
	return inTypeName.IsStartingWith("std::vector<");
}

String Utils::GetTemplateInnerType(const String& inTypeName) {
	String Left;
	String Center;
	String Right;
	String::SplitString(inTypeName, { '<' }, Left, Center);
	String::SplitString(Center, { '>' }, Left, Right, false);
	return Left;
}

inline size_t REFLECTION_UNIQUE_ID = 0;

size_t Utils::GenURID()
{
	return REFLECTION_UNIQUE_ID++;
}

void Utils::SourceToIntermediate(String& OutPublic, String& OutPrivate, const std::filesystem::path& SourcePath, String IntermediatesDir) {
    String FilePath = std::filesystem::absolute(SourcePath.parent_path()).string().c_str();

    // Remove trailing slash
    if (String::IsEndingWith(IntermediatesDir, "/")) IntermediatesDir = IntermediatesDir.SubString(IntermediatesDir.Length() - 1);

    String RelativePath = FilePath.SubString(std::filesystem::absolute(IntermediatesDir.GetData()).string().size() - 1);

    // Replace \ with /
    for (auto &Chr : RelativePath) if (Chr == '\\') Chr = '/';

    // Remove 'Public' directory from path
    if (RelativePath.IsStartingWith("/Public")) RelativePath = RelativePath.SubString(String("/Public/").Length());
    else if (RelativePath.IsStartingWith("Public")) RelativePath = RelativePath.SubString(String("Public/").Length());


    // Create missing directories
    OutPublic = IntermediatesDir / "Public" + (RelativePath == "" ? "" : "/" +  RelativePath);
    OutPrivate = IntermediatesDir / "Private" + (RelativePath == "" ? "" : "/" +  RelativePath);
}