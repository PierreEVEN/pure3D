#pragma once
#include "ReflectionUtilities.h"
#include "Types/String.h"

struct RType;

struct RProperty : public ReflectionObject {

	RProperty(const RType* inPropertyType, const String& inPropertyName, const size_t inPropertyOffset)
		: PropertyType(inPropertyType), PropertyName(inPropertyName), PropertyOffset(inPropertyOffset) {}

	template<typename Type>
	inline Type* Get(void* Object) {
		return (Type*)((size_t)Object + PropertyOffset);
	}

	inline const RType* GetType() const { return PropertyType; }
	inline const String& GetName() const { return PropertyName; }

private:

	const RType* PropertyType;
	const String PropertyName;
	const size_t PropertyOffset;
};