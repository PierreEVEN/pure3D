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
	constexpr static const char* Name = #Type; \
};


#define REFL_REGISTER_TYPE(Type) RType::RegisterType<Type, RType>(#Type);

#define REFL_REGISTER_CLASS(Class) RClass::RegisterClass<Class>(#Class);


#define CONCAT_MACRO_TWO_PARAMS_(x,y) x##y
#define CONCAT_MACRO_TWO_PARAMS(x,y) CONCAT_MACRO_TWO_PARAMS_(x,y)
#define CONCAT_MACRO_THREE_PARAMS_(x,y,z) x##y##z
#define CONCAT_MACRO_THREE_PARAMS(x,y,z) CONCAT_MACRO_THREE_PARAMS(x,y,z)
#define CONCAT_MACRO_FOUR_PARAMS_(w,x,y,z) w##x##y##z
#define CONCAT_MACRO_FOUR_PARAMS(w,x,y,z) CONCAT_MACRO_FOUR_PARAMS_(w,x,y,z)


#define REFL_DECLARE_CLASS(className) \
public: \
	friend void CONCAT_MACRO_TWO_PARAMS(_Refl_Register_Item_,className)(); \
	friend void _Refl_Register_Class(); \
	static class RClass* GetStaticClass(); \
	virtual RClass* GetClass() const;

#define REFLECT(...)
#define RPROPERTY(...)
#define RCONSTRUCTOR(...)
#define RFUNCTION(...)

#define REFLECT_BODY() CONCAT_MACRO_FOUR_PARAMS(_REFLECTION_BODY_, _REFL_FILE_UNIQUE_ID_, _LINE_, __LINE__)
