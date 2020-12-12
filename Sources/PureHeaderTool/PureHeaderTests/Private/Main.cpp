
#include "BasicObject.h"
#include "Reflection/RType.h"
#include "IO/Log.h"

REFL_DECLARE_TYPENAME(int);
REFL_DECLARE_TYPENAME(float);
REFL_DECLARE_TYPENAME(double);

REFL_DECLARE_TYPENAME(BasicObject);

int main() {

	REFL_REGISTER_TYPE(int);
	REFL_REGISTER_TYPE(float);
	REFL_REGISTER_TYPE(double);

	REFL_REGISTER_CLASS(BasicObject);

	const RClass* cl = RClass::GetClass<BasicObject>();
	const RType* ty1 = RType::GetType<double>();
	const RType* ty2 = RType::GetType("float");

	LOG(cl->GetName() + " size : " + cl->GetSize());
	LOG(ty1->GetName() + " size : " + ty1->GetSize());
	LOG(ty2->GetName() + " size : " + ty2->GetSize());

    return 0;
}