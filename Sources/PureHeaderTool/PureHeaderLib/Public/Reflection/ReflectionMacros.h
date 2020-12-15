#pragma once
#include "RClass.h"
#include "Reflection/ReflectionUtilities.h"

#define REFL_DECLARE_TYPENAME(Type) \
template<> \
struct RIsReflected<Type> { \
	constexpr static bool Value = true; \
}; \
 \
template<> \
struct RTypeName<Type> {	\
	constexpr static char* Name = #Type; \
};


#define REFL_REGISTER_TYPE(Type) RType::RegisterType<Type, RType>(#Type);

#define REFL_REGISTER_CLASS(Class) RClass::RegisterClass<Class>(#Class);

#define CONCAT_(x,y) x##y
#define CONCAT(x,y) CONCAT_(x,y)

#define REFLECT_BODY() CONCAT(CONCAT(CONCAT(_REFLECTION_BODY_, _REFL_FILE_UNIQUE_ID_), _LINE_), __LINE__)

#define REFLECT(...)
#define RPROPERTY(...)
#define RCONSTRUCTOR(...)
#define RFUNCTION(...)

#define REFL_DECLARE_CLASS(className) \
public: \
	friend void _Refl_Register_Item_##className##(); \
	friend void _Refl_Register_Class(); \
	static class RClass* GetStaticClass(); \
	virtual RClass* GetClass() const; \