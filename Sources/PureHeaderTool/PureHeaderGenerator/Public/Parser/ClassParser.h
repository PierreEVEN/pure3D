#pragma once
#include "SFileData.h"

namespace Parser {

	std::vector<SFileReference> ScanFiles(const std::filesystem::path& inDirectory);

	SFileData* ParseFile(const SFileReference& file);
}