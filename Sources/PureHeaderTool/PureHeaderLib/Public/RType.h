#pragma once

#include "ReflectionUtilities.h"

#include <utility>
#include <unordered_map>
#include <iostream>

class RType {
public:

    /**
     * Get type name
     */
    inline const char* GetName() const { return typeName; }

    /**
     * Get type by class
     */
    template<typename RClass>
    inline static const RType* GetType() { return GetType(RTypeName<RClass>::Name); }

    /**
     * Get type by class name
     */
    static const RType* GetType(const char* typeName);

    /**
     * Register new reflected type
     */
    static RType* RegisterType(const char* inTypeName, size_t inTypeSize);

    /**
     * Clear all types
     */
    static void DeleteTypes();

private:

	inline RType() = delete;
	inline RType(const RType&) = delete;
	inline RType(const RType&&) = delete;
	inline RType(const char* inTypeName, const size_t inTypeSize)
		: typeName(inTypeName), typeSize(inTypeSize) {}
	virtual ~RType() {}

    /**
     * Type name
     */
    const char* typeName;

    /**
     * Type size
     */
    const size_t typeSize;
};