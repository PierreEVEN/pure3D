#include "RType.h"

inline static std::unordered_map<std::string, const RType*> types;

const RType* RType::GetType(const char* typeName) {
	const auto& value = types.find(typeName);
	if (value == types.end()) return nullptr;
	return value->second;
}

RType* RType::RegisterType(const char* inTypeName, size_t inTypeSize) {
	if (GetType(inTypeName)) {
		std::cout << inTypeName << " is already registered" << std::endl;
		exit(1);
	}
	RType* NewType = new RType(inTypeName, inTypeSize);
	types[inTypeName] = NewType;
	return NewType;
}

void RType::DeleteTypes() {
	for (const auto& item : types) {
		delete item.second;
	}
	types.clear();
}