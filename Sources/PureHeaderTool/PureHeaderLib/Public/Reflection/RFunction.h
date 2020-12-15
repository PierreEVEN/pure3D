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

template<typename ClassName, typename... Arguments>
struct RConstructor : public IFunctionPointer {
	RConstructor(const String& inFunctionName, const std::function<void* (ClassName, Arguments...)>& inFunctionPointer, const std::unordered_map<String, const RType*>& inFunctionRArguments)
		: IFunctionPointer(inFunctionName), FunctionPointer(inFunctionPointer), FunctionRArguments(inFunctionRArguments) {}

private:

	const std::function<void* (Arguments...)> Constructor;
	std::unordered_map<String, const RType*> FunctionRArguments;
};


template<typename ReturnType, typename ClassName, typename... Arguments>
struct RFunction : public IFunctionPointer {
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