#pragma once

#include "RType.h"

#include <vector>
#include <unordered_map>
#include "RFunction.h"
#include "RConstructor.h"
#include "Events/EventManager.h"

struct RProperty;
struct RClass;
struct RCastFunction;

struct RClass : public RType {

	friend RType;
	using RCastFunc = std::function<void* (const RClass*, void*)>;

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
     * Add reflected function for this class
     */
    void AddFunction(RFunction* inFunction);

    /**
     * Add reflected constructor for this class
     */
    void AddConstructor(RConstructor* inConstructor) { Constructors.push_back(inConstructor); }

    /**
     * Add function that FromPtr from ThisClass to ParentClass
     */
	template<typename ThisClass, typename ParentClass>
    inline void AddCastFunction() {
        if constexpr (RIsReflected<ParentClass>::Value) {
            CastFunctions[MakeUniqueID<ParentClass>()] = RCastFunc(
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
        if (To->GetTypeVariant() != ERType::ERType_RObject) return Ptr;
        for (const auto& Parent : Parents)
            if (void* ToPtr = (CastFunctions[Parent->GetId()]) ((RClass*)To, Ptr)) 
                return ToPtr;
        return nullptr;
    }

    /**
     * Get type variant
     */
	inline virtual const ERType GetTypeVariant() const override { return ERType::ERType_RObject; }

    /**
     * Get properties from RUID or Name
     */
	inline std::unordered_map<RUID, RProperty*> GetProperties() const { return Properties; }
    inline RProperty* GetProperty(const String& PropertyID) const { return GetProperty(MakeUniqueID(PropertyID));  }
	RProperty* GetProperty(RUID PropertyID) const;

    /**
     * Get function from RUID or Name
	 */
	inline std::unordered_map<RUID, RFunction*> GetFunctions() const { return Functions; }
	inline RFunction* GetFunction(const String& FunctionName) const { return GetFunction(MakeUniqueID(FunctionName)); }
	RFunction* GetFunction(RUID FunctionID) const;

	template<typename ReturnType, typename... Arguments>
	RFunction* GetFunction(RUID FunctionID) {
		if (RFunction* Function = GetFunction(FunctionID))
			return Function->IsValid<ReturnType, Arguments...>() ? Function : nullptr;
		return nullptr;
	}
	template<typename ReturnType, typename... Arguments>
    RFunction* GetFunction(String FunctionName) {
		if (RFunction* Function = GetFunction(FunctionName))
			return Function->IsValid<ReturnType, Arguments...>() ? Function : nullptr;
		return nullptr;
	}

    /**
     * Get parent classes
     */
	const std::vector<RClass*>& GetParents() const { return Parents; }

    template<typename... Arguments>
    inline void* InstantiateNew(Arguments&&... inArguments) {
        for (const auto& Ctor : Constructors) {
            if (void* InstanciedObject = Ctor->InstanciateNew<Arguments...>(std::forward<Arguments>(inArguments)...))
                return InstanciedObject;
        }
        return nullptr;
	}

	virtual void* NewType();
    virtual void DeleteType(void* Memory);

private:

	inline RClass(const String& inTypeName, size_t inTypeSize) : RType(inTypeName, inTypeSize) {}

    static void RegisterClass_Internal(RClass* inClass);

    inline void OnRegisterParentClass(RType* RegisteredClass);

    /**
     * Cast functions to parent types
     */
    std::unordered_map<RUID, RCastFunc> CastFunctions;

    /**
     * Class properties
     */
    std::unordered_map<RUID, RProperty*> Properties;

    /**
     * Class Functions
     */
    std::unordered_map<RUID, RFunction*> Functions;
    /**
     * Class Constructors
     */
    std::vector<RConstructor*> Constructors;

    /**
     * Parent classes
     */
	std::vector<RClass*> Parents;
};

/**
 * Create a new object of 'inClass' type
 */
template<typename T = void, typename... Arguments>
T* NewObject(RClass* inClass, Arguments... inArguments) {
	return reinterpret_cast<T*>(inClass->InstantiateNew<Arguments...>(std::forward<Arguments>(inArguments)...));
}

/**
 * Create a new object of T type
 */
template<typename T = void, typename... Arguments>
T* NewObject(Arguments... inArguments) {
	RClass* ObjectClass = T::GetStaticClass();
	return reinterpret_cast<T*>(ObjectClass->InstantiateNew<Arguments...>(std::forward<Arguments>(inArguments)...));
}