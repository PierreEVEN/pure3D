#pragma once
#include "Types/String.h"
#include <vector>

namespace Parser {
	enum class EObjectType {
		ObjType_None = 0,
		ObjType_Enum = 1,
		ObjType_Struct = 2,
		ObjType_Class = 3
	};


	enum class EPropertyFlag {
		EPropFlag_Any = 0,
		EPropFlag_Const = 1 << 1,
		EPropFlag_Ptr = 1 << 2,
		EPropFlag_Ref = 1 << 3
	};
	inline EPropertyFlag operator|(EPropertyFlag a, EPropertyFlag b) { return static_cast<EPropertyFlag>(static_cast<int>(a) | static_cast<int>(b)); }
	inline EPropertyFlag& operator|= (EPropertyFlag& a, EPropertyFlag b) { return (EPropertyFlag&)((int&)a |= (int)b); }

	String ObjectTypeToString(const EObjectType& inType);

	struct SPropertyData {
		SPropertyData(const String& Type, const String& Name, bool InIsTransient);
		String PropertyType;
		bool IsDynamicRegisteredType;
		String PropertyName;
		bool IsTransient;
	};

	struct SFunctionData {
		String ReturnType;
		String FunctionName;
		std::vector<SPropertyData> Parameters;
	};
}