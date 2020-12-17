#pragma once
#include "ReflectionUtilities.h"
#include "Types/String.h"

struct RType;

struct RProperty : public ReflectionObject {

	RProperty(const String& inPropertyType, const String& inPropertyName, const size_t inPropertyOffset)
		: PropertyName(inPropertyName), PropertyOffset(inPropertyOffset) {
		PropertyType = nullptr;
		if (RType* FoundType = RType::GetType(inPropertyType))
			PropertyType = FoundType;
		else 
			RClass::WaitTypeRegistration(inPropertyType, this, &RProperty::OnRegisterType);
	}

	template<typename Type>
	inline Type* Get(void* Object) {
		return (Type*)((size_t)Object + PropertyOffset);
	}

	inline RType* GetType() const { return PropertyType; }
	inline const String& GetName() const { return PropertyName; }
	inline const size_t GetOffset() const { return PropertyOffset;  }
private:


	void OnRegisterType(RType* RegisteredClass) {
		PropertyType = RegisteredClass;
	}

	RType* PropertyType;
	const String PropertyName;
	const size_t PropertyOffset;
};