#include "Serialization/GenericSerializers.h"
#include "Reflection/RClass.h"
#include "IO/Log.h"
#include "Reflection/RProperty.h"
#include "Types/ByteArray.h"

void RSerializerInterface_Object::Serialize(const RUID& ParentClassID, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream)
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
		if (Property->IsTransient()) continue;
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

		RUID PropertyID = ParentClass.first;
		size_t ObjectSize = Serializer->GetObjectSize(PropertyType, Property->Get(ObjectPtr));

		OutputStream.write((char*)&PropertyID, sizeof(RUID));												// Property ID
		OutputStream.write((char*)&ParentClassID, sizeof(RUID));											// Property's parent object ID
		OutputStream.write((char*)&ObjectSize, sizeof(size_t));												// Property byte length
		Serializer->Serialize(PropertyType->GetId(), PropertyType, Property->Get(ObjectPtr), OutputStream); // Property Data
	}
}

void RSerializerInterface_Object::Deserialize(std::istream& InputStream, RType* ObjectType, void* ObjectPtr, int64_t TotalSize) {

	// Alway used on RCLass
	RClass* MyClass = static_cast<RClass*>(ObjectType);

	while (TotalSize > 0) {
		RUID PropertyID;
		RUID ParentID;
		size_t PropertySize;

		InputStream.read((char*)&PropertyID, sizeof(RUID));
		InputStream.read((char*)&ParentID, sizeof(RUID));
		InputStream.read((char*)&PropertySize, sizeof(size_t));

		RClass* ParentClass = RClass::GetClass(ParentID);
		RProperty* Property = ParentClass->GetProperty(PropertyID);

		size_t CurrentPosition = InputStream.tellg();
		
		if (!(Property && Property->IsTransient())) {
			if (!Property || !Property->GetType() || !Property->GetType()->GetSerializer()) {
				LOG_WARNING("Failed to read property %u (size = %u)", PropertyID, PropertySize);
			}
			else {
				void* ClassPtr = MyClass->CastTo(ParentClass, ObjectPtr);
				Property->GetType()->GetSerializer()->Deserialize(InputStream, Property->GetType(), Property->Get(ClassPtr), PropertySize);
			}
		}

		InputStream.seekg(CurrentPosition + PropertySize, std::ios::beg); // Fake READ

		TotalSize -= sizeof(RUID) * 2;
		TotalSize -= sizeof(size_t);
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
		ObjectSize += Serializer->GetObjectSize(PropertyType, Property->Get(ObjectPtr));
	}

	return ObjectSize;
}

void RSerializerInterface_PrimitiveTypes::Serialize(const RUID& ParentClassID, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream) {
	OutputStream.write((char*)ObjectPtr, ObjectType->GetSize());
}

void RSerializerInterface_PrimitiveTypes::Deserialize(std::istream& InputStream, RType* ObjectType, void* ObjectPtr, int64_t TotalSize) {
	InputStream.read((char*)ObjectPtr, ObjectType->GetSize());
}

size_t RSerializerInterface_PrimitiveTypes::GetObjectSize(RType* ObjectType, void* ObjectPtr) {
	return ObjectType->GetSize();
}

void RSerializerInterface_String::Serialize(const size_t& ParentClassID, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream) {
	String* Str = (String*)ObjectPtr;
	OutputStream.write(Str->GetData(), Str->Length());
}

void RSerializerInterface_String::Deserialize(std::istream& InputStream, RType* ObjectType, void* ObjectPtr, int64_t TotalSize) {
	std::shared_ptr<char> Text(new char[TotalSize + 1], std::default_delete<char[]>());
	Text.get()[TotalSize] = '\0';
	InputStream.read(Text.get(), TotalSize);
	*(String*)ObjectPtr = Text.get();
}

size_t RSerializerInterface_String::GetObjectSize(RType* ObjectType, void* ObjectPtr) {
	return static_cast<String*>(ObjectPtr)->Length();
}

void RSerializerInterface_ByteArray::Serialize(const size_t& ParentClassID, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream) {
	ByteArray* Object = ((ByteArray*)ObjectPtr);
	if (Object->GetLength() > 0) OutputStream.write((char*)Object->GetData(), Object->GetLength());
}

void RSerializerInterface_ByteArray::Deserialize(std::istream& InputStream, RType* ObjectType, void* ObjectPtr, int64_t TotalSize) {
	if (TotalSize > 0) {
		uint8_t* Data = new uint8_t[TotalSize];
		InputStream.read((char*)Data, TotalSize);
		((ByteArray*)ObjectPtr)->SetData(Data, TotalSize);
	}
	else ((ByteArray*)ObjectPtr)->SetData(nullptr, 0);
}

size_t RSerializerInterface_ByteArray::GetObjectSize(RType* ObjectType, void* ObjectPtr) {
	return ((ByteArray*)ObjectPtr)->GetLength();
}
