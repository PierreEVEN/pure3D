#pragma once
#include "Types/String.h"

namespace Parser {
	struct SFileData;
}

namespace Writer {
	String GenerateHeader(Parser::SFileData* Data);
}


