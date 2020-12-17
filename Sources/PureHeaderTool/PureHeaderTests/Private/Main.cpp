
#include "BasicObject.h"
#include "Reflection/RType.h"
#include "IO/Log.h"
#include "Reflection/RProperty.h"
#include "Reflection/RFunction.h"
#include "Reflection/RConstructor.h"
#include "Reflection/ReflectionMacros.h"
#include "Reflection/Serialization.h"
#include <fstream>

REFL_DECLARE_TYPENAME(int);
REFL_DECLARE_TYPENAME(float);
REFL_DECLARE_TYPENAME(double);
REFL_DECLARE_TYPENAME(bool);

#define DECLARE_PRIMITIVE_TYPE_SERIALIZER(Type) \
struct RSerializerInterface_##Type : RSerializerInterface { \
	virtual size_t GetPropertySize(void* TypePtr) override { return sizeof(Type); } \
}

DECLARE_PRIMITIVE_TYPE_SERIALIZER(int);
DECLARE_PRIMITIVE_TYPE_SERIALIZER(float);
DECLARE_PRIMITIVE_TYPE_SERIALIZER(double);
DECLARE_PRIMITIVE_TYPE_SERIALIZER(bool);

int main() {

	REFL_REGISTER_TYPE(int);
	REFL_REGISTER_TYPE(float);
	REFL_REGISTER_TYPE(double);
	REFL_REGISTER_TYPE(bool);


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
	if (!MyObject) return 0;







	std::ofstream output("test.txt");


	SArchiveField Field;
	Field.FieldName = "TestVar";
	Field.ObjectPtr = MyObject;
	Field.Type = MyClass;

	MyClass::GetSerializer();



	output.close();

	std::ifstream input("test.txt");



	input.close();









	/**
	 * Inheritance test
	 */
	for (const auto& ParentClass : MyClass->GetParents()) {
		void* ParentClassPtr = MyClass->CastTo(ParentClass, MyObject);
		RProperty* ParentPropertyA = ParentClass->GetProperty("A");
		RProperty* ParentPropertyB = ParentClass->GetProperty("B");
		RProperty* ParentPropertyC = ParentClass->GetProperty("C");
		if (ParentPropertyA) LOG(ParentClass->GetName() + "->" + ParentPropertyA->GetName() + " : " + *ParentPropertyA->Get<int>(ParentClassPtr));
		if (ParentPropertyB) LOG(ParentClass->GetName() + "->" + ParentPropertyB->GetName() + " : " + *ParentPropertyB->Get<double>(ParentClassPtr));
		if (ParentPropertyC) LOG(ParentClass->GetName() + "->" + ParentPropertyC->GetName() + " : " + *ParentPropertyC->Get<float>(ParentClassPtr));
	}

	/**
	 * Properties tests
	 */
	RProperty* PropertyA = MyClass->GetProperty("A");
	RProperty* PropertyB = MyClass->GetProperty("B");
	RProperty* PropertyC = MyClass->GetProperty("C");
	RProperty* PropertyD = MyClass->GetProperty("D");
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