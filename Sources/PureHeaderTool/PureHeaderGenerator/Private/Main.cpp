
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
	String OutputPathStr;
	String DebugMode;
	String CMakeRebuildCommand;
	Utils::Ensure(Utils::GetOption(argc, argv, "ModuleName", ModuleName), "Missing 'ModuleName' option");
	Utils::Ensure(Utils::GetOption(argc, argv, "ModulePath", ModulePathStr), "Missing 'ModulePath' option");
	Utils::Ensure(Utils::GetOption(argc, argv, "OutputPath", OutputPathStr), "Missing 'OutputPath' option");
	Utils::Ensure(Utils::GetOption(argc, argv, "CmakeRebuild", CMakeRebuildCommand), "Missing 'CmakeRebuild' option");
	if (Utils::GetOption(argc, argv, "Debug", DebugMode))
		Utils::PHT_DEBUG_MODE = DebugMode == "ON";
	else Utils::PHT_DEBUG_MODE = false;

	/**
	 * Scanning
	 */
	std::filesystem::path ModulePath(ModulePathStr.GetData());
	std::filesystem::path OutputPath(OutputPathStr.GetData());

	// Get all potential reflected headers
	std::vector<Parser::SFileReference> ScannedFiles = Parser::ScanFiles(ModulePath);
	std::vector<Parser::SFileReference> ExistingReflFiles = Parser::ScanFiles(OutputPath);

	size_t Objects = 0;
	// Parse every header found
	for (const auto& File : ScannedFiles) Objects += Parser::ParseFile(File)->GetObjects().size();

	/**
	 * Writting
	 */
	for (const auto& File : Parser::ReflectedFiles) {
		Writer::WriteFiles(File.second, ModulePathStr, OutputPathStr);
	}

	size_t UpToDates = Parser::ReflectedFiles.size();
	for (const auto& File : Parser::ReflectedFiles) {
		if (File.second->IsFileUpToDate()) UpToDates--;
		for (int64_t i = ExistingReflFiles.size() - 1; i >= 0; --i) {
			String FilePath = File.second->GetFile().GetFilePath().string().c_str();
			Utils::Log(FilePath.SubString(0, FilePath.Length() - String::GetFileExtension(FilePath).Length() - 2) + ".refl.h");
			if (String(ExistingReflFiles[i].GetFilePath().string().c_str()) == FilePath.SubString(0, FilePath.Length() - String::GetFileExtension(FilePath).Length() - 2) + ".refl.h") {
				ExistingReflFiles.erase(ExistingReflFiles.begin() + i);
			} //@TODO FAIRE LE BON CHEMIN POUR CE TRUC!!!!!
		}
	}

	//Remove outdated reflection files
	for (auto& File : ExistingReflFiles) {
		Utils::Log("éRemove old file : " + String(File.GetFilePath().string().c_str()));
	}

	/**
	 * Regenerate project files
	 */
	if (UpToDates > 0) {
		std::filesystem::current_path(CMakeRebuildCommand.GetData());
#if _WIN32
		if (system("Build.sh")) {
#else
        if (system("sh Build.sh")) {
#endif
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