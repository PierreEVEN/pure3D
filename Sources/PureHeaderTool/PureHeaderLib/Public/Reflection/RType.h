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
    ERType_RObject,
    ERType_Array,
    ERType_Map,
    ERType_Set
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

    /**
     * Get type Id
     */
    inline RUID GetId() const { return TypeId; }

    /**
     * Set this class serializer
     */
    inline virtual void SetSerializer(ISerializerInterface* Serializer) { ClassSerializer = Serializer; }

    /**
     * Get this class serializer
     */
    ISerializerInterface* GetSerializer() const { return ClassSerializer; }

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
    static RType* GetType(RUID InTypeId);

    /**
     * Get type variant
     */
    inline virtual const ERType GetTypeVariant() const { return ERType::ERType_RType; }

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

    /**
     * Wait for given type to be registered
     */
    template <typename WaitTypeName>
    inline static void WaitTypeRegistration(String inClassName, WaitTypeName* inObjPtr, void(WaitTypeName::* inFunc)(RType*)) {
        TypeRegistrationDelegate[MakeUniqueID(inClassName)].Add(inObjPtr, inFunc);
    }

    /**
     * Cast given pointer to this type (mainly used for multiple inheritance)
     */
    inline virtual void* CastTo(const RType* To, void* Ptr) { return Ptr; }

    /**
     * Create default object
     */
	virtual void* NewType() { return new char[TypeSize]; }

    /**
     * Delete type
     */
	virtual void DeleteType(void* Memory) { return delete(Memory); }

    inline static void AddAlias(const String& From, const String& To) { Alias[From] = To; }

    static String NormalizeTypeName(const String& TypeName);

protected:

	RType(const String& inTypeName, const size_t inTypeSize)
		: TypeName(inTypeName), TypeSize(inTypeSize), ClassSerializer(nullptr), TypeId(MakeUniqueID(inTypeName)) {}

	virtual ~RType() = default;

private:

    static void RegisterType_Internal(const String& inTypeName, RType* newType);
        
	inline static std::unordered_map<RUID, SOnRegisterType> TypeRegistrationDelegate;

    inline static std::unordered_map<String, String> Alias;

    /**
     * Type serializer
     */
    ISerializerInterface* ClassSerializer;

    /**
     * Type name
     */
    const String TypeName;

    /**
     * Type byte size
     */
    const size_t TypeSize;

    /**
     * Type Id (hash of type name)
     */
    const RUID TypeId;
};