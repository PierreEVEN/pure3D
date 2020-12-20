#include "Reflection/RClass.h"
#include <utility>
#include <unordered_map>
#include "Reflection/RProperty.h"
#include "Reflection/RFunction.h"


void RClass::AddParent(const String& inParent) {
	if (const RClass* FoundClass = RClass::GetClass(inParent))
		Parents.push_back(FoundClass);
	else
		RType::WaitTypeRegistration(inParent, this, &RClass::OnRegisterParentClass);
}

void RClass::OnRegisterParentClass(RType* RegisteredClass) {
	Parents.push_back((RClass*)RegisteredClass);
}

void RClass::AddProperty(RProperty* inProperty) {
	size_t Hash = std::hash<String>{}(inProperty->GetName());
	ReflEnsure(Properties.find(Hash) == Properties.end(), inProperty->GetName() + " is already registered, or an other property have same hash value.");
	Properties[Hash] = inProperty;
}

void RClass::AddFunction(IFunctionPointer* inFunction) {
	size_t Hash = std::hash<String>{}(inFunction->GetName());
	ReflEnsure(Functions.find(Hash) == Functions.end(), inFunction->GetName() + " is already registered, or another function have the same hash value");
	Functions[Hash] = inFunction;
}

IFunctionPointer* RClass::GetFunction(const String& FunctionName) const {
	const auto& Value = Functions.find(std::hash<String>{}(FunctionName));
	if (Value == Functions.end()) return nullptr;
	return Value->second;
}

RProperty* RClass::GetProperty(const String& PropertyName) const {
	const auto& Value = Properties.find(std::hash<String>{}(PropertyName));
	if (Value == Properties.end()) return nullptr;
	return Value->second;
}

const RClass* RClass::GetClass(const String& inClassName) {
	if (!Classes) Classes = new std::unordered_map<String, const RClass*>();
	const auto& value = Classes->find(inClassName);
	if (value == Classes->end()) return nullptr;
	return value->second;
}

void RClass::RegisterClass_Internal(const String& inClassName, const RClass* inClass) {
	if (!Classes) Classes = new std::unordered_map<String, const RClass*>();
	ReflEnsure(Classes->find(inClassName) == Classes->end(), (String("class ") + inClassName + " is already registered").GetData());
	(*Classes)[inClassName] = inClass;
}
