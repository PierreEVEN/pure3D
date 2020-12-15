
#include "BasicObject.h"
#include "Reflection/RType.h"
#include "IO/Log.h"
#include "Reflection/RProperty.h"
#include "Reflection/RFunction.h"
#include "Reflection/RConstructor.h"






struct RTestFunction {

	template<typename Return, typename Class, typename... Arguments>
	using RFunctionPtr = std::function<Return (Class, Arguments...)>;

	/**
	 * Register new constructor
	 */
	template<typename Return, typename Class, typename... Arguments>
	inline static RTestFunction* MakeFunction(const RFunctionPtr<Return, Class, Arguments...>& Function) {
		return new RTestFunction(std::make_any<RFunctionPtr<Return, Class, Arguments...>>(Function));
	}

	/**
	 * Instantiate new object
	 */
	template<typename Return, typename Class, typename... Arguments>
	inline Return Execute(Arguments&&... inArguments) {

		// Are arguments valids
		if (const InstantiateFunc<Arguments...>* Factory = std::any_cast<InstantiateFunc<Arguments...>>(&ConstructorFunction)) {

			// Call instantiate func
			return (*Factory) (std::forward<Arguments>(inArguments)...);
		}

		return (*Factory) (std::forward<Arguments>(inArguments)...);

		// Wrong arguments, return null
		return nullptr;
	}

private:

	RTestFunction(const std::any& inFunction)
		: Function(inFunction) {}

	const std::any Function;
};


int main() {
	/**
	 * Class tests
	 */
	RClass* MyClass = ChildOneTwo::GetStaticClass();
	LOG(MyClass->GetName() + " size : " + MyClass->GetSize());

	/**
	 * Instantiate tests
	 */
	void* MyObject = MyClass->InstantiateNew<int, double, float>(5, 20.4, 3.5f);

	/**
	 * Properties tests
	 */
	RProperty* PropertyA = MyClass->GetProperty("A");
	RProperty* PropertyB = MyClass->GetProperty("B");
	RProperty* PropertyC = MyClass->GetProperty("C");
	RProperty* PropertyD = MyClass->GetProperty("D");
	LOG(PropertyA->GetName() + " : " + *PropertyA->Get<int>(MyObject));
	LOG(PropertyB->GetName() + " : " + *PropertyB->Get<double>(MyObject));
	LOG(PropertyC->GetName() + " : " + *PropertyC->Get<float>(MyObject));
	LOG(PropertyD->GetName() + " : " + PropertyD->Get<BasicStructure>(MyObject)->A);

	/**
	 * Function tests
	 */	
	RFunction<void, ChildOneTwo>* FuncA = MyClass->GetFunction<void, ChildOneTwo>("FunctionA");
	RFunction<double, ChildOneTwo, int, int, int>* FuncB = MyClass->GetFunction<double, ChildOneTwo, int, int, int>("FunctionB");
	LOG(FuncA->GetName()); FuncA->Execute((ChildOneTwo*)MyObject);
	LOG(FuncB->GetName() + " : " + FuncB->Execute((ChildOneTwo*)MyObject, 10, 20, 30));



	std::function<double(ChildOneTwo*, int, int, int)> test = &ChildOneTwo::FunctionB;
	
	double result = test((ChildOneTwo*)MyObject, 1, 2, 3);
	LOG(result);

	//test->Execute<void, ChildOneTwo, int, int, int>(this, );
}