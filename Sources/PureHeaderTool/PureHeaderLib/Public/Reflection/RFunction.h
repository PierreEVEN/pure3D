#pragma once
#include "Types/String.h"
#include <unordered_map>
#include <functional>
#include "ReflectionUtilities.h"
#include <any>

struct RType;



struct RFunction : public ReflectionObject {


	template<typename ReturnType, typename... Args>
	using MethodeFunc = std::function<ReturnType(void*, Args...)>;


	template<typename ReturnType, typename... Args>
	using BaseFunc = std::function<ReturnType(Args...)>;

	template<typename ReturnType, typename... Arguments>
	static BaseFunc<ReturnType, Arguments...> MakeFunction_Internal(std::function<ReturnType(Arguments...)> InFunction) {
		return
			BaseFunc<ReturnType, Arguments...>(
				[InFunction](Arguments&&... inArguments) -> ReturnType {
					return InFunction(std::forward<Arguments>(inArguments)...);
				});

	}

	template<typename ReturnType, typename Class, typename... Arguments>
	static RFunction* MakeFunction(const String& InFunctionName, ReturnType(Class::* Var) (Arguments...)) {
		return new RFunction(InFunctionName,
			std::make_any<MethodeFunc<ReturnType, Arguments...>>([Var](void* Target, Arguments&&... inArguments)->ReturnType {
				return MakeFunction_Internal<ReturnType, Class, Arguments...>(Var)(*(Class*)Target, std::forward<Arguments>(inArguments)...);
				}));
	}


	/**
	 * Execute function with given parameter types
	 */
	template<typename ReturnType, typename... Arguments>
	inline ReturnType Execute(void* Target, Arguments&&... inArguments) {
		return (*std::any_cast<MethodeFunc<ReturnType, Arguments...>>(&FunctionPointer)) (Target, std::forward<Arguments>(inArguments)...);
	}

	/**
	 * Ensure requested parameter types are valid
	 */
	template<typename ReturnType, typename... Arguments>
	inline bool IsValid() const {
		MethodeFunc<ReturnType, Arguments...> truc;
		return std::any_cast<MethodeFunc<ReturnType, Arguments...>>(&FunctionPointer);
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

