
#include "BasicObject.h"
#include "Reflection/RType.h"
#include "IO/Log.h"
#include "Reflection/RProperty.h"
#include "Reflection/RFunction.h"

int main() {
	RClass* Class1 = BasicStructure().GetClass();
	RClass* Class2 = BasicObject::GetStaticClass();

	ChildOneTwo Object(40, 28.4598, 32.5f);
	RProperty* A = Object.GetClass()->GetProperty("A");
	RProperty* B = Object.GetClass()->GetProperty("B");
	RProperty* C = Object.GetClass()->GetProperty("C");
	RProperty* D = Object.GetClass()->GetProperty("D");

	LOG(Class1->GetName() + " size : " + Class1->GetSize());
	LOG(Class2->GetName() + " size : " + Class2->GetSize());
	LOG(A->GetName() + " : " + *A->Get<int>(&Object));
	LOG(B->GetName() + " : " + *B->Get<double>(&Object));
	LOG(C->GetName() + " : " + *C->Get<float>(&Object));
	LOG(D->GetName() + " : " + D->Get<BasicStructure>(&Object)->A);

	RFunction<double, ChildOneTwo, int, int, int>* Func = new RFunction<double, ChildOneTwo, int, int, int>("MyFunc", &ChildOneTwo::FunctionB, nullptr, {});
	LOG(Func->GetName() + " : " + Func->Execute(Object, 10, 20, 30));

    return 0;
}	