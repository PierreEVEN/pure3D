#pragma once
#include "Types/String.h"
#include <unordered_map>
#include <functional>
#include "ReflectionUtilities.h"

struct RType;

struct IFunctionPointer : public ReflectionObject {
public:
	IFunctionPointer(const String& inFunctionName) : FunctionName(inFunctionName), FunctionID(MakeUniqueID(inFunctionName)) {}

	inline const String& GetName() const { return FunctionName; }
	inline const RUID GetID() const { return FunctionID; }
private:
	const String FunctionName;
	const RUID FunctionID;
};

template<typename ReturnType, typename ClassName, typename... Arguments>
struct RFunction : public IFunctionPointer {
	RFunction(const String& inFunctionName, const std::function<ReturnType(ClassName, Arguments...)>& inFunctionPointer, const RType* inReturnRType, const std::unordered_map<String, const RType*>& inFunctionRArguments)
		: IFunctionPointer(inFunctionName), FunctionPointer(inFunctionPointer), ReturnRType(inReturnRType), FunctionRArguments(inFunctionRArguments) {}

	inline ReturnType Execute(ClassName* Target, Arguments&&... inArguments) { 
		return FunctionPointer(*Target, std::forward<Arguments>(inArguments)...);
	}

private:

	const std::function<ReturnType(ClassName, Arguments...)> FunctionPointer;
	const RType* ReturnRType;
	std::unordered_map<String, const RType*> FunctionRArguments;
};