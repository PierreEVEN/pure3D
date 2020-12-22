#pragma once

#include "Reflection/ReflectionMacros.h"

REFL_DECLARE_TYPENAME(bool);
REFL_DECLARE_TYPENAME(int8_t);
REFL_DECLARE_TYPENAME(int16_t);
REFL_DECLARE_TYPENAME(int32_t);
REFL_DECLARE_TYPENAME(int64_t);
REFL_DECLARE_TYPENAME(uint8_t);
REFL_DECLARE_TYPENAME(uint16_t);
REFL_DECLARE_TYPENAME(uint32_t);
REFL_DECLARE_TYPENAME(uint64_t);
REFL_DECLARE_TYPENAME(float);
REFL_DECLARE_TYPENAME(double);
REFL_DECLARE_TYPENAME(String);

#ifndef REFL_DECLARE_TYPENAME_std__vector_double_
	#define REFL_DECLARE_TYPENAME_std__vector_double_
	REFL_DECLARE_TYPENAME(std::vector<double>);
#endif // REFL_DECLARE_TYPENAME_std__vector_double_

struct PrimitiveTypeRegisterer {
	PrimitiveTypeRegisterer();
};