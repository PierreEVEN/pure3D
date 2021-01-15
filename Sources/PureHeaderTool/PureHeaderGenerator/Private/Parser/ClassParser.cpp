#include "Parser/ClassParser.h"
#include "Utils/Utils.h"

#include <sstream>
#include <fstream>
#include <iostream>

using namespace Parser;

std::vector<SFileReference> Parser::ScanFiles(const std::filesystem::path& inDirectory) {
    if (!std::filesystem::exists(inDirectory)) return {};

	std::vector<SFileReference> Result;

	for (const auto& Child : std::filesystem::directory_iterator(inDirectory)) {

		if (std::filesystem::is_directory(Child))
			for (const auto& File : ScanFiles(Child))
				Result.push_back(File);
		else {
			String extension = Child.path().extension().string().c_str();
			if (extension != ".h") continue;

			Result.push_back(SFileReference(Child.path(), "NONE"));
		}
	}

	return Result;
}


SFileData* Parser::ParseFile(const SFileReference& inFile) {
	SFileData* Data = new SFileData(inFile);
	Data->ParseContent();
	return Data;
}