
#include "BasicObject.h"
#include "Reflection/RType.h"
#include "IO/Log.h"
#include "Reflection/RProperty.h"
#include "Reflection/RFunction.h"
#include "Reflection/RConstructor.h"

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
}