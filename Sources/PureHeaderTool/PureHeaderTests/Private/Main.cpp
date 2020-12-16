
#include "BasicObject.h"
#include "Reflection/RType.h"
#include "IO/Log.h"
#include "Reflection/RProperty.h"
#include "Reflection/RFunction.h"
#include "Reflection/RConstructor.h"
#include "Reflection/ReflectionMacros.h"


template<typename From, typename To>
struct TCaster {
	constexpr inline static bool IsCastable = false;
};


template<>
struct TCaster<ChildOneTwo, ChildOne> {
	constexpr inline static bool IsCastable = true;
	ChildOne* Cast(ChildOneTwo* From) {
		return dynamic_cast<ChildOne*>(From);
	}
};


template<typename From, typename To>
To* Cast(From* inFrom) {
	return dynamic_cast<To*>(inFrom);
}

template<typename Class>
Class* Cast(RClass* ObjectClass, void* Object) {
	return nullptr;
}


int main() {
	/**
	 * Class tests
	 */
	RClass* MyClass = ChildOneTwo::GetStaticClass();
	if (!MyClass) return 0;
	LOG(MyClass->GetName() + " size : " + MyClass->GetSize());
	
	/**
	 * Instantiate tests
	 */
	void* MyObject = MyClass->InstantiateNew<int, double, float>(5, 20.4, 3.5f);
	if (!MyObject) return 0;

	ParentTwo* CastedParent = Cast<ParentTwo>(MyClass, MyObject);

	/**
	 * Inheritance test
	 */
	for (const auto& Parent : MyClass->GetParents()) {
		RProperty* ParentPropertyA = Parent->GetProperty("A");
		RProperty* ParentPropertyB = Parent->GetProperty("B");
		RProperty* ParentPropertyC = Parent->GetProperty("C");
		if (ParentPropertyA) LOG(Parent->GetName() + "->" + ParentPropertyA->GetName() + " : " + *ParentPropertyA->Get<int>(MyObject));
		if (ParentPropertyB) LOG(Parent->GetName() + "->" + ParentPropertyB->GetName() + " : " + *ParentPropertyB->Get<double>(MyObject));
		if (ParentPropertyC) LOG(Parent->GetName() + "->" + ParentPropertyC->GetName() + " : " + *ParentPropertyC->Get<float>(MyObject));
	}

	/**
	 * Properties tests
	 */
	RProperty* PropertyA = MyClass->GetProperty("A");
	RProperty* PropertyB = MyClass->GetProperty("B");
	RProperty* PropertyC = MyClass->GetProperty("C");
	RProperty* PropertyD = MyClass->GetProperty("D");
	if (PropertyA) LOG(PropertyA->GetName() + " : " + *PropertyA->Get<int>(MyObject));
	if (PropertyB) LOG(PropertyB->GetName() + " : " + *PropertyB->Get<double>(MyObject));
	if (PropertyC) LOG(PropertyC->GetName() + " : " + *PropertyC->Get<float>(MyObject));
	if (PropertyD) LOG(PropertyD->GetName() + " : " + PropertyD->Get<BasicStructure>(MyObject)->A);

	/**
	 * Function tests
	 */	
	RFunction<void, ChildOneTwo>* FuncA = MyClass->GetFunction<void, ChildOneTwo>("FunctionA");
	RFunction<double, ChildOneTwo, int, int, int>* FuncB = MyClass->GetFunction<double, ChildOneTwo, int, int, int>("FunctionB");
	if (FuncA) LOG(FuncA->GetName()); FuncA->Execute((ChildOneTwo*)MyObject);
	if (FuncB) LOG(FuncB->GetName() + " : " + FuncB->Execute((ChildOneTwo*)MyObject, 10, 20, 30));
}