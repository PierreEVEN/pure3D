#include "Reflection/Serialization.h"
#include "Reflection/RClass.h"
#include "IO/Log.h"


void SArchive::Serialize(std::ostream& OutputStream) {
	for (const auto& Object : LinkedObjects) {
		ISerializerInterface* Serializer = Object.second.ObjectType->GetSerializer();
		if (!Serializer) {
			LOG_WARNING("Cannot serialize " + Object.first + " : " + Object.second.ObjectType->GetName() + " is not serializable.");
			continue;
		}

		Serializer->Serialize(Object.second.ObjectType->GetId(), Object.second.ObjectType, Object.second.ObjectPtr, OutputStream);
	}
}

void SArchive::Deserialize(std::istream& InputStream) {
// 	SArchiveField NewField;
// 	InputStream >> NewField.FieldName;
// 	InputStream >> NewField.TypeName;
// 	InputStream >> NewField.DataLength;
// 	NewField.Data = new char(NewField.DataLength);
// 	InputStream.read(NewField.Data, NewField.DataLength);
// 
// 	RType::GetType(TypeName)->GetSerializer();
}

void SArchive::LinkObject(const String& ObjectName, RType* ObjectType, void* ObjectPtr) {
	LinkedObjects[ObjectName] = SArchiveObject { ObjectType, ObjectPtr };
}
