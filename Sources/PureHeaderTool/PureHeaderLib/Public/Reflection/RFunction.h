#pragma once
#include "Types/String.h"
#include <unordered_map>
#include <functional>

struct RType;

struct IFunctionPointer {
public:
	IFunctionPointer(const String& inFunctionName) : FunctionName(inFunctionName) {}

	inline const String& GetName() const { return FunctionName;  }
private:
	String FunctionName;
};

template<typename ReturnType, typename ClassName, typename... Arguments>
struct RFunction : IFunctionPointer{
	RFunction(const String& inFunctionName, const std::function<ReturnType(ClassName, Arguments...)>& inFunctionPointer, const RType* inReturnRType, const std::unordered_map<String, const RType*>& inFunctionRArguments)
		: IFunctionPointer(inFunctionName), FunctionPointer(inFunctionPointer), ReturnRType(inReturnRType), FunctionRArguments(inFunctionRArguments) {}

	inline ReturnType Execute(const ClassName& Target, Arguments&&... inArguments) { 
		return FunctionPointer(Target, std::forward<Arguments>(inArguments)...);
	}

private:

	const std::function<ReturnType(ClassName, Arguments...)> FunctionPointer;
	const RType* ReturnRType;
	std::unordered_map<String, const RType*> FunctionRArguments;
};