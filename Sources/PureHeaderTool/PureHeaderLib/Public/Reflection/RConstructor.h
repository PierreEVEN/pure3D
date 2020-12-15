#pragma once

#include <any>

class RClass;

struct RConstructor {

	template<typename... Args>
	using InstantiateFunc = std::function<void* (Args...)>;

	/**
	 * Register new constructor
	 */
	template<typename Type, typename... Arguments>
	inline static RConstructor* MakeConstructor() {

		// Make any of constructor lambda
		return new RConstructor(std::make_any<InstantiateFunc<Arguments...>>(

			// Constructor lambda
			[](Arguments&&... inArguments) -> void* {
				// Instantiate new Type
				return new Type(std::forward<Arguments>(inArguments)...);
			})
		);
	}

	/**
	 * Instantiate new object
	 */
	template<typename... Arguments>
	inline void* InstanciateNew(Arguments&&... inArguments) {

		// Are arguments valids
		if (const InstantiateFunc<Arguments...>* Factory = std::any_cast<InstantiateFunc<Arguments...>>(&ConstructorFunction)) {

			// Call instantiate func
			return (*Factory) (std::forward<Arguments>(inArguments)...);
		}

		// Wrong arguments, return null
		return nullptr;
	}

private:

	RConstructor(const std::any& inConstructorFunction)
		: ConstructorFunction(inConstructorFunction) {}

	const std::any ConstructorFunction;
};

template<typename T, typename... Arguments>
T* NewObject(RClass* inClass, Arguments... inArguments) {
	return reinterpret_cast<T*>(inClass->InstantiateNew<Arguments...>(std::forward<Arguments>(inArguments)...));
}
