#include <IO/Log.h>
#include "Utils/Utils.h"
#include <iostream>
#include <Types/Path.h>
#include <filesystem>
#include "Parser/ClassParser.h"

int main(int argc, const char* argv[]) {
	String ModulePathStr;
	String ModuleName;

	Utils::Ensure(Utils::GetOption(argc, argv, "ModuleName", ModuleName), "Missing 'ModuleName' option");
	Utils::Ensure(Utils::GetOption(argc, argv, "ModulePath", ModulePathStr), "Missing 'ModulePath' option");
	Utils::Log("Running reflection tool on " + ModuleName);

	std::filesystem::path ModulePath(ModulePathStr.GetData());

	std::vector<Parser::FileReference> ScannedFiles = Parser::ScanFiles(ModulePath);

	for (const auto& File : ScannedFiles) {
		Parser::ParseFile(File);
	}

	Utils::Log("Scanning " + String(ScannedFiles.size()) + String(" headers."));
}