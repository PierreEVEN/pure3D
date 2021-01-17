#pragma once
#include "Types/String.h"
#include <unordered_map>
#include "Reflection/ReflectionUtilities.h"

struct RType;

struct SArchiveObject {
	RType* ObjectType;
	void* ObjectPtr;
};

struct SArchive {


	void Serialize(std::ostream& OutputStream);
	void Deserialize(std::istream& InputStream);
	void DeserializeAndCreateClass(std::istream& InputStream);

	void LinkObject(const String& ObjectName, RType* ObjectType, void* ObjectPtr);

	[[nodiscard]] std::unordered_map<RUID, SArchiveObject> GetObjects() const { return LinkedObjects; }
private:

	std::unordered_map<RUID, SArchiveObject> LinkedObjects;
};


struct ISerializerInterface {
	template<typename Interface>
	static ISerializerInterface* Get(const String& InterfaceName) {
		if (!Interfaces) Interfaces = new std::unordered_map<String, ISerializerInterface*>();
		auto FoundInterface = Interfaces->find(InterfaceName);
		if (FoundInterface == Interfaces->end()) {
			ISerializerInterface* NewInterface = new Interface();
			(*Interfaces)[InterfaceName] = NewInterface;
			return NewInterface;
		}
		return FoundInterface->second;
	}

	virtual void Serialize(const RUID& ParentClassID, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream) = 0;
	virtual void Deserialize(std::istream& InputStream, RType* ObjectType, void* ObjectPtr, int64_t TotalSize) = 0;
	virtual size_t GetObjectSize(RType* ObjectType, void* ObjectPtr) = 0;

private:
	inline static std::unordered_map<String, ISerializerInterface*>* Interfaces;
};