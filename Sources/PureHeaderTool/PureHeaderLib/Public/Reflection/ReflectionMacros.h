#pragma once
#include "RClass.h"

#define REFL_DECLARE_TYPENAME(Type) \
template<> \
struct RTypeName<Type> { \
	constexpr static const char* Name = #Type; \
}; \
 \
template<> \
struct RIsReflected<Type> { \
	constexpr static const bool Value = true; \
}; \


#define REFL_REGISTER_TYPE(Type) RType::RegisterType<Type, RType>(#Type);

#define REFL_REGISTER_CLASS(Class) RClass::RegisterClass<Class>(#Class);

#define REFLECT_BODY();

#define REFLECT(...)
#define RPROPERTY(...)
#define RCONSTRUCTOR(...)
#define RFUNCTION(...)