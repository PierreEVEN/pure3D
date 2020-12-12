#pragma once

#include "ReflectionUtilities.h"

#include "Types/String.h"

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
     * Get type by class
     */
    template<typename Type>
	inline static const RType* GetType() {
		static_assert(RIsReflected<Type>::Value, "Not a reflected type, please declare this type as a reflected type.");
        return GetType(RTypeName<Type>::Name);
    }

    /**
     * Get type by class name
     */
    static const RType* GetType(const String& inTypeName);

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

protected:

    inline RType(const String& inTypeName, const size_t inTypeSize)
        : TypeName(inTypeName), TypeSize(inTypeSize) {}
	virtual ~RType() = default;

private:

    static void RegisterType_Internal(const String& inTypeName, const RType* newType);

    /**
     * Type name
     */
    const String TypeName;

    /**
     * Type size
     */
    const size_t TypeSize;
};