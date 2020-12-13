#pragma once
#include <filesystem>
#include "Types/String.h"
#include "SStateMachine.h"

namespace Parser {

	class SObject;

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

		void ParseContent();
		void ParseStructureHeader(const SStateMachine& Structure);

		const std::vector<SObject*>& GetObjects() const { return Objects; }

	private:

		std::vector<SObject*> Objects;
		SFileReference File;
		String Content;
	};
}