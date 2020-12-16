#include "Reflection/RClass.h"
#include <utility>
#include <unordered_map>
#include "Reflection/RProperty.h"
#include "Reflection/RFunction.h"


void RClass::AddParent(const String& inParent) {
	if (const RClass* FoundClass = RClass::GetClass(inParent))
		Parents.push_back(FoundClass);
	else
		RClass::WaitClassRegistration(inParent, this, &RClass::OnRegisterParentClass);
}

void RClass::OnRegisterParentClass(const RClass* RegisteredClass) {
	Parents.push_back(RegisteredClass);
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

IFunctionPointer* RClass::GetFunction(const String& FunctionName) const {
	const auto& Value = Functions.find(FunctionName);
	if (Value == Functions.end()) return nullptr;
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

	const auto& FoundDelegate = ClassRegistrationDelegate.find(inClassName);
	if (FoundDelegate != ClassRegistrationDelegate.end()) {
		FoundDelegate->second.Execute(inClass);
	}
}
