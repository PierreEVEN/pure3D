#pragma once

#include "Reflection/ReflectionUtilities.h"
#include "Reflection/ReflectionMacros.h"

REFLECT();
enum class BasicEnum { BAISC_ONE, BASIC_TWO, BASIC_THREE };

REFLECT();
enum class AdvancedEnum {
	ADV_ONE = 1,
	ADV_TWO = 1 << 1,
	ADV_THREE = 1 << 2
};

REFLECT();
struct BasicStructure {

	REFLECT_BODY();

	RCONSTRUCTOR()
		BasicStructure() {}

	RCONSTRUCTOR()
		BasicStructure(int inA, float inB) : A(inA), B(inB) {}

	RPROPERTY()
		int A = 10;

	RPROPERTY()
		float B = 5.5f;

	RPROPERTY()
		double C = 3.4;

	RFUNCTION()
		void TestFuncA() {}

	RFUNCTION()
		int TestFuncB(bool inA, int inB) { return 0;  }
};

REFLECT();
class BasicObject {

	REFLECT_BODY();

	RCONSTRUCTOR()
		BasicObject() {}

	RCONSTRUCTOR()
		BasicObject(int inA, float inB) : A(inA), B(inB) {}

	RPROPERTY()
		int A = 10;

	RPROPERTY()
		float B = 5.5f;

	RPROPERTY()
		double C = 3.4;

	RFUNCTION()
		void TestFuncA() {}

	RFUNCTION()
		int TestFuncB(bool inA, int inB) { return 0; }
};