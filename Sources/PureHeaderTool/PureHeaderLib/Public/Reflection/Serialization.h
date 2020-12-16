#pragma once
#include "Types/String.h"

struct SArchiveField {

	SArchiveField(const String& inTypeName, const size_t inDataLength, void* inData);

	const String TypeName;
	const size_t DataLength;
	void* Data;
};


struct SArchive {

	inline SArchive& operator<<(const SArchiveField& Field) {
		Fields.push_back(Field); return *this;
	}

	void Serialize();
	void Deserialize();

private:

	std::vector<SArchiveField> Fields;
};

