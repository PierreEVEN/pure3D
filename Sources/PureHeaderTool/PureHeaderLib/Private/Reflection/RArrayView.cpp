#include "Reflection/RArrayView.h"
#include "IO/Log.h"
#include "Serialization/Serialization.h"

void RSerializerInterface_Array::Serialize(const RUID& ParentClassID, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream) {
	RArrayType* ArrayType = static_cast<RArrayType*>(ObjectType);
	ISerializerInterface* InnerSerializer = ArrayType->GetInnerType() ? ArrayType->GetInnerType()->GetSerializer() : nullptr;
	if (!InnerSerializer) {
		LOG_WARNING("Cannot serialize array : inner type of " + ArrayType->GetName() + " is not serializable.");
		return;
	}

	RArrayView View = ArrayType->GetView(ObjectPtr);
	uint32_t ElementCount = static_cast<uint32_t>(View.GetLength());

	OutputStream.write((char*)&ElementCount, sizeof(uint32_t));
	for (uint32_t i = 0; i < ElementCount; ++i) {
		void* ElementValue = View.GetValuePtr(i);
		uint32_t ElementSize = static_cast<uint32_t>(InnerSerializer->GetObjectSize(ArrayType->GetInnerType(), ElementValue));
		OutputStream.write((char*)&ElementSize, sizeof(uint32_t));
		InnerSerializer->Serialize(ParentClassID, ArrayType->GetInnerType(), ElementValue, OutputStream);
	}
}

void RSerializerInterface_Array::Deserialize(std::istream& InputStream, RType* ObjectType, void* ObjectPtr, int64_t TotalSize)
{
	RArrayType* ArrayType = static_cast<RArrayType*>(ObjectType);
	ISerializerInterface* InnerSerializer = ArrayType->GetInnerType() ? ArrayType->GetInnerType()->GetSerializer() : nullptr;
	if (!InnerSerializer) {
		LOG_WARNING("Cannot deserialize array : inner type of " + ArrayType->GetName() + " is not serializable.");
		return;
	}
	RType* InnerType = ArrayType->GetInnerType();

	RArrayView View = ArrayType->GetView(ObjectPtr);
	uint32_t ElementCount;
	InputStream.read((char*)&ElementCount, sizeof(uint32_t));

	View.Clear();
	for (uint32_t i = 0; i < ElementCount; ++i) {
		void* Ouput = InnerType->NewType();
		uint32_t ElementSize;
		InputStream.read((char*)&ElementSize, sizeof(uint32_t));
		InnerSerializer->Deserialize(InputStream, InnerType, Ouput, ElementSize);
		View.Insert(i, Ouput);
		InnerType->DeleteType(Ouput);
	}
}

size_t RSerializerInterface_Array::GetObjectSize(RType* ObjectType, void* ObjectPtr)
{
	RArrayType* ArrayType = static_cast<RArrayType*>(ObjectType);
	ISerializerInterface* InnerSerializer = ArrayType->GetInnerType() ? ArrayType->GetInnerType()->GetSerializer() : nullptr;
	if (!InnerSerializer) return 0;
	RArrayView View = ArrayType->GetView(ObjectPtr);
	size_t Size = sizeof(uint32_t);

	for (int i = 0; i < View.GetLength(); ++i) {
		Size += sizeof(uint32_t);
		Size += InnerSerializer->GetObjectSize(ArrayType->GetInnerType(), View.GetValuePtr(i));
	}
	return Size;
}

void* RArrayType::NewType() {
	void* Memory = new char[GetSize()];
	RArrayView View = GetView(Memory);
	View.Construct();
	return Memory;
}

void RArrayType::DeleteType(void* Memory) {
	RArrayView View = GetView(Memory);
	View.Destruct();
}
