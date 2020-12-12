#include "RType.h"

#include <unordered_map>
#include <iostream>

inline static std::unordered_map<String, const RType*> Types;

const RType* RType::GetType(const String& inTypeName) {
	const auto& value = Types.find(inTypeName);
	ReflEnsure(value != Types.end(), (String(inTypeName) + " is not a reflected type").GetData());
	return value->second;
}

void RType::RegisterType_Internal(const String& inTypeName, const RType* inType) {
	ReflEnsure(Types.find(inTypeName) == Types.end(), (String("type ") + inTypeName + " is already registered").GetData());
	Types[inTypeName] = inType;
}