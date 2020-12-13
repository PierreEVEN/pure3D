
#include "BasicObject.h"
#include "Reflection/RType.h"
#include "IO/Log.h"

REFL_DECLARE_TYPENAME(bool);
REFL_DECLARE_TYPENAME(int32_t);
REFL_DECLARE_TYPENAME(uint32_t);
REFL_DECLARE_TYPENAME(int64_t);
REFL_DECLARE_TYPENAME(uint64_t);
REFL_DECLARE_TYPENAME(float);
REFL_DECLARE_TYPENAME(double);

int main() {

	REFL_REGISTER_TYPE(bool);
	REFL_REGISTER_TYPE(int32_t);
	REFL_REGISTER_TYPE(uint32_t);
	REFL_REGISTER_TYPE(int64_t);
	REFL_REGISTER_TYPE(uint64_t);
	REFL_REGISTER_TYPE(float);
	REFL_REGISTER_TYPE(double);

	const RClass* cl = RClass::GetClass<BasicObject>();
	const RType* ty1 = RType::GetType<double>();
	const RType* ty2 = RType::GetType("float");

	LOG(cl->GetName() + " size : " + cl->GetSize());
	LOG(ty1->GetName() + " size : " + ty1->GetSize());
	LOG(ty2->GetName() + " size : " + ty2->GetSize());
	
    return 0;
}