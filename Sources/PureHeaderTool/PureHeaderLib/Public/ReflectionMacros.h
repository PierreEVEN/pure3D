#pragma once

#define REFL_DECLARE_TYPENAME(Type) \
template<> \
struct RTypeName<Type> { \
	constexpr static const char* Name = #Type; \
};

#define REFL_REGISTER_TYPE(Type) RType::RegisterType(#Type, sizeof(Type));