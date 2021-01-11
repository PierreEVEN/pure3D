#pragma once

#include <any>
#include <vector>
#include "RType.h"
#include "RProperty.h"
#include "Serialization/Serialization.h"

enum class EArrayViewOperation {
	EAvo_GetValue,
	EAvo_Length,
	EAvo_Insert,
	EAvo_Delete,
	EAvo_Clear,
	EAvo_Construct,
	EAvo_Destruct,
};

using FArrayViewVisitFunc = std::any(*)(const EArrayViewOperation&, void*, std::any);

struct RArrayView {

	RArrayView(void* InContainer, FArrayViewVisitFunc InFunc) : Func(InFunc), Container(InContainer) {}

	template<typename T = void>
	inline T* GetValuePtr(size_t Index) { return (T*)std::any_cast<T*>(Func(EArrayViewOperation::EAvo_GetValue, Container, Index)); };
	inline size_t GetLength() { return std::any_cast<size_t>(Func(EArrayViewOperation::EAvo_Length, Container, nullptr)); }
	inline void Insert(size_t Index, void* Value) { Func(EArrayViewOperation::EAvo_Insert, Container, std::pair<size_t, void*>(Index, Value)); }
	inline void Delete(size_t Index) { Func(EArrayViewOperation::EAvo_Delete, Container, Index); }
	inline void Clear() { Func(EArrayViewOperation::EAvo_Clear, Container, nullptr); }
	inline void Construct() { Func(EArrayViewOperation::EAvo_Construct, Container, nullptr); }
	inline void Destruct() { Func(EArrayViewOperation::EAvo_Destruct, Container, nullptr); }

private:

	void* Container;
	FArrayViewVisitFunc Func;
};

template<typename T, typename Policy>
struct TArrayViewPolicyBase {
	inline static std::any Get(const EArrayViewOperation& op, void* Container, std::any additional_data) {
		switch (op) {
		case EArrayViewOperation::EAvo_GetValue:
			return Policy::GetValuePtr(Container, additional_data);
			break;
		case EArrayViewOperation::EAvo_Length:
			return Policy::Length(Container, additional_data);
			break;
		case EArrayViewOperation::EAvo_Insert:
			Policy::Insert(Container, additional_data);
			break;
		case EArrayViewOperation::EAvo_Delete:
			Policy::Delete(Container, additional_data);
			break;
		case EArrayViewOperation::EAvo_Clear:
			Policy::Clear(Container, additional_data);
			break;
		case EArrayViewOperation::EAvo_Construct:
			Policy::Construct(Container, additional_data);
			break;
		case EArrayViewOperation::EAvo_Destruct:
			Policy::Destruct(Container, additional_data);
			break;
		}
		return nullptr;
	}
};

template<typename T>
struct TArrayViewPolicyStdVector : public TArrayViewPolicyBase<T, TArrayViewPolicyStdVector<T>> {
	inline static std::any GetValuePtr(void* Container, std::any AdditionalData) {
		size_t Index = std::any_cast<size_t>(AdditionalData);
		return static_cast<void*>(&(*(reinterpret_cast<std::vector<T>*>(Container)->begin() + Index)));
	}

	inline static std::any Length(void* Container, std::any AdditionalData) {
		return static_cast<std::vector<T>*>(Container)->size();
	}

	inline static std::any Insert(void* Container, std::any AdditionalData) {
		std::vector<T>* vec = reinterpret_cast<std::vector<T>*>(Container);
		std::pair<size_t, void*> InsertData = std::any_cast<std::pair<size_t, void*>>(AdditionalData);
		return vec->insert(vec->begin() + std::any_cast<size_t>(InsertData.first), *static_cast<T*>(InsertData.second));
	}

	inline static std::any Delete(void* Container, std::any AdditionalData) {
		std::vector<T>* vec = reinterpret_cast<std::vector<T>*>(Container);
		return vec->erase(vec->begin() + std::any_cast<size_t>(AdditionalData));
	}

	inline static std::any Clear(void* Container, std::any AdditionalData) {
		reinterpret_cast<std::vector<T>*>(Container)->clear();
		return nullptr;
	}

	inline static std::any Construct(void* Container, std::any AdditionalData) {
		return new (Container) std::vector<T>();
	}

	inline static std::any Destruct(void* Container, std::any AdditionalData) {
		delete(reinterpret_cast<std::vector<T>*>(Container));
		return nullptr;
	}
};


struct RArrayType : public RType {


	RArrayType(const String& inTypeName, const size_t inTypeSize)
		: RType(inTypeName, inTypeSize) {}

	inline RType* GetInnerType() const { return InnerType; }

	template<typename Type, typename InnerType>
	inline static RArrayType* RegisterType(const String& inTypeName, const String& inInnerType) {
		static_assert(RIsReflected<Type>::Value, "Failed to register array type : not a reflected type. Please declare this type as a reflected type.");
		RArrayType* RegisteredType = RType::RegisterType<Type, RArrayType>(inTypeName);
		RegisteredType->SetInnerType(inInnerType);
		RegisteredType->ArrayViewFunc = &TArrayViewPolicyStdVector<InnerType>::Get;
		return RegisteredType;
	}

	/**
	 * Get type variant
	 */
	inline virtual const ERType GetTypeVariant() const { return ERType::ERType_Array; }

	RArrayView GetView(void* Object) { return RArrayView(Object, ArrayViewFunc); }

	virtual void* NewType();
	virtual void DeleteType(void* Memory);

private:

	void SetInnerType(const String& inInnerType) {
		String TypeName = RType::NormalizeTypeName(inInnerType);
		if (!(InnerType = RType::GetType(TypeName)))
			RArrayType::WaitTypeRegistration(TypeName, this, &RArrayType::OnRegisterType);
	}

	inline void OnRegisterType(RType* RegisteredType) { InnerType = RegisteredType; }

	RType* InnerType;

	FArrayViewVisitFunc ArrayViewFunc;
};

struct RArrayproperty : public RProperty {

	RArrayproperty(const String& inPropertyType, const FArrayViewVisitFunc& InView, const String& inPropertyName, const size_t inPropertyOffset, bool InIsTransient)
		: RProperty(inPropertyType, inPropertyName, inPropertyOffset, InIsTransient) {
		ArrayViewFunc = InView;
	}

private:

	FArrayViewVisitFunc ArrayViewFunc;
};

struct RSerializerInterface_Array : public ISerializerInterface {

	virtual void Serialize(const RUID& ParentClassID, RType* ObjectType, void* ObjectPtr, std::ostream& OutputStream);
	virtual void Deserialize(std::istream& InputStream, RType* ObjectType, void* ObjectPtr, int64_t TotalSize);
	virtual size_t GetObjectSize(RType* ObjectType, void* ObjectPtr);

};