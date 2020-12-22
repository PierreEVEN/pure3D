#pragma once
#include "Serialization.h"


struct RSerializerInterface_PrimitiveTypes : ISerializerInterface {
	virtual void Serialize(const size_t& ParentClassID, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream);
	virtual void Deserialize(std::istream& InputStream, RType* ObjectType, void* ObjectPtr, int64_t TotalSize);
	virtual size_t GetObjectSize(RType* ObjectType, void* ObjectPtr);
};

struct RSerializerInterface_Object : ISerializerInterface {
	virtual void Serialize(const size_t& ParentClassID, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream);
	virtual void Deserialize(std::istream& InputStream, RType* ObjectType, void* ObjectPtr, int64_t TotalSize);
	virtual size_t GetObjectSize(RType* ObjectType, void* ObjectPtr);
};

struct RSerializerInterface_String : ISerializerInterface {
	virtual void Serialize(const size_t& ParentClassID, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream);
	virtual void Deserialize(std::istream& InputStream, RType* ObjectType, void* ObjectPtr, int64_t TotalSize);
	virtual size_t GetObjectSize(RType* ObjectType, void* ObjectPtr);
};
