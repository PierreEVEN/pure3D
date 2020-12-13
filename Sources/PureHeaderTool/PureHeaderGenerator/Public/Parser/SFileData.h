#pragma once
#include <filesystem>
#include "Types/String.h"
#include "SStateMachine.h"
#include <unordered_map>

namespace Parser {

	class SObject;
	class SFileData;

	inline std::unordered_map<String, SFileData*> ReflectedFiles;

	struct SFileReference {
		SFileReference(const std::filesystem::path& inFilePath, const std::filesystem::path& inReflectedPath);

		const std::filesystem::path& GetFilePath() const { return FilePath; }

		const String GetName() const;

		String GetDateFormated() const;

	private:

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