#include "Reflection/RType.h"

#include <unordered_map>
#include <iostream>

inline static std::unordered_map<std::string, const RType*>* Types = nullptr;

const RType* RType::GetType(const String& inTypeName) {
	if (!Types) return nullptr;
	const auto& value = Types->find(inTypeName.GetData());
	ReflEnsure(value != Types->end(), (String(inTypeName) + " is not a reflected type").GetData());
	return value->second;
}

void RType::RegisterType_Internal(const String& inTypeName, const RType* inType) {
	if (!Types) Types = new std::unordered_map<std::string, const RType*>;
	ReflEnsure(Types->find(inTypeName.GetData()) == Types->end(), "type " + inTypeName + " is already registered");
	(*Types)[inTypeName.GetData()] = inType;
}