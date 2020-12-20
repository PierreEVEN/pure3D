#include "Serialization/GenericSerializers.h"
#include "Reflection/RClass.h"
#include "IO/Log.h"
#include "Reflection/RProperty.h"

void RSerializerInterface_Object::Serialize(const size_t& ParentClassID, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream)
{

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

void RSerializerInterface_Object::Deserialize(std::istream& InputStream, RType* ObjectType, void* ObjectPtr, size_t TotalSize) {

	// Alway used on RCLass
	RClass* MyClass = static_cast<RClass*>(ObjectType);

	while (TotalSize > 0) {
		size_t PropertyID;
		size_t ParentID;
		size_t PropertySize;

		InputStream.read((char*)&PropertyID, sizeof(size_t));
		InputStream.read((char*)&ParentID, sizeof(size_t));
		InputStream.read((char*)&PropertySize, sizeof(size_t));

		RClass* ParentClass = RClass::GetClass(ParentID);
		RProperty* Property = ParentClass->GetProperty(PropertyID);

		size_t CurrentPosition = InputStream.tellg();

		if (!Property->GetType() || !Property->GetType()->GetSerializer()) {
			LOG_WARNING("failed to read prop");
		}
		else {
			void* ClassPtr = MyClass->CastTo(ParentClass, ObjectPtr);
			Property->GetType()->GetSerializer()->Deserialize(InputStream, Property->GetType(), Property->Get(ClassPtr), PropertySize);
		}

		InputStream.seekg(CurrentPosition + PropertySize, std::ios::beg); // Fake READ

		TotalSize -= sizeof(size_t) * 3;
		TotalSize -= PropertySize;
	}
}

size_t RSerializerInterface_Object::GetObjectSize(RType* ObjectType, void* ObjectPtr) {
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

void RSerializerInterface_PrimitiveTypes::Serialize(const size_t& ParentClassID, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream) {
	OutputStream.write((char*)ObjectPtr, ObjectType->GetSize());
}

void RSerializerInterface_PrimitiveTypes::Deserialize(std::istream& InputStream, RType* ObjectType, void* ObjectPtr, size_t TotalSize) {
	InputStream.read((char*)ObjectPtr, ObjectType->GetSize());
}

size_t RSerializerInterface_PrimitiveTypes::GetObjectSize(RType* ObjectType, void* ObjectPtr) {
	return ObjectType->GetSize();
}
