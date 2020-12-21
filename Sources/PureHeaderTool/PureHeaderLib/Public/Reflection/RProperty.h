#pragma once
#include "ReflectionUtilities.h"
#include "Types/String.h"

struct RType;

struct RProperty : public ReflectionObject {

	RProperty(const String& inPropertyType, const String& inPropertyName, const size_t inPropertyOffset)
		: PropertyName(inPropertyName), PropertyOffset(inPropertyOffset), PropertyID(MakeUniqueID(inPropertyName)) {
		if (!(PropertyType = RType::GetType(inPropertyType)))
			RClass::WaitTypeRegistration(inPropertyType, this, &RProperty::OnRegisterType);
	}

	/**
	 * Get this property value on given Object pointer
	 */
	template<typename Type = void>
	inline Type* Get(void* Object) { return (Type*)((size_t)Object + PropertyOffset); }

	/**
	 * Get property type
	 */
	inline RType* GetType() const { return PropertyType; }

	/**
	 * Get property name
	 */
	inline const String& GetName() const { return PropertyName; }

	/**
	 * Get property name
	 */
	inline const RUID GetID() const { return PropertyID; }

	/**
	 * Get property offset
	 */
	inline const size_t GetOffset() const { return PropertyOffset;  }

private:

	/**
	 * Bind property type
	 */
	void OnRegisterType(RType* RegisteredClass) { PropertyType = RegisteredClass; }

	/**
	 * Property type
	 */
	RType* PropertyType;

	/**
	 * Property name
	 */
	const String PropertyName;

	/**
	 * Property offset
	 */
	const size_t PropertyOffset;

	/**
	 * Property Unique ID
	 */
	const RUID PropertyID;
};