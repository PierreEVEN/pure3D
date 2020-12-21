#include "Writer/ClassWriter.h"
#include <fstream>
#include "Parser/SFileData.h"
#include "Utils/Utils.h"
#include "Writer/HeaderWriter.h"
#include "Writer/SourceWriter.h"

using namespace Writer;

void Writer::WriteFiles(Parser::SFileData* File, const String& ModulePath, const String& OutputPath) {
	String FilePath = std::filesystem::absolute(File->GetFile().GetFilePath().parent_path()).string().c_str();

	String RelativePath = FilePath.SubString(std::filesystem::absolute(ModulePath.GetData()).string().size());

	// Replace \ with /
	for (auto& Chr : RelativePath) if (Chr == '\\') Chr = '/';

	// Remove 'Public' directory from path
	if (RelativePath.IsStartingWith("/Public")) RelativePath = RelativePath.SubString(String("/Public/").Length());

	// Create missing directories
	String PublicDirectory = OutputPath / "Public" / RelativePath;
	String PrivateDirectory = OutputPath / "Private" / RelativePath;	
	std::filesystem::create_directories(PublicDirectory.GetData());
	std::filesystem::create_directories(PrivateDirectory.GetData());

	String GeneratedHeaderPath = PublicDirectory / File->GetFile().GetName() + ".refl.h";
	String GeneratedSourcePath = PrivateDirectory / File->GetFile().GetName() + ".refl.cpp";

	if (File->GetFile().IsUpToDate(GeneratedHeaderPath) && File->GetFile().IsUpToDate(GeneratedSourcePath)) {
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
