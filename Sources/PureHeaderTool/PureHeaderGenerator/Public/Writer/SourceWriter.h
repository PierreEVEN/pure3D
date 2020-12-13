#pragma once
#include "Types/String.h"

namespace Parser {
	struct SFileData;
}

namespace Writer {
	String GenerateSource(Parser::SFileData* Data, const String& ReflHeaderPath);
}


