#pragma once
#include "Types/Array.h"
#include <filesystem>
#include <vector>

namespace Parser {

	struct FileItem {

		FileItem(const String& inRawData, const String& inFileName, const String& inItemName, size_t inFromLine, size_t inToLine)
			: RawData(inRawData), FileName(inFileName), ItemName(inItemName), FromLine(inFromLine), ToLine(inToLine) {}

	private:
		const String ItemName;
		const String FileName;
		size_t FromLine;
		size_t ToLine;
		String RawData;
		std::vector<FileItem> ItemContent;
	};

	struct Object : public FileItem {

	};

	struct Struct : public Object {

	};

	struct Class : public Object {

	};

	struct Enum : public FileItem {

	};

	struct Property : public FileItem {

	};

	struct Function : public FileItem {

	};

	struct Constructor : public Function {

	};

	struct FileReference {
		FileReference(const std::filesystem::path& inFilePath, const std::filesystem::path& inReflectedPath);

		const std::filesystem::path& GetFilePath() const { return FilePath; }

		bool IsUpToDate() const;

	private:

		static String FormatDate(time_t inTime);

		std::filesystem::path FilePath;
		std::filesystem::path ReflectedPath;
		std::time_t LastEdit;
	};

	struct FileData {
		FileData(const FileReference& inFilePath);

		const FileReference& GetFile() const { return File; }

		void ParseContent();

	private:
		FileReference File;
		String Content;
	};

	struct StateMachine {
		size_t CurrentPos = 0;
		bool IsReadingStructure = false;
		bool ReflectBodyLevel = 0;
		bool IsInString = false;
		bool IsInOneLineComment = false;
		bool IsInMultiLineLineComment = false;
		size_t IndentationLevel = 0;
	};

	std::vector<FileReference> ScanFiles(const std::filesystem::path& inDirectory);

	bool DetectStructure(const char* Data, const char* Structure);

	void ParseFile(const FileReference& file);

}