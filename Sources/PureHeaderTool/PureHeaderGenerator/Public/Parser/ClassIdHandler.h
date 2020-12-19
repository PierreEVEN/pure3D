#pragma once
#include <unordered_map>
#include "Types/String.h"
#include <fstream>
#include <set>
#include <string>
#include "Utils/Utils.h"


namespace Parser {

	struct SClassIdHandler {

		static void LoadClassIds(String FilePath) {
			std::ifstream InFile(FilePath.GetData());

			std::string line;
			while (std::getline(InFile, line)) {
				String Name;
				String IDString;
				if (!String::SplitString(line.c_str(), { '\t' }, Name, IDString)) {
					Utils::Log("failed to read object id : " + String(line.c_str()));
				}
				std::cout << "found " << IDString.GetData() << " : " << atoi(IDString.GetData()) << std::endl;

				size_t ID = atoi(IDString.GetData());
				ExistingIds.insert(ID);
				ClassIds[Name] = ID;
			}

			InFile.close();
		}

		static void SaveClassIds(String FilePath) {
			std::ofstream InFile(FilePath.GetData());
			for (const auto& Class : ClassIds) {
				InFile << Class.first.GetData() << "\t" << Class.second << std::endl;
			}
			InFile.close();
		}


		static size_t GetClassId(const String& ClassName) {
			auto& FoundId = ClassIds.find(ClassName);
			if (FoundId == ClassIds.end()) {
				size_t GeneratedId = GenId();
				ExistingIds.insert(GeneratedId);
				ClassIds[ClassName] = GeneratedId;
				return GeneratedId;
			}
			return FoundId->second;
		}

	private:


		static size_t GenId() {
			size_t Max = 0;
			while (ExistingIds.find(Max) != ExistingIds.end()) Max++;
			return Max;
		}

		inline static std::set<size_t> ExistingIds;
		inline static std::unordered_map<String, size_t> ClassIds;
	};

};