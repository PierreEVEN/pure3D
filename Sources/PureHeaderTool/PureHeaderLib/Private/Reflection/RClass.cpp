#include "Reflection/RClass.h"
#include <utility>
#include <unordered_map>
#include "Reflection/RProperty.h"
#include "Reflection/RFunction.h"


inline static std::unordered_map<String, const RClass*>* Classes = nullptr;


void RClass::AddParent(const String& inParent)
{
	//ReflEnsure(std::find(Parents.begin(), Parents.end(), inParent) != Parents.end(), (std::string("Cannot add the same parent class ") + inParent->GetName() + " twice for " + GetName()).c_str());
	//Parents.push_back(inParent);
}

void RClass::AddProperty(RProperty* inProperty) {
	ReflEnsure(Properties.find(inProperty->GetName()) == Properties.end(), inProperty->GetName() + " is already registered.");
	Properties[inProperty->GetName()] = inProperty;
}

void RClass::AddFunction(IFunctionPointer* inFunction)
{
	ReflEnsure(Functions.find(inFunction->GetName()) == Functions.end(), inFunction->GetName() + " is already registered.");
	Functions[inFunction->GetName()] = inFunction;
}

void RClass::AddConstructor(IFunctionPointer* inConstructor) {
	ReflEnsure(Constructors.find(inConstructor->GetName()) == Constructors.end(), inConstructor->GetName() + " is already registered.");
	Constructors[inConstructor->GetName()] = inConstructor;
}

IFunctionPointer* RClass::GetFunction(const String& FunctionName) const {
	const auto& Value = Functions.find(FunctionName);
	if (Value == Functions.end()) return nullptr;
	return Value->second;
}

IFunctionPointer* RClass::GetConstructor(const String& ConstructorName) const {
	const auto& Value = Constructors.find(ConstructorName);
	if (Value == Constructors.end()) return nullptr;
	return Value->second;
}

RProperty* RClass::GetProperty(const String& PropertyName) const {
	const auto& Value = Properties.find(PropertyName);
	if (Value == Properties.end()) return nullptr;
	return Value->second;
}

const RClass* RClass::GetClass(const String& inClassName) {
	if (!Classes) Classes = new std::unordered_map<String, const RClass*>();
	const auto& value = Classes->find(inClassName);
	ReflEnsure(value != Classes->end(), (String(inClassName) + " is not a reflected class").GetData());
	return value->second;
}

void RClass::RegisterClass_Internal(const String& inClassName, const RClass* inClass) {
	if (!Classes) Classes = new std::unordered_map<String, const RClass*>();
	ReflEnsure(Classes->find(inClassName) == Classes->end(), (String("class ") + inClassName + " is already registered").GetData());
	(*Classes)[inClassName] = inClass;
}
