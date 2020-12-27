
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
		String Value = "Not Printable (type = Not Reflected)";
		return;
	}

	if (Type->GetTypeVariant() == ERType::ERType_RObject) {
		for (const auto& ParentClass : ((RClass*)Type)->GetParents()) PrintClassProperties(ParentClass, Type->CastTo(ParentClass, Object), PropertyName);

		for (const auto& Property : ((RClass*)Type)->GetProperties()) {
			RType* PropertyType = Property.second->GetType();
			PrintClassProperties((RClass*)PropertyType, Property.second->Get(Object), Type->GetName() + "::" + Property.second->GetName());
		}
	}
	else if (Type && Type->GetTypeVariant() == ERType::ERType_Array) {
		RArrayType* ArrayType = static_cast<RArrayType*>(Type);
		RArrayView View = ArrayType->GetView(Object);
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
		LOG(PropertyName + "->" + " = " + Value);
	}
}


int main() {

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
	ChildOneTwoObj->D.D = "CA MARCHE TROP BIEN";
	ChildOneTwoObj->ParentOne::A = 4;
	ChildOneTwoObj->ParentOne::B = 25;
	ChildOneTwoObj->ParentOne::C = 24;
	ChildOneTwoObj->ParentTwo::A = 12;
	ChildOneTwoObj->ParentTwo::B = 23;
	ChildOneTwoObj->ParentTwo::C = 38;

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

	/**
	 * Properties tests
	 */
	PrintClassProperties(MyClass, MyObject, "MyObject");
	/**
	 * Function tests
	 */	
	RFunction<void, ChildOneTwo>* FuncA = MyClass->GetFunction<void, ChildOneTwo>("FunctionA");
	RFunction<double, ChildOneTwo, int, int, int>* FuncB = MyClass->GetFunction<double, ChildOneTwo, int, int, int>("FunctionB");
	if (FuncA) LOG(FuncA->GetName()); FuncA->Execute((ChildOneTwo*)MyObject);
	if (FuncB) LOG(FuncB->GetName() + " : " + FuncB->Execute((ChildOneTwo*)MyObject, 10, 20, 30));
}