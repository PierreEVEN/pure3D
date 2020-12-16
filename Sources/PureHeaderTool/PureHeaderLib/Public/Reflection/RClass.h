#pragma once

#include "RType.h"

#include <vector>
#include <unordered_map>
#include "RFunction.h"
#include "RConstructor.h"
#include "Events/EventManager.h"
#include "IO/Log.h"

struct RProperty;
struct IFunctionPointer;
struct RClass;
struct RCastFunction;

DECLARE_DELEGATE_MULTICAST(SOnRegisterClass, const RClass*);

struct RClass : public RType {

    friend RType;

    /**
     * Get class from type
     */
    template<typename Class>
    inline static const RClass* GetClass() {
		static_assert(RIsReflected<Class>::Value, "Failed to get class : not a reflected class. Please declare this class as a reflected class.");
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
        static_assert(RIsReflected<Class>::Value, "Failed to register class : not a reflected class. Please declare this class as a reflected class.");
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
    void AddConstructor(RConstructor* inConstructor) {
        Constructors.push_back(inConstructor);
    }

	using RCastFunc = std::function<void* (const RClass*, void*)>;

    /**
     * Add function that FromPtr from ThisClass to ParentClass
     */
	template<typename ThisClass, typename ParentClass>
    inline void AddCastFunction() {
        CastFunctions[RClass::GetClass<ParentClass>()->GetName()] = RCastFunc(
            [](const RClass* DesiredClass, void* FromPtr) -> void* {
                return ParentClass::GetStaticClass()->CastTo(DesiredClass, reinterpret_cast<void*>(static_cast<ParentClass*>((ThisClass*)FromPtr)));
            }
        );
    }

    /**
     * Cast Ptr to To Object
     * if ThisClass == To, return Ptr, else try to cast to one of the parent class
     */
    void* CastTo(const RClass* To, void* Ptr) {
		if (To == this) return Ptr;
        for (const auto& Parent : Parents)
            if (void* ToPtr = (CastFunctions[Parent->GetName()]) (To, Ptr)) 
                return ToPtr;
        return nullptr;
    }

    template<typename ReturnType, typename Class, typename... Arguments>
    RFunction<ReturnType, Class, Arguments...>* GetFunction(const String& PropertyName) const {
        return (RFunction<ReturnType, Class, Arguments...>*)GetFunction(PropertyName);
    }

	IFunctionPointer* GetFunction(const String& PropertyName) const;

    RProperty* GetProperty(const String& PropertyName) const;

    template<typename... Arguments>
    inline void* InstantiateNew(Arguments&&... inArguments) {
        for (const auto& Ctor : Constructors) {
            if (void* InstanciedObject = Ctor->InstanciateNew<Arguments...>(std::forward<Arguments>(inArguments)...))
                return InstanciedObject;
        }
        return nullptr;
	}

	template <typename WaitObjectName>
    inline static void WaitClassRegistration(String inClassName, WaitObjectName* inObjPtr, void(WaitObjectName::* inFunc)(const RClass*)) {
        ClassRegistrationDelegate[inClassName].Add(inObjPtr, inFunc);
    }

    const std::vector<const RClass*>& GetParents() const { return Parents; }

private:

	inline RClass(const String& inTypeName, size_t inTypeSize)
		: RType(inTypeName, inTypeSize) {}

    static void RegisterClass_Internal(const String& inClassName, const RClass* inClass);

    inline static std::unordered_map<String, SOnRegisterClass> ClassRegistrationDelegate;

	inline static std::unordered_map<String, const RClass*>* Classes = nullptr;

    inline void OnRegisterParentClass(const RClass* RegisteredClass);

    std::unordered_map<String, RCastFunc> CastFunctions;
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
    std::vector<RConstructor*> Constructors;

    /**
     * Parent classes
     */
	std::vector<const RClass*> Parents;
};

template<typename T = void, typename... Arguments>
T* NewObject(RClass* inClass, Arguments... inArguments) {
	return reinterpret_cast<T*>(inClass->InstantiateNew<Arguments...>(std::forward<Arguments>(inArguments)...));
}
