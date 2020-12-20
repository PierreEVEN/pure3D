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
		OutputStream.write((char*)&Object.first, sizeof(size_t));
		OutputStream.write((char*)&ObjectSize, sizeof(size_t));

		Serializer->Serialize(Object.second.ObjectType->GetId(), Object.second.ObjectType, Object.second.ObjectPtr, OutputStream);
	}
}

void SArchive::Deserialize(std::istream& InputStream) {
	size_t ObjectId;
	size_t ObjectSize;
	InputStream.read((char*)&ObjectId, sizeof(size_t));
	InputStream.read((char*)&ObjectSize, sizeof(size_t));
	auto FoundObject = LinkedObjects.find(ObjectId);
	if (FoundObject == LinkedObjects.end()) return;
	RType* ObjectType = FoundObject->second.ObjectType;

	ISerializerInterface* Serializer = ObjectType->GetSerializer();
	if (!Serializer) {
		LOG_WARNING("Cannot serialize " + String(FoundObject->first) + " : " + ObjectType->GetName() + " is not serializable.");
		return;
	}

	Serializer->Deserialize(InputStream, ObjectType, FoundObject->second.ObjectPtr, ObjectSize);
}

void SArchive::LinkObject(const String& ObjectName, RType* ObjectType, void* ObjectPtr) {
	LinkedObjects[std::hash<String>{}(ObjectName)] = SArchiveObject{ ObjectType, ObjectPtr };
}
