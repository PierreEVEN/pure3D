#include "Reflection/RClass.h"
#include <utility>
#include <unordered_map>
#include "Reflection/RProperty.h"


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
