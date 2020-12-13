#pragma once
#include "Types/Array.h"
#include <filesystem>
#include <vector>

namespace Parser {

	enum class EObjectType {
		ObjType_None = 0,
		ObjType_Enum = 1,
		ObjType_Struct = 2,
		ObjType_Class = 3
	};


	struct SStateMachine {
		size_t CurrentPos = 0;
		bool IsReadingStructure = false;
		bool IsInString = false;
		bool IsInOneLineComment = false;
		bool IsInMultiLineLineComment = false;
		bool PauseCapture = false;
		size_t IndentationLevel = 0;
		String Content = "";

		bool DoesSkipChar(const char* Position);
	};


	struct SObject {
		SObject(const String& inObjectName, EObjectType inObjectType)
			: ObjectName(inObjectName), ObjectType(inObjectType) {}
		virtual void ParseContent(const String& Content) = 0;
		virtual void Log() = 0;
		const String& GetName() const { return ObjectName; }
		const EObjectType& GetType() const { return ObjectType; }
	private:
		String ObjectName;
		EObjectType ObjectType;
	};


	struct SEnum : public SObject {
		using SObject::SObject;
		void ParseContent(const String& Content);
		virtual void Log();
	private:
		std::vector<String> Fields;
	};


	struct SStruct : public SObject {
		using SObject::SObject;
		void ParseContent(const String& Content);
		virtual void Log();
	};

	struct SFileReference {
		SFileReference(const std::filesystem::path& inFilePath, const std::filesystem::path& inReflectedPath);

		const std::filesystem::path& GetFilePath() const { return FilePath; }

		bool IsUpToDate() const;

		const String GetName() const;

	private:

		static String FormatDate(time_t inTime);

		std::filesystem::path FilePath;
		std::filesystem::path ReflectedPath;
		std::time_t LastEdit;
	};

	struct SFileData final {

		SFileData(const SFileReference& inFilePath);
		~SFileData();

		const SFileReference& GetFile() const { return File; }

		bool IsStartingWith(const char* Data, const char* Start);
		void ParseContent();
		void ParseStructureHeader(const SStateMachine& Structure);

		const std::vector<SObject*>& GetObjects() const { return Objects; }

	private:

		std::vector<SObject*> Objects;
		SFileReference File;
		String Content;
	};
	std::vector<SFileReference> ScanFiles(const std::filesystem::path& inDirectory);

	SFileData* ParseFile(const SFileReference& file);
}