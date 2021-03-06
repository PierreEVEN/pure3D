#pragma once
#include "Types/String.h"
#include <unordered_map>

struct RType;

struct SArchiveObject {
	RType* ObjectType;
	void* ObjectPtr;
};

struct SArchive {


	void Serialize(std::ostream& OutputStream);
	void Deserialize(std::istream& InputStream);

	void LinkObject(const String& ObjectName, RType* ObjectType, void* ObjectPtr);

private:

	std::unordered_map<size_t, SArchiveObject> LinkedObjects;
};


struct ISerializerInterface {

	virtual void Serialize(const size_t& ParentClassID, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream) = 0;
	virtual void Deserialize(std::istream& InputStream, RType* ObjectType, void* ObjectPtr, size_t TotalSize) = 0;
	virtual size_t GetObjectSize(RType* ObjectType, void* ObjectPtr) = 0;
};