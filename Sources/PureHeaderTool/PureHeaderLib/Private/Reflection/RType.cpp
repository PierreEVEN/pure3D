#include "Reflection/RType.h"

#include <unordered_map>
#include <iostream>
#include "IO/Log.h"

inline static std::unordered_map<size_t, RType*>* Types = nullptr;

std::unordered_map<size_t, RType*>* GetTypes() {
	if (!Types) Types = new std::unordered_map<size_t, RType*>();
	return Types;
}


RType* RType::GetTypeVariant(size_t inTypeId) {
	const auto& value = GetTypes()->find(inTypeId);
	if (value == GetTypes()->end()) return nullptr;
	return value->second;
}

RType* RType::GetTypeVariant(const String& inTypeName) {
	return GetTypeVariant(std::hash<String>{}(inTypeName));
}


void RType::RegisterType_Internal(const String& inTypeName, RType* inType) {
	auto& foundElem = GetTypes()->find(inType->GetId());
	if (foundElem != GetTypes()->end()) {
		LOG_ASSERT("Cannot register " + inTypeName + " (two RTypes names gives the same UID : " + String(inType->GetId()) + ").");
	}
	(*GetTypes())[inType->GetId()] = inType;

	const auto& FoundDelegate = TypeRegistrationDelegate.find(inTypeName);
	if (FoundDelegate != TypeRegistrationDelegate.end()) {
		FoundDelegate->second.Execute(inType);
	}
}