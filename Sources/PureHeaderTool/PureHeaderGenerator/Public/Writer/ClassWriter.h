#pragma once
#include "Types/String.h"

namespace Parser {
	struct SFileData;
}

namespace Writer {

	void WriteFiles(Parser::SFileData* File, const String& ModulePath, const String& OutputPath);

}