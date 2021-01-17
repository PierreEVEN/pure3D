#include "Parser/STypes.h"
#include "Utils/Utils.h"

using namespace Parser;

String Parser::ObjectTypeToString(const EObjectType& inType) {
	switch (inType)
	{
	case EObjectType::ObjType_Enum:
		return "enum class";

	case EObjectType::ObjType_Struct:
		return "struct";

	case EObjectType::ObjType_Class:
		return "class";

	default:
		return "unknown_structure";
	}
}


Parser::SPropertyData::SPropertyData(String Type, const String& Name, bool InIsTransient) {
	IsTransient = InIsTransient;
	PropertyName = String::RemoveBorderSpaces(Name);
	bool bIsConst = String::RemoveBorderSpaces(Type).IsStartingWith("const ");
	if (bIsConst) Type = Type.SubString(strlen("const "));
	PropertyType = String::Replace(String::Replace(String::RemoveBorderSpaces(Type), ' ', ""), '\t', "");
	if (bIsConst) PropertyType = "const " + PropertyType;
	IsDynamicRegisteredType = PropertyType.Contains('<') && !PropertyType.Contains("*") && !PropertyType.Contains("&");
}
