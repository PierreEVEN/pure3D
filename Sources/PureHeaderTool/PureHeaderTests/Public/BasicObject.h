#pragma once

#include "Reflection/ReflectionUtilities.h"
#include "Reflection/ReflectionMacros.h"
#include <unordered_map>

#include "BasicObject.refl.h"
#include "IO/Log.h"

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
		virtual void TestFuncA () {}

	RFUNCTION()
		virtual const int* TestFuncB(std::unordered_map<float, int> inA, int inB) { return nullptr; }
};

REFLECT();
struct ParentOne {

	REFLECT_BODY();

	RCONSTRUCTOR()
		ParentOne() = default;
	RCONSTRUCTOR()
		ParentOne(int inA, double inB, float inC) : A(inA), B(inB), C(inC) {}

	RPROPERTY()
		int A = 24;

	RPROPERTY()
		double B = 32.897;

	RPROPERTY()
		float C = 8.4f;

	RFUNCTION()
		void FunctionA() {}

	RFUNCTION()
		virtual double FunctionB(int A, int B, int C) { return 12.87; }
};

REFLECT();
struct ParentTwo {
	REFLECT_BODY();

	RCONSTRUCTOR()
		ParentTwo() = default;

	RCONSTRUCTOR()
		ParentTwo(int inA, double inB, float inC) : A(inA), B(inB), C(inC) {}

	RPROPERTY()
		int A = 25;

	RPROPERTY()
		double B = 38.897;

	RPROPERTY()
		float C = 7.4f;

	RFUNCTION()
		virtual void FunctionA() {
		LOG("CALL FUNCTION A");
	}

	RFUNCTION()
		double FunctionB(int A, int B, int C) { return 20.8; }
};

REFLECT();
struct ChildOne : public ParentOne {
	REFLECT_BODY();

	RCONSTRUCTOR()
		ChildOne() = default;
	RCONSTRUCTOR()
		ChildOne(int inA, double inB, float inC) : A(inA), B(inB), C(inC) {}

	RPROPERTY()
		int A = 18;

	RPROPERTY()
		double B = 16.897;

	RPROPERTY()
		float C = 5.4f;

	RFUNCTION()
		virtual void FunctionA() {}

	RFUNCTION()
		double FunctionB(int A, int B, int C) override { return ParentOne::FunctionB(A, B, C);  }
};

REFLECT();
struct ChildTwo : public ParentTwo {
	REFLECT_BODY();

	RCONSTRUCTOR()
		ChildTwo() = default;
	RCONSTRUCTOR()
		ChildTwo(int inA, double inB, float inC) : A(inA), B(inB), C(inC) {}

	RPROPERTY()
		int A = 18;

	RPROPERTY()
		double B = 16.897;

	RPROPERTY()
		float C = 5.4f;

	RFUNCTION()
		virtual void FunctionA() override { return ParentTwo::FunctionA();  }

	RFUNCTION()
		double FunctionB(int A, int B, int C) { return 12.87; }
};

REFLECT();
struct ChildOneTwo : public ParentOne, public ParentTwo {

	REFLECT_BODY();

	RCONSTRUCTOR()
		ChildOneTwo() = default;
	RCONSTRUCTOR()
	ChildOneTwo(int inA, double inB, float inC) : A(inA), B(inB), C(inC) {
		LOG("create ChildOneTwo : " + String(A) + " " + B + " " + C);
	}

	RPROPERTY()
		int A = 18;

	RPROPERTY()
		double B = 16.897;

	RPROPERTY()
		float C = 5.4f;

	RPROPERTY()
		BasicStructure D;

	RFUNCTION()
		virtual void FunctionA() override {
		return ParentTwo::FunctionA();
	}

	RFUNCTION()
		virtual double FunctionB(int inA, int inB, int inC) override { return ParentOne::FunctionB(inA, inB, inC); }
};