#pragma once
#include "Types/String.h"
#include <unordered_map>
#include <functional>
#include "ReflectionUtilities.h"
#include <any>

class RType;



struct RFunction : public ReflectionObject {


	template<typename ReturnType, typename... Args>
	using VoidFunction = std::function<ReturnType(void*, Args...)>;

	template<typename ReturnType, typename Class, typename... Arguments>
	static RFunction* MakeFunction(const String& InFunctionName, ReturnType(Class::* BaseMethod) (Arguments...)) {
		return new RFunction(InFunctionName,
			std::make_any<VoidFunction<ReturnType, Arguments...>>([BaseMethod](void* Target, Arguments&&... inArguments) -> ReturnType {
				return (((Class*)Target)->*BaseMethod)(std::forward<Arguments>(inArguments)...);
				}));
	}


	/**
	 * Execute function with given parameter types
	 */
	template<typename ReturnType, typename... Arguments>
	inline ReturnType Execute(void* Target, Arguments&&... inArguments) {
		return (*std::any_cast<VoidFunction<ReturnType, Arguments...>>(&FunctionPointer)) (Target, std::forward<Arguments>(inArguments)...);
	}

	/**
	 * Ensure requested parameter types are valid
	 */
	template<typename ReturnType, typename... Arguments>
	inline bool IsValid() const {
		VoidFunction<ReturnType, Arguments...> truc;
		return std::any_cast<VoidFunction<ReturnType, Arguments...>>(&FunctionPointer);
	}


	/**
	 * Get function name
	 */
	inline const String& GetName() const { return FunctionName; }

	/**
	 * Get function ID
	 */
	inline const RUID GetID() const { return FunctionID; }

	RFunction(const String& inFunctionName, const std::any inFunctionPointer)
		: FunctionName(inFunctionName), FunctionID(MakeUniqueID(inFunctionName)), FunctionPointer(inFunctionPointer) {
	}

	/**
	 * Function name
	 */
	const String FunctionName;

	/**
	 * Function ID
	 */
	const RUID FunctionID;

	/**
	 * Function Pointer
	 */
	const std::any FunctionPointer;
};

