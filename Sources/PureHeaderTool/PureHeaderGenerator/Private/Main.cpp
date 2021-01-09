
#include "Utils/Utils.h"
#include <iostream>
#include <filesystem>
#include <chrono>
#include "Parser/ClassParser.h"
#include "Parser/SObject.h"
#include "Writer/ClassWriter.h"
#include <ctime>


int main(int argc, const char* argv[]) {

	std::chrono::steady_clock::time_point StartTime = std::chrono::steady_clock::now();

	String ModulePathStr;
	String ModuleName;
	String OutputPath;
	String DebugMode;
	String CMakeRebuildCommand;
	Utils::Ensure(Utils::GetOption(argc, argv, "ModuleName", ModuleName), "Missing 'ModuleName' option");
	Utils::Ensure(Utils::GetOption(argc, argv, "ModulePath", ModulePathStr), "Missing 'ModulePath' option");
	Utils::Ensure(Utils::GetOption(argc, argv, "OutputPath", OutputPath), "Missing 'OutputPath' option");
	Utils::Ensure(Utils::GetOption(argc, argv, "CmakeRebuild", CMakeRebuildCommand), "Missing 'CmakeRebuild' option");
	if (Utils::GetOption(argc, argv, "Debug", DebugMode))
		Utils::PHT_DEBUG_MODE = DebugMode == "ON";
	else Utils::PHT_DEBUG_MODE = false;

	/**
	 * Scanning
	 */
	std::filesystem::path ModulePath(ModulePathStr.GetData());

	// Get all potential reflected headers
	std::vector<Parser::SFileReference> ScannedFiles = Parser::ScanFiles(ModulePath);

	size_t Objects = 0;
	// Parse every header found
	for (const auto& File : ScannedFiles) Objects += Parser::ParseFile(File)->GetObjects().size();

	/**
	 * Writting
	 */
	for (const auto& File : Parser::ReflectedFiles) {
		Writer::WriteFiles(File.second, ModulePathStr, OutputPath);
	}

	size_t UpToDates = Parser::ReflectedFiles.size();
	for (const auto& File : Parser::ReflectedFiles) {
		if (File.second->IsFileUpToDate()) UpToDates--;
	}

	/**
	 * Regenerate project files
	 */
	if (UpToDates > 0) {
		std::filesystem::current_path(CMakeRebuildCommand.GetData());
		if (system("Build.sh")) {
			Utils::Log("Failed to regenerate cmake cache");
			exit(2);
		}
	}

	/**
	 * Summary
	 */
	const auto& Duration = std::chrono::steady_clock::now() - StartTime;


	Utils::Log("Running reflection tool on " + ModuleName + " (" + String(std::chrono::duration_cast<std::chrono::milliseconds>(Duration).count()) + " ms - " + String(Objects) + " objects)" + (Utils::PHT_DEBUG_MODE ? " - DEBUG MODE" : "") + " : " + UpToDates + " files updates.");
	if (Utils::PHT_DEBUG_MODE)
		for (const auto& File : Parser::ReflectedFiles) {
			Utils::Log("\t-> file : " + File.second->GetFile().GetName() + ".h");
			for (const auto& Object : File.second->GetObjects()) {
				Object->Log();
			}
		}


	for (const auto& Object : Parser::ReflectedFiles) delete Object.second;
}