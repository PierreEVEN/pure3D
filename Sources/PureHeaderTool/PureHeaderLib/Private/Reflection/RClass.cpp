#include "Reflection/RClass.h"
#include <utility>
#include <unordered_map>
#include "Reflection/RProperty.h"
#include "Reflection/RFunction.h"
#include "IO/Log.h"
#include <iostream>


void RClass::AddParent(const String& inParent) {
	if (RClass* FoundClass = RClass::GetClass(inParent))
		Parents.push_back(FoundClass);
	else {
		RType::WaitTypeRegistration(inParent, this, &RClass::OnRegisterParentClass);
	}
}

void RClass::OnRegisterParentClass(RType* RegisteredClass) {
	Parents.push_back((RClass*)RegisteredClass);
}

void RClass::AddProperty(RProperty* inProperty) {
	ReflEnsure(Properties.find(inProperty->GetID()) == Properties.end(), inProperty->GetName() + " is already registered, or an other property have same hash value.");
	Properties[inProperty->GetID()] = inProperty;
}

void RClass::AddFunction(IFunctionPointer* inFunction) {
	ReflEnsure(Functions.find(inFunction->GetID()) == Functions.end(), inFunction->GetName() + " is already registered, or another function have the same hash value");
	Functions[inFunction->GetID()] = inFunction;
}

IFunctionPointer* RClass::GetFunction(const RUID ProeprtyID) const {
	const auto& Value = Functions.find(ProeprtyID);
	if (Value == Functions.end()) return nullptr;
	return Value->second;
}

RProperty* RClass::GetProperty(size_t PropertyId) const {
	const auto& Value = Properties.find(PropertyId);
	if (Value == Properties.end()) return nullptr;
	return Value->second;
}

void* RClass::NewType() {
	return NewObject(this);
}

void RClass::DeleteType(void* Memory) {
	// @TODO ADD PLACEMENT NEW AND DESTRUCTOR FUNCTION
	delete(Memory);
}

inline static std::unordered_map<size_t, RClass*>* Classes = nullptr;

std::unordered_map<size_t, RClass*>* GetClasses() {
	if (!Classes) Classes = new std::unordered_map<size_t, RClass*>();
	return Classes;
}

RClass* RClass::GetClass(const String& inClassName) {
	return GetClass(std::hash<String>{}(inClassName));
}

RClass* RClass::GetClass(size_t inClassId) {
	const auto& value = GetClasses()->find(inClassId);
	if (value == GetClasses()->end()) return nullptr;
	return value->second;
}

void RClass::RegisterClass_Internal(RClass* inClass) {
	(*GetClasses())[inClass->GetId()] = inClass;
}
