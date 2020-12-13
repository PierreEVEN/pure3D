#include "Parser/ClassParser.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include "Utils/Utils.h"

using namespace Parser;

std::vector<SFileReference> Parser::ScanFiles(const std::filesystem::path& inDirectory) {
	std::vector<SFileReference> Result;

	for (const auto& Child : std::filesystem::directory_iterator(inDirectory)) {

		if (std::filesystem::is_directory(Child))
			for (const auto& File : ScanFiles(Child)) 
				Result.push_back(File);
		else {
			String extension = Child.path().extension().u8string().c_str();
			if (extension != ".h") continue;

			Result.push_back(SFileReference(Child.path(), "NONE"));
		}
	}

	return Result;
}

bool Parser::SFileData::IsStartingWith(const char* Data, const char* Start) {
	for (int i = 0; Start[i] != '\0'; ++i) {
		if (Data[i] == '\0' || Data[i] != Start[i]) return false;
	}
	return true;
}

SFileData* Parser::ParseFile(const SFileReference& inFile) {
	SFileData* Data = new SFileData(inFile);
	Data->ParseContent();
	return Data;
}


Parser::SFileReference::SFileReference(const std::filesystem::path& inFilePath, const std::filesystem::path& inReflectedPath)
	: FilePath(inFilePath), ReflectedPath(inReflectedPath) {
	std::filesystem::file_time_type file_time = std::filesystem::last_write_time(FilePath);
	LastEdit = std::chrono::system_clock::to_time_t(
		std::chrono::time_point_cast<std::chrono::system_clock::duration>(
			file_time - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now()
			)
	);
}

bool Parser::SFileReference::IsUpToDate() const {
	std::ifstream FileStream(ReflectedPath);
	char* LineData = new char[1000];
	FileStream.getline(LineData, 1000);
	String LineString(LineData);
	delete LineData;
	return LineString.IsStartingWith(FormatDate(LastEdit));
}

const String Parser::SFileReference::GetName() const {
	return FilePath.filename().u8string().c_str();
}

String Parser::SFileReference::FormatDate(time_t inTime) {
	std::tm* gmt = new std::tm();
	gmtime_s(gmt, &inTime);

	std::stringstream TimeBuffer;
	TimeBuffer << std::put_time(gmt, "%A, %d %B %Y %H:%M:%S");
	delete gmt;
	return String("//LAST UPDATE : ") + TimeBuffer.str().c_str();
}

Parser::SFileData::SFileData(const SFileReference& inFilePath)
	: File(inFilePath) {

	std::ifstream FileStream(File.GetFilePath());
	Content = "";
	std::string line;
	while (std::getline(FileStream, line)) {
		Content << line.c_str() << '\n';
	}
}

void Parser::SFileData::SFileData::ParseContent() {
	SStateMachine Status;

	String Simplified = Content;


	for (Status.CurrentPos = 0; Status.CurrentPos < Simplified.Length(); ++Status.CurrentPos) {
		const char* currentData = &Simplified.GetData()[Status.CurrentPos];

		if (Status.DoesSkipChar(currentData)) continue;

		if (!Status.IsReadingStructure) {
			if (IsStartingWith(currentData, "REFLECT(")) {
				Status.IsReadingStructure = true;
				Status.Content = "";
			}
		}

		if (Status.IsReadingStructure) {
			Status.Content << currentData[0];
			if (currentData[0] == '{') {
				Status.IndentationLevel++;
			}
			else if (currentData[0] == '}') {
				Status.IndentationLevel--;
				if (Status.IndentationLevel == 0) {
					ParseStructureHeader(Status);
					Status.IsReadingStructure = false;
				}
			}
		}
	}
}

void Parser::SFileData::ParseStructureHeader(const SStateMachine& Structure) {
	SStateMachine Status;
	for (int i = 0; i < Structure.Content.Length(); ++i) {
		const char* CurrentData = &Structure.Content.GetData()[i];

		if (Status.DoesSkipChar(CurrentData)) continue;

		if (CurrentData[0] == '{') break;

		if (IsStartingWith(CurrentData, "enum")) {
			SObject* NewObject = new SEnum("", EObjectType::ENUM);
			NewObject->ParseContent(Structure.Content);
			Objects.push_back(NewObject);
		}
		if (IsStartingWith(CurrentData, "class")) {
			SObject* NewObject = new SStruct("", EObjectType::STRUCT);
			NewObject->ParseContent(Structure.Content);
			Objects.push_back(NewObject);
		}
		if (IsStartingWith(CurrentData, "struct")) {
			SObject* NewObject = new SClass("", EObjectType::CLASS);
			NewObject->ParseContent(Structure.Content);
			Objects.push_back(NewObject);
		}
	}
}

Parser::SFileData::~SFileData() {
	for (auto& Object : Objects) delete Object;
}

bool Parser::SStateMachine::DoesSkipChar(const char* Position) {
	if (Position[1] == '\0') return false;

	//Detect multiline comment only outside strings
	if (!IsInString && !IsInOneLineComment && !IsInMultiLineLineComment) {
		if (Position[0] == '/' && Position[1] == '*') {
			IsInMultiLineLineComment = true;
			return true;
		}
	}

	//Detect multiline comment ending
	if (IsInMultiLineLineComment) {
		if (Position[0] == '/' && Position[-1] == '*') {
			IsInMultiLineLineComment = false;
			return true;
		}
	}

	//Skip multiline comment content
	if (IsInMultiLineLineComment) return true;

	//Detect String outside comments
	if (Position[0] == '"' && Position[-1] != '\\') {
		IsInString = !IsInString;
		return true;
	}

	//Skip Strings
	if (IsInString) return true;

	//Detect OneLine comments
	if (!IsInOneLineComment && Position[0] == '/' && Position[1] == '/') {
		IsInOneLineComment = true;
	}

	//Skip OneLine comments
	if (IsInOneLineComment) {
		if (Position[0] == '\n') IsInOneLineComment = false;
		return true;
	}
	return false;
}

void Parser::SEnum::ParseContent(const String& Content)
{
	SStateMachine Status;

	for (Status.CurrentPos = 0; Status.CurrentPos < Content.Length(); ++Status.CurrentPos) {
		const char* CurrentData = &Content.GetData()[Status.CurrentPos];

		if (!Status.IsReadingStructure) {
			if (CurrentData[0] == '{') Status.IsReadingStructure = true;
			continue;
		}
		//Remove space/tabs...
		if (CurrentData[0] == ' ' || CurrentData[0] == '\t' || CurrentData[0] == '\n' || CurrentData[0] == '}') continue;

		//Next field
		if (CurrentData[0] == ',') {
			Fields.push_back(Status.Content);
			Status.Content = "";
			Status.PauseCapture = false;
			continue;
		}
		//Skip assigned value
		if (CurrentData[0] == '=') Status.PauseCapture = true;

		if (Status.PauseCapture) continue;

		Status.Content << CurrentData[0];		
	}
	if (Status.Content != "") Fields.push_back(Status.Content);
}
