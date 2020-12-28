#pragma once
#include "Types/String.h"
#include <unordered_map>

/**
 * Get reflected type name
 */
template<typename RClass>
struct RTypeName;

/**
 * Is type reflected
 */
template<typename RClass>
struct RIsReflected { constexpr static const bool Value = false; };

void ReflEnsure(bool inCondition, const String& inMessage);

/**
 * Remove copy and move constructor for reflected objects
 */
struct ReflectionObject {
	inline ReflectionObject(const ReflectionObject&) = delete;
	inline ReflectionObject(const ReflectionObject&&) = delete;
protected:
	ReflectionObject() = default;
};

/**
 * Reflection unique ID
 */
typedef size_t RUID;

/**
 * Hash type name to RUID
 */
template<typename Type>
inline RUID MakeUniqueID() {
	return std::hash<String>{}(RTypeName<Type>::Name);
}

/**
 * Hash string to RUID
 */
inline RUID MakeUniqueID(const String& Name) {
	return std::hash<String>{}(Name);
}

template<typename Type>
inline bool UniqueIDAlreadyExist(const String& Name, std::unordered_map<RUID, Type> Container) {
	return Container.find(MakeUniqueID(Name)) != Container.end();
}