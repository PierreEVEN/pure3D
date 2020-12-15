#pragma once

#include "RType.h"

#include <vector>
#include <unordered_map>
#include "RFunction.h"

struct RProperty;
struct IFunctionPointer;

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
    void AddProperty(RProperty* inProperty);

    /**
     * Add reflected property for this class
     */
    void AddFunction(IFunctionPointer* inProperty);

    /**
     * Add reflected property for this class
     */
    void AddConstructor(IFunctionPointer* inConstructor);

    template<typename ReturnType, typename Class, typename... Arguments>
    RFunction<ReturnType, Class, Arguments...>* GetFunction(const String& PropertyName) const {
        return (RFunction<ReturnType, Class, Arguments...>*)GetFunction(PropertyName);
    }

	IFunctionPointer* GetFunction(const String& PropertyName) const;
	IFunctionPointer* GetConstructor(const String& PropertyName) const;

    RProperty* GetProperty(const String& PropertyName) const;

    template<typename Type = void, typename... Arguments>
    inline Type* InstantiateNew(Arguments&&... inArguments) {


        //return static_cast<T*>()
        return nullptr;
    }


private:

	inline RClass(const String& inTypeName, size_t inTypeSize)
		: RType(inTypeName, inTypeSize) {}

    static void RegisterClass_Internal(const String& inClassName, const RClass* inClass);

    /**
     * Class properties
     */
    std::unordered_map<String, RProperty*> Properties;

    /**
     * Class properties
     */
    std::unordered_map<String, IFunctionPointer*> Functions;
    /**
     * Class properties
     */
    std::unordered_map<String, IFunctionPointer*> Constructors;
    /**
     * Parent classes
     */
	std::vector<const RClass*> Parents;
};