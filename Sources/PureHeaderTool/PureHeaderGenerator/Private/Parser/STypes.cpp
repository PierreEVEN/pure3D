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


Parser::SPropertyData::SPropertyData(const String& Type, const String& Name)
{
	PropertyName = String::RemoveBorderSpaces(Name);
	String FullType = String::RemoveBorderSpaces(Type);
	PropertyFlags = EPropertyFlag::EPropFlag_Any;
	if (FullType.Contains("&")) PropertyFlags |= EPropertyFlag::EPropFlag_Ref;
	if (FullType.Contains("*")) PropertyFlags |= EPropertyFlag::EPropFlag_Ptr;
	int TemplateReadStatus;
	for (int i = 0; i < FullType.Length(); ++i) {

		if (FullType[i] != '*' && FullType[i] != '&') PropertyType << FullType[i];

	}
	Utils::Log(PropertyType);
}
