
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

struct RSerializerInterface_PrimitiveTypes : ISerializerInterface {

	virtual void Serialize(const String& PropertyName, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream) {
		OutputStream << PropertyName;
		OutputStream << ObjectType->GetSize();
		OutputStream.write((char*)ObjectPtr, ObjectType->GetSize());
	}
	virtual void Deserialize(std::istream& InputStream) {

	}
	virtual size_t GetObjectSize(RType* ObjectType, void* ObjectPtr) {
		return ObjectType->GetSize();
	}
};


struct RSerializerInterface_Object : ISerializerInterface {

	virtual void Serialize(const String& PropertyName, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream) {

		OutputStream << PropertyName;
		OutputStream << GetObjectSize(ObjectType, ObjectPtr);

		RClass* MyClass = static_cast<RClass*>(ObjectType);

		for (auto& Parent : MyClass->GetParents()) {
			ISerializerInterface* ParentSerializer = Parent->GetSerializer();
			if (!ParentSerializer) continue;


			void* NewObjectPtr = MyClass->CastTo(Parent, ObjectPtr);

			//ParentSerializer->Serialize(PropertyName, Parent, NewObjectPtr);

		}



		for (const auto& PropertyField : MyClass->GetProperties()) {
			RProperty* Property = PropertyField.second;
			RType* PropertyType = Property->GetType();
			if (!PropertyType) {
				LOG_WARNING(PropertyName + "(" + ObjectType->GetName() + ") : " + Property->GetName() + " is not serializable because it doesn't have any RType.");
				continue;
			}
			ISerializerInterface* Serializer = PropertyType->GetSerializer();
			if (!Serializer) {
				LOG_WARNING(PropertyName + "(" + ObjectType->GetName() + ") : Cannot serialize " + Property->GetName() + " because " + PropertyType->GetName() + " is not serializable.");
				continue;
			}
			void* NewObjectPtr = ObjectPtr;
			if (PropertyType->GetType() == ERType::ERType_RClass) MyClass->CastTo((RClass*)PropertyType, ObjectPtr);

			OutputStream << Serializer->GetObjectSize(ObjectType, ObjectPtr);
			Serializer->Serialize(Property->GetName(), PropertyType, Property->Get<void>(NewObjectPtr), OutputStream);
		}
	}

	virtual void Deserialize(std::istream& InputStream) {

	}

	virtual size_t GetObjectSize(RType* ObjectType, void* ObjectPtr) {
		size_t ObjectSize = 0;
		RClass* MyClass = static_cast<RClass*>(ObjectType);
		for (const auto& PropertyField : MyClass->GetProperties()) {
			RProperty* Property = PropertyField.second;
			RType* PropertyType = Property->GetType();
			if (!PropertyType) continue;
			ISerializerInterface* Serializer = PropertyType->GetSerializer();
			if (!Serializer) continue;

			void* NewObjectPtr = ObjectPtr;
			if (PropertyType->GetType() == ERType::ERType_RClass) MyClass->CastTo((RClass*)PropertyType, ObjectPtr);

			ObjectSize += Property->GetName().Length() + 1;
			ObjectSize += sizeof(size_t);
			ObjectSize += Serializer->GetObjectSize(PropertyType, NewObjectPtr);

		}
		return ObjectSize;
	}
};

int main() {

	REFL_REGISTER_TYPE(int);
	REFL_REGISTER_TYPE(float);
	REFL_REGISTER_TYPE(double);
	REFL_REGISTER_TYPE(bool);

	RSerializerInterface_Object* ObjectSerializer = new RSerializerInterface_Object();
	RSerializerInterface_PrimitiveTypes* PrimitiveTypeSerializer = new RSerializerInterface_PrimitiveTypes();
	ChildOneTwo::GetStaticClass()->SetSerializer(ObjectSerializer);
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
	if (!MyObject) return 0;


	SArchive Archive;
	Archive.LinkObject("MyObject", MyClass, MyObject);

	std::ofstream output("test.txt");
	Archive.Serialize(output);
	output.close();


	std::ifstream input("test.txt");
	Archive.Deserialize(input);
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