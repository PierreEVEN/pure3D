#include "Reflection/RType.h"

#include <unordered_map>
#include <iostream>

inline static std::unordered_map<std::string, const RType*>* Types = nullptr;

const RType* RType::GetType(const String& inTypeName) {
	if (!Types) Types = new std::unordered_map<std::string, const RType*>;
	const auto& value = Types->find(inTypeName.GetData());
	if (value == Types->end()) return nullptr;
	return value->second;
}

void RType::RegisterType_Internal(const String& inTypeName, const RType* inType) {
	if (!Types) Types = new std::unordered_map<std::string, const RType*>;
	ReflEnsure(Types->find(inTypeName.GetData()) == Types->end(), "type " + inTypeName + " is already registered");
	(*Types)[inTypeName.GetData()] = inType;
}