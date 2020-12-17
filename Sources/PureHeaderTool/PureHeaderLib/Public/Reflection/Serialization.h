#pragma once
#include "Types/String.h"

struct RClass;

struct SArchiveField {
	String FieldName;
	RClass* Type;
	char* ObjectPtr;
};


struct SArchive {

	inline SArchive& operator<<(const SArchiveField& Field);

	void Serialize(std::ostream& OutputStream);
	void Deserialize(std::istream& InputStream);

private:

	std::vector<SArchiveField> Fields;
};


struct RSerializerInterface {
	virtual size_t GetPropertySize(void* TypePtr) = 0;
	virtual char* GetTypeData(void* TypePtr) { return (char*)TypePtr; }
	virtual bool DeserializeType(size_t PropertySize, char* Data, void* TypePtr) {
		memcpy(TypePtr, Data, PropertySize);
		return true;
	}
	virtual void PostSerializeData();
};