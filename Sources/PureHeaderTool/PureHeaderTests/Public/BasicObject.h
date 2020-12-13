#pragma once

#include "Reflection/ReflectionUtilities.h"
#include "Reflection/ReflectionMacros.h"
#include <unordered_map>

REFLECT();
enum class BasicEnum { BASIC_ONE, BASIC_TWO, BASIC_THREE };

REFLECT();
enum class AdvancedEnum {
	ADV_ONE = 1,
	ADV_TWO = 1 << 1,
	ADV_THREE = 1 << 2
};

REFLECT();
struct BasicStructure final {

	REFLECT_BODY();

	RCONSTRUCTOR()
		BasicStructure() {}

	RCONSTRUCTOR()
		BasicStructure(int inA, std::vector<double> inB) : A(inA), B(inB) {}

	RPROPERTY()
		int A = 10;

	RPROPERTY()
		std::vector<double> B = { 2.0, 5.185, 12.4 };

	RPROPERTY()
		const double& C = 3.4;

	RFUNCTION()
		void TestFuncA() {}

	RFUNCTION()
		int TestFuncB(bool inA, float inB, void* inC, std::vector<double> inD) { return 0;  }
};

REFLECT();
class BasicObject {

	REFLECT_BODY();

	RCONSTRUCTOR()
		BasicObject() {}

	RCONSTRUCTOR()
		BasicObject(std::unordered_map<float, int> inA, float inB) : A(inA), B(inB) {}

	RPROPERTY()
		std::unordered_map<float, int> A = { {10.f, 5}, {8.4f, 3} };

	RPROPERTY()
		float B = 5.5f;

	RPROPERTY()
		double* C = nullptr;

	RFUNCTION()
		void TestFuncA () {}

	RFUNCTION()
		const int* TestFuncB(std::unordered_map<float, int> inA, int inB) { return nullptr; }
};