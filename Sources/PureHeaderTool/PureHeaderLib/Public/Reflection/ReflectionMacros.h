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

#define REFLECT_BODY();

#define REFLECT(...)
#define RPROPERTY(...)
#define RCONSTRUCTOR(...)
#define RFUNCTION(...)