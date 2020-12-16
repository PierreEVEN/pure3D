#include "Reflection/Serialization.h"
#include "Reflection/RType.h"
#include "IO/Log.h"

SArchiveField::SArchiveField(const String& inTypeName, const size_t inDataLength, void* inData)
	: TypeName(inTypeName), DataLength(inDataLength), Data(inData) {
	if (const RType* ReflType = RType::GetType(TypeName)) {
		if (!ReflType->GetSerializer())
			LOG_ASSERT("Cannot serialize " + inTypeName + " because it doesn't have any serializer.");
	}
	LOG_ASSERT("Cannot serialize " + inTypeName + " because it's not a reflected type.");
}

void SArchive::Serialize() {
	for (const auto& Field : Fields) {
		RType::GetType(Field.TypeName)->GetSerializer()->Serialize(Field.Data);
	}
}

void SArchive::Deserialize() {
	String TypeName;
	size_t DataLength;
	void* Data;

	RType::GetType(TypeName)->GetSerializer();
}
