#pragma once
#include <filesystem>
#include "Types/String.h"
#include "SStateMachine.h"
#include <unordered_map>
#include <set>
#include <unordered_set>

namespace Parser {

	struct SObject;
	struct SFileData;

	inline std::unordered_map<String, SFileData*> ReflectedFiles;

	struct SFileReference {
		SFileReference(const std::filesystem::path& inFilePath, const std::filesystem::path& inReflectedPath);

		const std::filesystem::path& GetFilePath() const { return FilePath; }

		const String GetName() const;

		String GetDateFormated() const;

		bool IsUpToDate(const String& GeneratedFile) const;

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
		void ParseStructureHeader(const SStateMachine& Structure, const size_t StructureBeginning);

		const std::vector<SObject*>& GetObjects() const { return Objects; }
		void BuildDynamicTypes();
		inline const std::unordered_set<String> GetDynamicTypes() const { return DynamicTypes; }
		void AddDynamicType(const String& inType);

		inline bool IsFileUpToDate() const { return IsUpToDate; }
		inline void SetUpToDate() { IsUpToDate = true; }

		inline size_t GetFileID() const { return FileUniqueID; }

	private:

		std::unordered_set<String> DynamicTypes;
		bool IsUpToDate = false;
		std::vector<SObject*> Objects;
		SFileReference File;
		String Content;
		size_t FileUniqueID;
	};
}