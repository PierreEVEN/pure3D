
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

void PrintClassProperties(RClass* Type, void* Object) {
	for (const auto& ParentClass : Type->GetParents()) PrintClassProperties(ParentClass, Type->CastTo(ParentClass, Object));

	for (const auto& Property : Type->GetProperties()) {
		RType* PropertyType = Property.second->GetType();
		if (PropertyType && PropertyType->GetTypeVariant() == ERType::ERType_RObject) {
			LOG(Type->GetName() + "->" + Property.second->GetName() + " = Object '" + PropertyType->GetName() + "'");
			PrintClassProperties((RClass*)PropertyType, Property.second->Get(Object));
		}
		else {
			String Value = "Not Printable (type = " + (PropertyType ? PropertyType->GetName() : "Not Reflected") + ")";
			if (PropertyType == RType::GetType<int32_t>()) Value = String(*Property.second->Get<int32_t>(Object));
			else if (PropertyType == RType::GetType<double>()) Value = String(*Property.second->Get<double>(Object));
			else if (PropertyType == RType::GetType<float>()) Value = String(*Property.second->Get<float>(Object));
			else if (PropertyType == RType::GetType<bool>()) Value = String(*Property.second->Get<bool>(Object));
			else if (PropertyType == RType::GetType<String>()) Value = String(*Property.second->Get<String>(Object));
			LOG(Type->GetName() + "->" + Property.second->GetName() + " = " + Value);
		}
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
	PrintClassProperties(MyClass, MyObject);

	/**
	 * Function tests
	 */	
	RFunction<void, ChildOneTwo>* FuncA = MyClass->GetFunction<void, ChildOneTwo>("FunctionA");
	RFunction<double, ChildOneTwo, int, int, int>* FuncB = MyClass->GetFunction<double, ChildOneTwo, int, int, int>("FunctionB");
	if (FuncA) LOG(FuncA->GetName()); FuncA->Execute((ChildOneTwo*)MyObject);
	if (FuncB) LOG(FuncB->GetName() + " : " + FuncB->Execute((ChildOneTwo*)MyObject, 10, 20, 30));
}