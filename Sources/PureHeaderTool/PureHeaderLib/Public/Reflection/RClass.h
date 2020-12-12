#pragma once

#include "RType.h"

#include <vector>

struct RProperty;

struct RClass : public RType {

    friend RType;

    /**
     * Get class from type
     */
    template<typename Class>
    inline static const RClass* GetClass() {
		static_assert(RIsReflected<Class>::Value, "Not a reflected class, please declare this class as a reflected class.");
        return GetClass(RTypeName<Class>::Name);
    }

    /**
     * Get class from name
     */
    static const RClass* GetClass(const String& inClassName);

    /**
     * Register class
     */
    template<typename Class>
    static RClass* RegisterClass(const String& inClassName) {
        static_assert(RIsReflected<Class>::Value, "Not a reflected class, please declare this class as a reflected class.");

        RClass* RegisteredClass = RType::RegisterType<Class, RClass>(inClassName);
        RegisterClass_Internal(inClassName, RegisteredClass);
        return RegisteredClass;
	}

    /**
     * Register a parent class for this class
     */
	void AddParent(const String&  inParent);

    /**
     * Add reflected property for this class
     */
    inline void AddProperty(RProperty* inProperty) {
        Properties.push_back(inProperty);
    }

private:

	inline RClass(const String& inTypeName, size_t inTypeSize)
		: RType(inTypeName, inTypeSize) {}

    static void RegisterClass_Internal(const String& inClassName, const RClass* inClass);

    /**
     * Class properties
     */
    std::vector<RProperty*> Properties;

    /**
     * Parent classes
     */
	std::vector<const RClass*> Parents;
};