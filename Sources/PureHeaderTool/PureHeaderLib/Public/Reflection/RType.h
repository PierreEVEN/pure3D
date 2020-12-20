#pragma once

#include "ReflectionUtilities.h"

#include "Types/String.h"
#include "Events/EventManager.h"
#include <type_traits>
#include <unordered_map>

struct ISerializerInterface;
struct RType;

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
     * Get type by class name
     */
    static RType* GetType(size_t InTypeId);

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

    inline virtual const ERType GetTypeVariant() const { return ERType::ERType_RType; }

	template <typename WaitTypeName>
	inline static void WaitTypeRegistration(String inClassName, WaitTypeName* inObjPtr, void(WaitTypeName::* inFunc)(RType*)) {
        TypeRegistrationDelegate[std::hash<String>{}(inClassName)].Add(inObjPtr, inFunc);
	}

    inline size_t GetId() const { return TypeId; }

    template<typename Type>
    inline static size_t MakeTypeID() { return std::hash<String>{}(RTypeName<Type>::Name); }

    inline virtual void* CastTo(const RType* To, void* Ptr) { return Ptr; }

protected:

	RType(const String& inTypeName, const size_t inTypeSize)
		: TypeName(inTypeName), TypeSize(inTypeSize), ClassSerializer(nullptr), TypeId(std::hash<String>{}(inTypeName)) {}

	virtual ~RType() = default;

private:

    static void RegisterType_Internal(const String& inTypeName, RType* newType);

	inline static std::unordered_map<size_t, SOnRegisterType> TypeRegistrationDelegate;

    ISerializerInterface* ClassSerializer;

    /**
     * Type name
     */
    const String TypeName;

    /**
     * Type size
     */
    const size_t TypeSize;

    /**
     * TypeId
     */
    const size_t TypeId;
};