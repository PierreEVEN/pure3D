
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

REFL_DECLARE_TYPENAME(int);
REFL_DECLARE_TYPENAME(float);
REFL_DECLARE_TYPENAME(double);
REFL_DECLARE_TYPENAME(bool);

int main() {

	REFL_REGISTER_TYPE(int);
	REFL_REGISTER_TYPE(float);
	REFL_REGISTER_TYPE(double);
	REFL_REGISTER_TYPE(bool);

	RSerializerInterface_Object* ObjectSerializer = new RSerializerInterface_Object();
	RSerializerInterface_PrimitiveTypes* PrimitiveTypeSerializer = new RSerializerInterface_PrimitiveTypes();

	ChildOneTwo::GetStaticClass()->SetSerializer(ObjectSerializer);
	ParentOne::GetStaticClass()->SetSerializer(ObjectSerializer);
	ParentTwo::GetStaticClass()->SetSerializer(ObjectSerializer);
	BasicObject::GetStaticClass()->SetSerializer(ObjectSerializer);
	BasicStructure::GetStaticClass()->SetSerializer(ObjectSerializer);
	RType::GetType<int>()->SetSerializer(PrimitiveTypeSerializer);
	RType::GetType<float>()->SetSerializer(PrimitiveTypeSerializer);
	RType::GetType<double>()->SetSerializer(PrimitiveTypeSerializer);
	RType::GetType<bool>()->SetSerializer(PrimitiveTypeSerializer);

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
		RProperty* ParentPropertyA = ParentClass->GetProperty(GenUniqueID("A"));
		RProperty* ParentPropertyB = ParentClass->GetProperty(GenUniqueID("B"));
		RProperty* ParentPropertyC = ParentClass->GetProperty(GenUniqueID("C"));
		if (ParentPropertyA) LOG(ParentClass->GetName() + "->" + ParentPropertyA->GetName() + " : " + *ParentPropertyA->Get<int>(ParentClassPtr));
		if (ParentPropertyB) LOG(ParentClass->GetName() + "->" + ParentPropertyB->GetName() + " : " + *ParentPropertyB->Get<double>(ParentClassPtr));
		if (ParentPropertyC) LOG(ParentClass->GetName() + "->" + ParentPropertyC->GetName() + " : " + *ParentPropertyC->Get<float>(ParentClassPtr));
	}



	/**
	 * Properties tests
	 */
	RProperty* PropertyA = MyClass->GetProperty(GenUniqueID("A"));
	RProperty* PropertyB = MyClass->GetProperty(GenUniqueID("B"));
	RProperty* PropertyC = MyClass->GetProperty(GenUniqueID("C"));
	RProperty* PropertyD = MyClass->GetProperty(GenUniqueID("D"));
	if (PropertyA) LOG(PropertyA->GetName() + " : " + *PropertyA->Get<int>(MyObject));
	if (PropertyB) LOG(PropertyB->GetName() + " : " + *PropertyB->Get<double>(MyObject));
	if (PropertyC) LOG(PropertyC->GetName() + " : " + *PropertyC->Get<float>(MyObject));
	if (PropertyD) LOG(PropertyD->GetName() + " : " + PropertyD->Get<BasicStructure>(MyObject)->A);

	/**
	 * Function tests
	 */	
	RFunction<void, ChildOneTwo>* FuncA = MyClass->GetFunction<void, ChildOneTwo>("FunctionA");
	RFunction<double, ChildOneTwo, int, int, int>* FuncB = MyClass->GetFunction<double, ChildOneTwo, int, int, int>("FunctionB");
	if (FuncA) LOG(FuncA->GetName()); FuncA->Execute((ChildOneTwo*)MyObject);
	if (FuncB) LOG(FuncB->GetName() + " : " + FuncB->Execute((ChildOneTwo*)MyObject, 10, 20, 30));
}