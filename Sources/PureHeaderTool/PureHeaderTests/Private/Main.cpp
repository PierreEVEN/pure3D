
#include "BasicObject.h"
#include "Reflection/RType.h"
#include "IO/Log.h"
#include "Reflection/RProperty.h"
#include "Reflection/RFunction.h"
#include "Reflection/RConstructor.h"
#include "Reflection/ReflectionMacros.h"
#include "Reflection/Serialization.h"
#include <fstream>
#include "Types/Array.h"

REFL_DECLARE_TYPENAME(int);
REFL_DECLARE_TYPENAME(float);
REFL_DECLARE_TYPENAME(double);
REFL_DECLARE_TYPENAME(bool);

#define DECLARE_PRIMITIVE_TYPE_SERIALIZER(Type) \
struct RSerializerInterface_##Type : RSerializerInterface { \
	virtual size_t GetPropertySize(void* TypePtr) override { return sizeof(Type); } \
}

struct RSerializerInterface_PrimitiveTypes : ISerializerInterface {

	virtual void Serialize(const size_t& ParentClassID, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream) {
		OutputStream.write((char*)ObjectPtr, ObjectType->GetSize());
	}

	virtual void Deserialize(std::istream& InputStream) {
	}

	virtual size_t GetObjectSize(RType* ObjectType, void* ObjectPtr) {
		return ObjectType->GetSize();
	}
};


struct RSerializerInterface_Object : ISerializerInterface {

	virtual void Serialize(const size_t& ParentClassID, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream) {

		// Alway used on RCLass
		RClass* MyClass = static_cast<RClass*>(ObjectType);

		// Get parent properties
		for (const auto& ParentClass : MyClass->GetParents()) {

			ISerializerInterface* Serializer = ParentClass->GetSerializer();
			if (!Serializer) {
				LOG_WARNING("(" + ObjectType->GetName() + ") : Cannot serialize parent class " + ParentClass->GetName() + " because it doesn't have any serializer.");
				continue;
			}

			Serializer->Serialize(ParentClass->GetId(), ParentClass, ObjectType->CastTo(ParentClass, ObjectPtr), OutputStream);
		}

		// Get every serializable properties
		for (const auto& ParentClass : MyClass->GetProperties()) {
			// Current property
			RProperty* Property = ParentClass.second;
			// Current property type
			RType* PropertyType = Property->GetType();
			// Current property's serializer
			ISerializerInterface* Serializer = PropertyType ? PropertyType->GetSerializer() : nullptr;

			// Ensure the property can be serialized
			if (!PropertyType) {
				LOG_WARNING(Property->GetName() + "(" + ObjectType->GetName() + ") : " + Property->GetName() + " is not serializable because it doesn't have any RType.");
				continue;
			}
			if (!Serializer) {
				LOG_WARNING(Property->GetName() + "(" + ObjectType->GetName() + ") : Cannot serialize " + Property->GetName() + " because " + PropertyType->GetName() + " is not serializable.");
				continue;
			}

			size_t PropertyID = ParentClass.first;
			size_t ObjectSize = Serializer->GetObjectSize(PropertyType, ObjectPtr);

			OutputStream.write((char*)&PropertyID, sizeof(size_t));																// Property ID
			OutputStream.write((char*)&ParentClassID, sizeof(size_t));															// Property's parent object ID
			OutputStream.write((char*)&ObjectSize, sizeof(size_t));																// Property byte length
			Serializer->Serialize(PropertyType->GetId(), PropertyType, Property->Get(ObjectPtr), OutputStream); // Property Data
		}
	}

	virtual void Deserialize(std::istream& InputStream) {

	}

	virtual size_t GetObjectSize(RType* ObjectType, void* ObjectPtr) {
		size_t ObjectSize = 0;

		// Alway used on RCLass
		RClass* MyClass = static_cast<RClass*>(ObjectType);

		// Get parent size
		for (const auto& ParentClass : MyClass->GetParents()) {
			ISerializerInterface* Serializer = ParentClass->GetSerializer();
			if (!Serializer) continue;
			ObjectSize += Serializer->GetObjectSize(ParentClass, ObjectType->CastTo(ParentClass, ObjectPtr));
		}

		// Get every serializable property size
		for (const auto& ParentClass : MyClass->GetProperties()) {
			// Current property
			RProperty* Property = ParentClass.second;
			// Current property type
			RType* PropertyType = Property->GetType();
			// Current property's serializer
			ISerializerInterface* Serializer = PropertyType ? PropertyType->GetSerializer() : nullptr;

			// Ensure the property can be serialized
			if (!PropertyType || !Serializer) continue;

			ObjectSize += sizeof(size_t) * 3;
			ObjectSize += Serializer->GetObjectSize(PropertyType, ObjectPtr);
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
	ParentOne::GetStaticClass()->SetSerializer(ObjectSerializer);
	ParentTwo::GetStaticClass()->SetSerializer(ObjectSerializer);
	BasicObject::GetStaticClass()->SetSerializer(ObjectSerializer);
	BasicStructure::GetStaticClass()->SetSerializer(ObjectSerializer);
	RType::GetType<int>()->SetSerializer(PrimitiveTypeSerializer);
	RType::GetType<float>()->SetSerializer(PrimitiveTypeSerializer);
	RType::GetType<double>()->SetSerializer(PrimitiveTypeSerializer);
	RType::GetType<bool>()->SetSerializer(PrimitiveTypeSerializer);


	std::vector<int> Arr2 = { 10, 20, 30 };


	TVector<int> Arr = { 10, 20, 30 };
	Arr.Add(3);
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

	std::ofstream output("test.txt", std::ios::binary);
	Archive.Serialize(output);
	output.close();


	std::ifstream input("test.txt", std::ios::binary);
	Archive.Deserialize(input);
	input.close();

	/**
	 * Inheritance test
	 */
	for (const auto& ParentClass : MyClass->GetParents()) {
		LOG("Get properties for : " + ParentClass->GetName());
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