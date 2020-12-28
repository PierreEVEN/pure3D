
#include "BasicObject.h"
#include "Reflection/RType.h"
#include "IO/Log.h"
#include "Reflection/RProperty.h"
#include "Reflection/RFunction.h"
#include "Reflection/RConstructor.h"
#include "Reflection/ReflectionMacros.h"
#include "Serialization/Serialization.h"
#include <fstream>
#include "Types/Array.h"
#include "Serialization/GenericSerializers.h"
#include "Primitives/PrimitiveTypes.h"
#include "Reflection/RArrayView.h"

void PrintClassProperties(RType* Type, void* Object, String PropertyName) {
	if (!Type) {
		LOG(PropertyName + " = None                   {Undetermined}");
		return;
	}

	if (Type->GetTypeVariant() == ERType::ERType_RObject) {
		for (const auto& ParentClass : ((RClass*)Type)->GetParents()) PrintClassProperties(ParentClass, Type->CastTo(ParentClass, Object), PropertyName);

		for (const auto& Property : ((RClass*)Type)->GetProperties()) {
			RType* PropertyType = Property.second->GetType();
			PrintClassProperties((RClass*)PropertyType, Property.second->Get(Object), PropertyName + "::" + Type->GetName() + "::" + Property.second->GetName());
		}
	}
	else if (Type && Type->GetTypeVariant() == ERType::ERType_Array) {
		RArrayType* ArrayType = static_cast<RArrayType*>(Type);
		RArrayView View = ArrayType->GetView(Object);
		LOG(PropertyName + " = Array:" + String(View.GetLength()) + "                   {" + Type->GetName() + "}");
		for (int i = 0; i < View.GetLength(); ++i) {
			PrintClassProperties(ArrayType->GetInnerType(), View.GetValuePtr(i), PropertyName + "[" + String(i) + "]");
		}
	}
	else {
		String Value = "Not Printable (type = " + Type->GetName() + ")";
		if (Type == RType::GetType<int32_t>()) Value = String(*reinterpret_cast<int32_t*>(Object));
		else if (Type == RType::GetType<double>()) Value = String(*reinterpret_cast<double*>(Object));
		else if (Type == RType::GetType<float>()) Value = String(*reinterpret_cast<float*>(Object));
		else if (Type == RType::GetType<bool>()) Value = String(*reinterpret_cast<bool*>(Object));
		else if (Type == RType::GetType<String>()) Value = String(*reinterpret_cast<String*>(Object));
		LOG(PropertyName + " = " + Value + "                   {" + Type->GetName() + "}");
	}
}

struct Obj {
	float Func() { 
		LOG("EXEC");
		return 0; }
};


template<typename ReturnType, typename Class, typename... Args>
using BaseFuncTest = ReturnType(Class::*) (Args...);


template<typename ReturnType, typename... Args>
using BaseFunc = std::function<ReturnType(Args...)>;

template<typename ReturnType, typename... Arguments>
static RFunction* MakeFunction_Internal(const String& InFunctionName, std::function<ReturnType(Arguments...)> InFunction) {
	return new RFunction(InFunctionName,
		std::make_any<BaseFunc<ReturnType, Arguments...>>(
			[InFunction](Arguments&&... inArguments) -> ReturnType {
				return InFunction(std::forward<Arguments>(inArguments)...);
			})
	);
}


template<typename ReturnType, typename Class, typename... Arguments>
static RFunction* MakeFunction(const String& InFunctionName, BaseFuncTest<ReturnType, Class, Arguments...> Var) {
	return MakeFunction_Internal<ReturnType, Class, Arguments...>(InFunctionName, Var);
}


int main() {

	//Obj obj;
	RFunction* Func = MakeFunction<float, Obj>("test", &Obj::Func);
	std::cout << Func->FunctionPointer.type().name() << std::endl;
	//if (!Func->IsValid<float, Obj>()) LOG_ASSERT("not valid");

	//Func->Execute<float>(&obj);

	//return 0;

	/**
	 * Class tests
	 */
	RClass* MyClass = ChildOneTwo::GetStaticClass();
	if (!MyClass) return 0;
	LOG(MyClass->GetName() + " size : " + MyClass->GetSize());

	/**
	 * Instantiate tests
	 */
	void* MyObject = MyClass->InstantiateNew<int, double, float>(5, 20.4, 3.5f);
	ChildOneTwo* ChildOneTwoObj = NewObject<ChildOneTwo>();

	/**
	 * Serialization tests
	 */
	ChildOneTwoObj->A = 10;
	ChildOneTwoObj->B = 40;
	ChildOneTwoObj->C = 50;
	ChildOneTwoObj->D.A = 123456;
	ChildOneTwoObj->D.B = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12} };
	ChildOneTwoObj->D.C = 5.323000001233;
	ChildOneTwoObj->D.D = "CA MARCHE TROP BIEN";
	ChildOneTwoObj->D.E = { 13, 14, 15, 16, 17, 18 };
	ChildOneTwoObj->ParentOne::A = 4;
	ChildOneTwoObj->ParentOne::B = 25;
	ChildOneTwoObj->ParentOne::C = 24;
	ChildOneTwoObj->ParentTwo::A = 12;
	ChildOneTwoObj->ParentTwo::B = 23;
	ChildOneTwoObj->ParentTwo::C = 38;

	for (int i = 0; i < 1; ++i) {

		// Serialize
		SArchive ArchiveSerialize;
		ArchiveSerialize.LinkObject("MyObject", MyClass, ChildOneTwoObj);

		std::ofstream output("test.pbin", std::ios::binary);
		ArchiveSerialize.Serialize(output);
		output.close();

		// Deserialize
		SArchive ArchiveDeserialize;
		ArchiveDeserialize.LinkObject("MyObject", MyClass, MyObject);

		std::ifstream input("test.pbin", std::ios::binary);
		ArchiveDeserialize.Deserialize(input);
		input.close();
	}

	/**
	 * Properties tests
	 */
	PrintClassProperties(MyClass, MyObject, "MyObject");

	/**
	 * Function tests
	 */
	RFunction* FuncA = MyClass->GetFunction<void>("FunctionA");
	RFunction* FuncB = MyClass->GetFunction<double, int, int, int>("FunctionB");

	if (FuncA) {
		FuncA->Execute<void>(MyObject);
	}
	if (FuncB) {
		LOG(FuncB->GetName() + " : " + FuncB->Execute<double, int, int, int>(MyObject, 10, 20, 40));
	}
}