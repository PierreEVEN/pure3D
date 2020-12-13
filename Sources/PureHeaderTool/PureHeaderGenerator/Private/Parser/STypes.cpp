#include "Parser/STypes.h"

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

