#include "Parser/ClassParser.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include "Utils/Utils.h"

using namespace Parser;

std::vector<FileReference> Parser::ScanFiles(const std::filesystem::path& inDirectory) {
	std::vector<FileReference> Result;

	for (const auto& Child : std::filesystem::directory_iterator(inDirectory)) {

		if (std::filesystem::is_directory(Child))
			for (const auto& File : ScanFiles(Child)) 
				Result.push_back(File);
		else {
			String extension = Child.path().extension().u8string().c_str();
			if (extension != ".h") continue;

			Result.push_back(FileReference(Child.path(), "NONE"));
		}
	}

	return Result;
}

bool Parser::DetectStructure(const char* Data, const char* Structure) {
	for (int i = 0; Structure[i] != '\0'; ++i) {
		if (Data[i] == '\0' || Data[i] != Structure[i]) return false;
	}
	return true;
}

void Parser::ParseFile(const FileReference& inFile) {
	FileData Data(inFile);
	Data.ParseContent();
}

Parser::FileReference::FileReference(const std::filesystem::path& inFilePath, const std::filesystem::path& inReflectedPath)
	: FilePath(inFilePath), ReflectedPath(inReflectedPath) {
	std::filesystem::file_time_type file_time = std::filesystem::last_write_time(FilePath);
	LastEdit = std::chrono::system_clock::to_time_t(
		std::chrono::time_point_cast<std::chrono::system_clock::duration>(
			file_time - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now()
			)
	);
}

bool Parser::FileReference::IsUpToDate() const {
	std::ifstream FileStream(ReflectedPath);
	char* LineData = new char[1000];
	FileStream.getline(LineData, 1000);
	String LineString(LineData);
	delete LineData;
	return LineString.IsStartingWith(FormatDate(LastEdit));
}

String Parser::FileReference::FormatDate(time_t inTime) {
	std::tm* gmt = new std::tm();
	gmtime_s(gmt, &inTime);

	std::stringstream TimeBuffer;
	TimeBuffer << std::put_time(gmt, "%A, %d %B %Y %H:%M:%S");
	delete gmt;
	return String("//LAST UPDATE : ") + TimeBuffer.str().c_str();
}

Parser::FileData::FileData(const FileReference& inFilePath)
	: File(inFilePath) {

	std::ifstream FileStream(File.GetFilePath());
	Content = "";
	std::string line;
	while (std::getline(FileStream, line)) {
		Content << line.c_str() << '\n';
	}
}

void Parser::FileData::ParseContent()
{
	StateMachine Status;

	for (Status.CurrentPos = 0; Status.CurrentPos < Content.Length(); ++Status.CurrentPos) {
		const char* currentData = &Content.GetData()[Status.CurrentPos];

		/**
		 * Skip strings
		 */
		if (currentData[0] == '"' && currentData[-1] != '\\') {
			Status.IsInString = !Status.IsInString;
			continue;
		}
		if (Status.IsInString) continue;



		DetectStructure(currentData, "REFLECT_BODY();");






	}
}
