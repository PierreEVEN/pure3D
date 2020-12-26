#pragma once

#include "Reflection/ReflectionMacros.h"
#include "Types/ByteArray.h"

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
REFL_DECLARE_TYPENAME(ByteArray);

namespace PrimitiveTypes {
	struct PrimitiveTypeRegisterer {
		PrimitiveTypeRegisterer();
	};
	inline static PrimitiveTypeRegisterer _PrimitiveTypeRegisterer;
}