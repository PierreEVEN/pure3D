#include "Reflection/Serialization.h"
#include "Reflection/RClass.h"
#include "IO/Log.h"


SArchive& SArchive::operator<<(const SArchiveField& Field) {
	Fields.push_back(Field); 
	return *this;
}

void SArchive::Serialize(std::ostream& OutputStream) {
	for (const auto& Field : Fields) {
		RSerializerInterface* Serializer = Field.Type->GetSerializer();
		size_t PropertyLength = Serializer->GetPropertySize(Field.ObjectPtr);
		OutputStream << Field.FieldName;
		OutputStream << Field.Type->GetName();
		OutputStream << PropertyLength;
		OutputStream.write(Serializer->GetTypeData(Field.ObjectPtr), PropertyLength);
		Serializer->PostSerializeData();
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
