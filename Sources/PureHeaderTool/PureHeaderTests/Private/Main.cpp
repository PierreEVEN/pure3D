
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
	ChildOneTwoObj->ParentOne::A = 8;
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
	 * Inheritance test
	 */
	for (const auto& ParentClass : MyClass->GetParents()) {
		LOG("Get properties for : " + ParentClass->GetName());
		void* ParentClassPtr = MyClass->CastTo(ParentClass, MyObject);
		RProperty* ParentPropertyA = ParentClass->GetProperty(MakeUniqueID("A"));
		RProperty* ParentPropertyB = ParentClass->GetProperty(MakeUniqueID("B"));
		RProperty* ParentPropertyC = ParentClass->GetProperty(MakeUniqueID("C"));
		if (ParentPropertyA) LOG(ParentClass->GetName() + "->" + ParentPropertyA->GetName() + " : " + *ParentPropertyA->Get<int>(ParentClassPtr));
		if (ParentPropertyB) LOG(ParentClass->GetName() + "->" + ParentPropertyB->GetName() + " : " + *ParentPropertyB->Get<double>(ParentClassPtr));
		if (ParentPropertyC) LOG(ParentClass->GetName() + "->" + ParentPropertyC->GetName() + " : " + *ParentPropertyC->Get<float>(ParentClassPtr));
	}



	/**
	 * Properties tests
	 */
	RProperty* PropertyA = MyClass->GetProperty(MakeUniqueID("A"));
	RProperty* PropertyB = MyClass->GetProperty(MakeUniqueID("B"));
	RProperty* PropertyC = MyClass->GetProperty(MakeUniqueID("C"));
	RProperty* PropertyD = MyClass->GetProperty(MakeUniqueID("D"));
	if (PropertyA) LOG(PropertyA->GetName() + " : " + *PropertyA->Get<int>(MyObject));
	if (PropertyB) LOG(PropertyB->GetName() + " : " + *PropertyB->Get<double>(MyObject));
	if (PropertyC) LOG(PropertyC->GetName() + " : " + *PropertyC->Get<float>(MyObject));
	if (PropertyD) LOG(PropertyD->GetName() + " : " + PropertyD->Get<BasicStructure>(MyObject)->D);

	/**
	 * Function tests
	 */	
	RFunction<void, ChildOneTwo>* FuncA = MyClass->GetFunction<void, ChildOneTwo>("FunctionA");
	RFunction<double, ChildOneTwo, int, int, int>* FuncB = MyClass->GetFunction<double, ChildOneTwo, int, int, int>("FunctionB");
	if (FuncA) LOG(FuncA->GetName()); FuncA->Execute((ChildOneTwo*)MyObject);
	if (FuncB) LOG(FuncB->GetName() + " : " + FuncB->Execute((ChildOneTwo*)MyObject, 10, 20, 30));
}