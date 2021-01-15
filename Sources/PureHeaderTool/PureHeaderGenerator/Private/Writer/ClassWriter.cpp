#include "Writer/ClassWriter.h"
#include <fstream>
#include "Parser/SFileData.h"
#include "Utils/Utils.h"
#include "Writer/HeaderWriter.h"
#include "Writer/SourceWriter.h"
#include <set>

using namespace Writer;

void Writer::WriteFiles(Parser::SFileData* File, const String& ModulePath, const String& OutputPath) {

	File->BuildDynamicTypes();

	// Create missing directories
	String PublicDirectory;
	String PrivateDirectory;

    Utils::SourceToIntermediate(PublicDirectory, PrivateDirectory, File->GetFile().GetFilePath(),OutputPath);


	std::filesystem::create_directories(PublicDirectory.GetData());
	std::filesystem::create_directories(PrivateDirectory.GetData());

	String GeneratedHeaderPath = PublicDirectory / File->GetFile().GetName() + ".refl.h";
	String GeneratedSourcePath = PrivateDirectory / File->GetFile().GetName() + ".refl.cpp";

	if ((File->GetFile().IsUpToDate(GeneratedHeaderPath) && File->GetFile().IsUpToDate(GeneratedSourcePath)) || File->GetObjects().size() == 0) {
		File->SetUpToDate();
		return;
	}

	// Generate data
	String HeaderContent = GenerateHeader(File);
	String SourceContent = GenerateSource(File, GeneratedHeaderPath);

	// Write files
	std::ofstream Header(GeneratedHeaderPath.GetData(), std::ofstream::out);
	std::ofstream Source(GeneratedSourcePath.GetData(), std::ofstream::out);
	Header.write(HeaderContent.GetData(), HeaderContent.Length());
	Source.write(SourceContent.GetData(), SourceContent.Length());
	Header.close();
	Source.close();
}
