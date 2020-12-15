
#include "BasicObject.h"
#include "Reflection/RType.h"
#include "IO/Log.h"
#include "Reflection/RProperty.h"
#include "Reflection/RFunction.h"
#include <any>



template <typename Type, typename... Arguments> 
struct Constructor {
	constexpr static bool IsValid = false;
};


#define CONSTRUCTOR(Type, ...) \
template <> \
struct Constructor<Type, __VA_ARGS__> { \
	constexpr static bool IsValid = true; \
\
	inline static std::function<void* (Type, __VA_ARGS__)> ConstructorPtr; \
}



// Dans le .refl.h
class MyClass;
CONSTRUCTOR(MyClass, float, int);
CONSTRUCTOR(MyClass);
// Dans le .refl.h


class MyClass {
public:

	void* MyClass_Factory_int_float(float A, int B) { return new MyClass(A, B); }

	void* MyClass_Factory() { return new MyClass(); }


	MyClass() {

	}

	MyClass(float A, int B) {

	}
};





int main() {
	RClass* Class1 = BasicStructure().GetClass();
	RClass* Class2 = BasicObject::GetStaticClass();

	ChildOneTwo Object(40, 28.4598, 32.5f);
	RProperty* A = Object.GetClass()->GetProperty("A");
	RProperty* B = Object.GetClass()->GetProperty("B");
	RProperty* C = Object.GetClass()->GetProperty("C");
	RProperty* D = Object.GetClass()->GetProperty("D");
	RFunction<void, ChildOneTwo>* FuncA = Object.GetClass()->GetFunction<void, ChildOneTwo>("FunctionA");
	RFunction<double, ChildOneTwo, int, int, int>* FuncB = Object.GetClass()->GetFunction<double, ChildOneTwo, int, int, int>("FunctionB");

	LOG(Class1->GetName() + " size : " + Class1->GetSize());
	LOG(Class2->GetName() + " size : " + Class2->GetSize());
	LOG(A->GetName() + " : " + *A->Get<int>(&Object));
	LOG(B->GetName() + " : " + *B->Get<double>(&Object));
	LOG(C->GetName() + " : " + *C->Get<float>(&Object));
	LOG(D->GetName() + " : " + D->Get<BasicStructure>(&Object)->A);
	LOG(FuncA->GetName()); FuncA->Execute(Object);
	LOG(FuncB->GetName() + " : " + FuncB->Execute(Object, 10, 20, 30));

	std::any test = 546;

	std::cout << test.type().name() << std::endl;


	// Dans le .refl.cpp
	static_assert(Constructor<MyClass>::IsValid, "MyClass doesn't have any default constructor");
	Constructor<MyClass, float, int>::ConstructorPtr = std::function<void* (MyClass, float, int)>(&MyClass::MyClass_Factory_int_float);
	Constructor<MyClass>::ConstructorPtr = std::function<void* (MyClass)>(&MyClass::MyClass_Factory);
	// Dans le .refl.cpp



    return 0;
}