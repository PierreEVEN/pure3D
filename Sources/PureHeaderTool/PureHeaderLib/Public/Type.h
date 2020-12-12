#ifndef PURE3D_TYPE_H
#define PURE3D_TYPE_H

#include <utility>
#include <unordered_map>
#include <iostream>

template<typename CppClass>
class TypeName {};

class Type {
public:
    virtual ~Type() {}

    inline const char* GetName() const { return typeName; }

    template<typename CppClass>
    inline static const Type* GetType() {
        return GetType(TypeName<CppClass>::name);
    }

    inline static const Type* GetType(const char* typeName) {
        const auto& value = types.find(typeName);
        if (value != types.end()) return value->second;
        return nullptr;
    }

    static void RegisterType(const char* inTypeName, size_t inTypeSize) {
        if (GetType(inTypeName)) {
            std::cout << inTypeName << " is already registered" << std::endl;
            exit(1);
        }

        types[inTypeName] = new Type(inTypeName, inTypeSize);
    }

private:

    inline Type() = delete;
    inline Type(const Type&&) = delete;

    inline Type(const char* inTypeName, const size_t inTypeSize)
    : typeName(inTypeName), typeSize(inTypeSize) {}

    inline static std::unordered_map<const char*, const Type*> types;

    const char* typeName;
    const size_t typeSize;
};




#endif