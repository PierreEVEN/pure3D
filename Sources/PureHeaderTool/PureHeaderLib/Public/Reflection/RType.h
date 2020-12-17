#pragma once

#include "ReflectionUtilities.h"

#include "Types/String.h"
#include "Serialization.h"
#include "Events/EventManager.h"


DECLARE_DELEGATE_MULTICAST(SOnRegisterType, RType*);

enum class ERType {
    ERType_RType,
    ERType_RClass
};

struct RType : public ReflectionObject {
    /**
     * Get type name
     */
    inline const String& GetName() const { return TypeName; }

    /**
     * Get type size
     */
    inline const size_t& GetSize() const { return TypeSize; }

    inline virtual void SetSerializer(ISerializerInterface* Serializer) {
        ClassSerializer = Serializer;
    }

    /**
     * Get type by class
     */
    template<typename Type>
	inline static RType* GetType() {
		static_assert(RIsReflected<Type>::Value, "Not a reflected type, please declare this type as a reflected type.");
        return GetType(RTypeName<Type>::Name);
    }

    /**
     * Get type by class name
     */
    static RType* GetType(const String& inTypeName);

    /**
     * Register new reflected type
     */
    template<typename Class, typename Type = RType>
    inline static Type* RegisterType(const String& inTypeName) {
		static_assert(RIsReflected<Class>::Value, "Not a reflected type, please declare this type as a reflected type.");
        Type* newType = new Type(inTypeName, sizeof(Class));
        RegisterType_Internal(inTypeName, newType);
        return newType;
    }

    ISerializerInterface* GetSerializer() const {
        return ClassSerializer;
    }

    inline virtual const ERType GetType() const { return ERType::ERType_RType; }

	template <typename WaitTypeName>
	inline static void WaitTypeRegistration(String inClassName, WaitTypeName* inObjPtr, void(WaitTypeName::* inFunc)(RType*)) {
		TypeRegistrationDelegate[inClassName].Add(inObjPtr, inFunc);
	}

protected:

    inline RType(const String& inTypeName, const size_t inTypeSize)
        : TypeName(inTypeName), TypeSize(inTypeSize), ClassSerializer(nullptr) {}

	virtual ~RType() = default;

private:

    static void RegisterType_Internal(const String& inTypeName, RType* newType);

	inline static std::unordered_map<String, SOnRegisterType> TypeRegistrationDelegate;

    ISerializerInterface* ClassSerializer;

    /**
     * Type name
     */
    const String TypeName;

    /**
     * Type size
     */
    const size_t TypeSize;
};