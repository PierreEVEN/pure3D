#pragma once
#include "ReflectionUtilities.h"
#include "Types/String.h"

struct RType;

struct RProperty : public ReflectionObject {

	RProperty(const String& inPropertyType, const String& inPropertyName, const size_t inPropertyOffset, bool InIsTransient)
		: PropertyName(inPropertyName), PropertyOffset(inPropertyOffset), PropertyID(MakeUniqueID(inPropertyName)), IsPropertyTransient(InIsTransient) {
		String TypeName = RType::NormalizeTypeName(inPropertyType);
		if (!(PropertyType = RType::GetType(TypeName)))
			RClass::WaitTypeRegistration(TypeName, this, &RProperty::OnRegisterType);
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

	/**
	 * Is transient
	 */
	inline const bool IsTransient() const { return IsPropertyTransient; }

private:

	/**
	 * Bind property type
	 */
	inline void OnRegisterType(RType* RegisteredClass) { PropertyType = RegisteredClass; }

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

	/**
	 * Is property transient
	 */
	const bool IsPropertyTransient;
};