#pragma once

#include "RType.h"

#include <vector>
#include <unordered_map>
#include "RFunction.h"
#include "RConstructor.h"
#include "Events/EventManager.h"

struct RProperty;
struct IFunctionPointer;
struct RClass;
struct RCastFunction;

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
    static RClass* GetClass(const String& inClassName);

    /**
     * Get class from name
     */
    static RClass* GetClass(size_t inClassId);

    /**
     * Register class
     */
    template<typename Class>
    static RClass* RegisterClass(const String& inClassName) {
        static_assert(RIsReflected<Class>::Value, "Failed to register class : not a reflected class. Please declare this class as a reflected class.");
        RClass* RegisteredClass = RType::RegisterType<Class, RClass>(inClassName);
        RegisterClass_Internal(RegisteredClass);
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
        if constexpr (RIsReflected<ParentClass>::Value) {
            CastFunctions[RType::MakeTypeID<ParentClass>()] = RCastFunc(
                [](const RClass* DesiredClass, void* FromPtr) -> void* {
                    return ParentClass::GetStaticClass()->CastTo(DesiredClass, reinterpret_cast<void*>(static_cast<ParentClass*>((ThisClass*)FromPtr)));
                }
            );
        }
    }

    /**
     * Cast Ptr to To Object
     * if ThisClass == To, return Ptr, else try to cast to one of the parent class
     */
    inline virtual void* CastTo(const RType* To, void* Ptr) override {
		if (To == this) return Ptr;
        if (To->GetTypeVariant() != ERType::ERType_RClass) return Ptr;
        for (const auto& Parent : Parents)
            if (void* ToPtr = (CastFunctions[Parent->GetId()]) ((RClass*)To, Ptr)) 
                return ToPtr;
        return nullptr;
    }

	inline virtual const ERType GetTypeVariant() const override { return ERType::ERType_RClass; }

    template<typename ReturnType, typename Class, typename... Arguments>
    RFunction<ReturnType, Class, Arguments...>* GetFunction(const String& PropertyName) const {
        return (RFunction<ReturnType, Class, Arguments...>*)GetFunction(PropertyName);
    }

	IFunctionPointer* GetFunction(const String& PropertyName) const;

	RProperty* GetProperty(size_t PropertyName) const;

    std::unordered_map<size_t, RProperty*> GetProperties() { return Properties; }

    template<typename... Arguments>
    inline void* InstantiateNew(Arguments&&... inArguments) {
        for (const auto& Ctor : Constructors) {
            if (void* InstanciedObject = Ctor->InstanciateNew<Arguments...>(std::forward<Arguments>(inArguments)...))
                return InstanciedObject;
        }
        return nullptr;
	}

    const std::vector<RClass*>& GetParents() const { return Parents; }

private:

	inline RClass(const String& inTypeName, size_t inTypeSize)
		: RType(inTypeName, inTypeSize) {}

    static void RegisterClass_Internal(RClass* inClass);

    inline void OnRegisterParentClass(RType* RegisteredClass);

    std::unordered_map<size_t, RCastFunc> CastFunctions;
    /**
     * Class properties
     */
    std::unordered_map<size_t, RProperty*> Properties;

    /**
     * Class properties
     */
    std::unordered_map<size_t, IFunctionPointer*> Functions;
    /**
     * Class properties
     */
    std::vector<RConstructor*> Constructors;

    /**
     * Parent classes
     */
	std::vector<RClass*> Parents;
};

template<typename T = void, typename... Arguments>
T* NewObject(RClass* inClass, Arguments... inArguments) {
	return reinterpret_cast<T*>(inClass->InstantiateNew<Arguments...>(std::forward<Arguments>(inArguments)...));
}

template<typename T = void, typename... Arguments>
T* NewObject(Arguments... inArguments) {
	return reinterpret_cast<T*>(T::GetStaticClass()->InstantiateNew<Arguments...>(std::forward<Arguments>(inArguments)...));
}