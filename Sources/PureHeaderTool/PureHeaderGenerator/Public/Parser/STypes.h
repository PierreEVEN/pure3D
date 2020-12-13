#pragma once
#include "Types/String.h"

namespace Parser {
	enum class EObjectType {
		ObjType_None = 0,
		ObjType_Enum = 1,
		ObjType_Struct = 2,
		ObjType_Class = 3
	};

	String ObjectTypeToString(const EObjectType& inType);

	struct SPropertyData {
		String PropertyType;
		String PropertyName;
	};

	struct SFunctionData {
		String ReturnType;
		String FunctionName;
		std::vector<SPropertyData> Parameters;
	};
}