#include <IO/Log.h>
#include "Utils/Utils.h"
#include <iostream>
#include <Types/Path.h>
#include <filesystem>
#include <chrono>
#include "Parser/ClassParser.h"
#include "Parser/SObject.h"

int main(int argc, const char* argv[]) {

	const auto& StartTime = std::chrono::steady_clock::now();

	String ModulePathStr;
	String ModuleName;
	String DebugMode;

	Utils::Ensure(Utils::GetOption(argc, argv, "ModuleName", ModuleName), "Missing 'ModuleName' option");
	Utils::Ensure(Utils::GetOption(argc, argv, "ModulePath", ModulePathStr), "Missing 'ModulePath' option");
	if (Utils::GetOption(argc, argv, "Debug", DebugMode)) 
		Utils::PHT_DEBUG_MODE = DebugMode == "ON";
	else Utils::PHT_DEBUG_MODE = false;

	std::filesystem::path ModulePath(ModulePathStr.GetData());

	std::vector<Parser::SFileReference> ScannedFiles = Parser::ScanFiles(ModulePath);
	std::vector<Parser::SFileData*> Data;

	for (const auto& File : ScannedFiles) Data.push_back(std::move(Parser::ParseFile(File)));

	const auto& Duration = std::chrono::steady_clock::now() - StartTime;

	size_t Objects = 0;
	for (const auto& File : Data) for (const auto& Object : File->GetObjects()) Objects++;

	Utils::Log("Running reflection tool on " + ModuleName + " (" + String(std::chrono::duration_cast<std::chrono::milliseconds>(Duration).count()) + " ms - " + String(Objects) + " objects)"  + (Utils::PHT_DEBUG_MODE ? " - DEBUG MODE" : ""));
	if (Utils::PHT_DEBUG_MODE) {
		Utils::Log("\t- Found " + String(Data.size()) + String(" headers."));
		for (const auto& File : Data) {
			Utils::Log("\t\t-> " + File->GetFile().GetName() + " : " + String(File->GetObjects().size()) + " objects");
			for (const auto& Object : File->GetObjects()) {
				Object->Log();
			}
		}
	}

	for (auto& Item : Data) delete Item;
}