#include "Utils/Utils.h"
#include <IO/Log.h>
#include <iostream>

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
