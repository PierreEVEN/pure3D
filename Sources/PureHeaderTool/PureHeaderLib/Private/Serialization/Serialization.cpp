#include "Serialization/Serialization.h"
#include "Reflection/RClass.h"
#include "IO/Log.h"


void SArchive::Serialize(std::ostream& OutputStream) {
	for (const auto& Object : LinkedObjects) {
		ISerializerInterface* Serializer = Object.second.ObjectType->GetSerializer();
		if (!Serializer) {
			LOG_WARNING("Cannot serialize " + String(Object.first) + " : " + Object.second.ObjectType->GetName() + " is not serializable.");
			continue;
		}

		size_t ObjectSize = Serializer->GetObjectSize(Object.second.ObjectType, Object.second.ObjectPtr);
		size_t ObjectTypeID = Object.second.ObjectType->GetId();
		OutputStream.write((char*)&Object.first, sizeof(RUID));
		OutputStream.write((char*)&ObjectTypeID, sizeof(RUID));
		OutputStream.write((char*)&ObjectSize, sizeof(size_t));

		Serializer->Serialize(Object.second.ObjectType->GetId(), Object.second.ObjectType, Object.second.ObjectPtr, OutputStream);
	}
}

void SArchive::Deserialize(std::istream& InputStream) {
	RUID ObjectId;
	RUID ObjectTypeID;
	size_t ObjectSize;
	InputStream.read((char*)&ObjectId, sizeof(RUID));
	InputStream.read((char*)&ObjectTypeID, sizeof(size_t));
	InputStream.read((char*)&ObjectSize, sizeof(size_t));
	auto FoundObject = LinkedObjects.find(ObjectId);
	if (FoundObject == LinkedObjects.end()) return;
	RType* ObjectType = FoundObject->second.ObjectType;

	ISerializerInterface* Serializer = ObjectType->GetSerializer();
	if (!Serializer) {
		LOG_WARNING("Cannot deserialize " + String(FoundObject->first) + " : " + ObjectType->GetName() + " is not serializable.");
		return;
	}

	Serializer->Deserialize(InputStream, ObjectType, FoundObject->second.ObjectPtr, ObjectSize);
}

void SArchive::DeserializeAndCreateClass(std::istream& InputStream)
{
	RUID ObjectId;
	RUID ObjectTypeID;
	size_t ObjectSize;
	InputStream.read((char*)&ObjectId, sizeof(RUID));
	InputStream.read((char*)&ObjectTypeID, sizeof(RUID));
	InputStream.read((char*)&ObjectSize, sizeof(size_t));

	RClass* ObjectType = RClass::GetClass(ObjectTypeID);
	if (!ObjectType) {
		LOG_WARNING("Cannot find class with ID " + String(ObjectTypeID));
		return;
	}

	void* ObjectPtr = NewObject(ObjectType);
	if (!ObjectPtr) {
		LOG_WARNING("Cannot create object for class " + ObjectType->GetName() + " : No default constructor found.");
		return;
	}

	LinkedObjects[MakeUniqueID("DeserializedObject")] = SArchiveObject{ ObjectType, ObjectPtr };

	ISerializerInterface* Serializer = ObjectType->GetSerializer();
	if (!Serializer) {
		LOG_WARNING("Cannot deserialize class : " + ObjectType->GetName() + " is not serializable.");
		return;
	}

	Serializer->Deserialize(InputStream, ObjectType, ObjectPtr, ObjectSize);
}

void SArchive::LinkObject(const String& ObjectName, RType* ObjectType, void* ObjectPtr) {
	LinkedObjects[MakeUniqueID(ObjectName)] = SArchiveObject{ ObjectType, ObjectPtr };
}
