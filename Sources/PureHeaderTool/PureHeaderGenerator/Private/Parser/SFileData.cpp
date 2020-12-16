
#include "Parser/SFileData.h"
#include <fstream>
#include <sstream>
#include "Parser/STypes.h"
#include "Parser/SObject.h"
#include "Utils/Utils.h"
#include "Types/String.h"

using namespace Parser;

Parser::SFileReference::SFileReference(const std::filesystem::path& inFilePath, const std::filesystem::path& inReflectedPath)
	: FilePath(inFilePath), ReflectedPath(inReflectedPath) {
	std::filesystem::file_time_type file_time = std::filesystem::last_write_time(FilePath);
	LastEdit = std::chrono::system_clock::to_time_t(
		std::chrono::time_point_cast<std::chrono::system_clock::duration>(
			file_time - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now()
			)
	);
}

const String Parser::SFileReference::GetName() const {
	return FilePath.stem().u8string().c_str();
}

String Parser::SFileReference::GetDateFormated() const {
	std::tm* gmt = new std::tm();
#if _WIN32
	gmtime_s(gmt, &LastEdit);
#else
	gmtime_r(&inTime, gmt);
#endif

	std::stringstream TimeBuffer;
	TimeBuffer << std::put_time(gmt, "%A, %d %B %Y %H:%M:%S");
	delete gmt;
	return String("/// VERSION : ") + TimeBuffer.str().c_str();
}

bool Parser::SFileReference::IsUpToDate(const String& GeneratedFile) const {
	bool UpToDate = false;
	std::ifstream FileStream(GeneratedFile.GetData());
	std::string Line;
	if (std::getline(FileStream, Line)) {
		UpToDate = String(Line.c_str()).IsStartingWith(GetDateFormated());
	}
	FileStream.close();
	return UpToDate;
}

Parser::SFileData::SFileData(const SFileReference& inFilePath)
	: File(inFilePath) {

	ReflectedFiles[inFilePath.GetFilePath().u8string().c_str()] = this;

	std::ifstream FileStream(File.GetFilePath());
	Content = "";
	std::string Line;
	while (std::getline(FileStream, Line)) {
		Content << Line.c_str() << '\n';
	}
	FileStream.close();
}

void Parser::SFileData::SFileData::ParseContent() {
	SStateMachine Status;
	Status.CurrentLine = 1;

	String Simplified = Content;
	size_t StructureBeginning = 0;


	for (Status.CurrentPos = 0; Status.CurrentPos < Simplified.Length(); ++Status.CurrentPos) {
		const char* CurrentData = &Simplified.GetData()[Status.CurrentPos];

		Status.CountLine(CurrentData[0]);

		if (Status.DoesSkipChar(CurrentData)) continue;

		if (!Status.IsReadingStructure) {
			if (Utils::IsStartingWith(CurrentData, "REFLECT(")) {
				StructureBeginning = Status.CurrentLine;
				Status.IsReadingStructure = true;
				Status.Content = "";
			}
		}

		if (Status.IsReadingStructure) {
			Status.Content << CurrentData[0];
			if (CurrentData[0] == '{') {
				Status.IndentationLevel++;
			}
			else if (CurrentData[0] == '}') {
				Status.IndentationLevel--;
				if (Status.IndentationLevel == 0) {
					ParseStructureHeader(Status, StructureBeginning);
					Status.IsReadingStructure = false;
				}
			}
		}
	}
}

void Parser::SFileData::ParseStructureHeader(const SStateMachine& Structure, const size_t StructureBeginning) {
	SStateMachine Status;
	EObjectType ObjectType = EObjectType::ObjType_None;
	for (int i = 0; i < Structure.Content.Length(); ++i) {
		const char* CurrentData = &Structure.Content.GetData()[i];

		if (CurrentData[0] == '{' || CurrentData[0] == ':') break;

		//Extract structure type
		if (ObjectType == EObjectType::ObjType_None) {
			if (Utils::IsStartingWith(CurrentData, "enum")) ObjectType = EObjectType::ObjType_Enum;
			if (Utils::IsStartingWith(CurrentData, "struct")) ObjectType = EObjectType::ObjType_Struct;
			if (Utils::IsStartingWith(CurrentData, "class"))  ObjectType = EObjectType::ObjType_Class;
			continue;
		}

		//Then extract name
		if (CurrentData[0] == ' ' || CurrentData[0] == '\t' || CurrentData[0] == '\n' || CurrentData[0] == '}') {
			Status.PauseCapture = true;
		}
		else if (Status.PauseCapture) {
			if (Utils::IsStartingWith(CurrentData, "final")) {
				i += String("final").Length() - 1;
				continue;
			}
			Status.Content = "";
			Status.PauseCapture = false;
		}
		if (!Status.PauseCapture) Status.Content << CurrentData[0];
	}

	SObject* NewObject = nullptr;
	switch (ObjectType)
	{
	case Parser::EObjectType::ObjType_Enum:
		NewObject = new SEnum(Status.Content, ObjectType);
		break;
	case Parser::EObjectType::ObjType_Struct:
	case Parser::EObjectType::ObjType_Class:
		NewObject = new SStruct(Status.Content, ObjectType);
		break;
	}

	if (!NewObject) return;

	NewObject->ParseContent(Structure.Content, StructureBeginning);
	Objects.push_back(NewObject);
}

Parser::SFileData::~SFileData() {
	for (auto& Object : Objects) delete Object;
}
