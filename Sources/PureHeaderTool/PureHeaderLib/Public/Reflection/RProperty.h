#pragma once
#include "ReflectionUtilities.h"
#include "Types/String.h"

struct RType;

struct RProperty : public ReflectionObject {

	RProperty(const RType* inPropertyType, const String& inPropertyName, const size_t inPropertyOffset)
		: PropertyType(inPropertyType), PropertyName(inPropertyName), PropertyOffset(inPropertyOffset) {}

private:

	const RType* PropertyType;
	const String PropertyName;
	const size_t PropertyOffset;
};